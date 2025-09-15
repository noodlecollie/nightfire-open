#include "botrix/waypoint.h"
#include <limits>
#include "EnginePublicAPI/eiface.h"
#include "MathLib/vec3.h"
#include "botrix/type2string.h"
#include "botrix/players.h"
#include "botrix/defines.h"
#include "botrix/item.h"
#include "botrix/clients.h"
#include "botrix/parameter_vars.h"
#include "mod.h"
#include "engine_util.h"
#include "enginecallback.h"
#include "standard_includes.h"
#include "PlatformLib/String.h"
#include "MathLib/utils.h"
#include "customGeometry/sharedDefs.h"
#include "customGeometry/geometryItem.h"
#include "customGeometry/messageWriter.h"
#include "customGeometry/primitiveMessageWriter.h"
#include "types.h"
#include "util.h"

namespace Botrix
{
//----------------------------------------------------------------------------------------------------------------
// Waypoints file header.
//----------------------------------------------------------------------------------------------------------------
#pragma pack(push)
#pragma pack(1)
	struct waypoint_header
	{
		int szFileType;
		char szMapName[64];
		int iVersion;
		int iNumWaypoints;
		int iFlags;
		uint32_t mapCrc;
	};
#pragma pack(pop)

	static constexpr const char* const WAYPOINT_FILE_HEADER_ID = "BtxW";  // Botrix's Waypoints.

	static constexpr int WAYPOINT_VERSION = 2;  // Waypoints file version.
	static constexpr int WAYPOINT_FILE_FLAG_VISIBILITY = 1 << 0;  // Flag for waypoint visibility table.
	static constexpr int WAYPOINT_FILE_FLAG_AREAS = 1 << 1;  // Flag for area names.

	//----------------------------------------------------------------------------------------------------------------
	// CWaypoint static members.
	//----------------------------------------------------------------------------------------------------------------
	int CWaypoint::iWaypointTexture = -1;
	int CWaypoint::iDefaultDistance = 144;

	int CWaypoint::iUnreachablePathFailuresToDelete = 4;

	int CWaypoint::iAnalyzeDistance = 96;
	int CWaypoint::iWaypointsMaxCountToAnalyzeMap = 64;

#if defined(DEBUG) || defined(_DEBUG)
	float CWaypoint::fAnalyzeWaypointsPerFrame = 1;
#else
	float CWaypoint::fAnalyzeWaypointsPerFrame = 0.25f;
#endif

	bool CWaypoint::bSaveOnMapChange = false;
	bool CWaypoint::bShowAnalyzePotentialWaypoints = false;
	bool CWaypoint::bAnalyzeTraceAll = false;

	const TWaypointFlags CWaypoint::m_aFlagsForEntityType[EItemTypeCanPickTotal] = {
		FWaypointHealth | FWaypointHealthMachine,
		FWaypointArmor | FWaypointArmorMachine,
		FWaypointWeapon,
		FWaypointAmmo,
	};

	StringVector CWaypoints::m_aAreas;
	CWaypoints::WaypointGraph CWaypoints::m_cGraph;
	CWaypoints::Bucket CWaypoints::m_cBuckets[CWaypoints::BUCKETS_SIZE_X][CWaypoints::BUCKETS_SIZE_Y]
											 [CWaypoints::BUCKETS_SIZE_Z];

	good::vector<good::bitset> CWaypoints::m_aVisTable;
	bool CWaypoints::bValidVisibilityTable = false;

	good::vector<CWaypoints::unreachable_path_t> CWaypoints::m_aUnreachablePaths;

	// Sometimes the analyze doesn't add waypoints in small passages, so we try to add waypoints at inter-position
	// between waypoint analyzed neighbours. Waypoint analyzed neighbours are x's. W = analyzed waypoint.
	//
	// x - x - x
	// |   |   |
	// x - W - x
	// |   |   |
	// x - x - x
	//
	// '-' and '|' on the picture will be evaluated in 'inters' step, when the adjacent waypoints are not set for some
	// reason (like hit wall).
	good::vector<CWaypoints::CNeighbour> CWaypoints::m_aWaypointsNeighbours;
	good::vector<Vector> CWaypoints::m_aWaypointsToAddOmitInAnalyze[CWaypoints::EAnalyzeWaypointsTotal];

	CWaypoints::TAnalyzeStep CWaypoints::m_iAnalyzeStep = EAnalyzeStepTotal;
	float CWaypoints::m_fAnalyzeWaypointsForNextFrame = 0;
	edict_t* CWaypoints::m_pAnalyzer = NULL;
	bool CWaypoints::m_bIsAnalyzeStepAddedWaypoints = false;
	TWaypointId CWaypoints::m_iCurrentAnalyzeWaypoint = 0;

	static const float ANALIZE_HELP_IF_LESS_WAYPOINTS_PER_FRAME = 0.0011f;

	static bool ReadInto(byte*& cursor, byte* end, void* memory, size_t bytesToRead)
	{
		if ( bytesToRead < 1 || cursor + bytesToRead > end )
		{
			return false;
		}

		memcpy(memory, cursor, bytesToRead);
		cursor += bytesToRead;
		return true;
	}

	template<typename T>
	static bool Read(byte*& cursor, byte* end, T& out, const size_t bytesToRead = sizeof(T))
	{
		return ReadInto(cursor, end, &out, bytesToRead);
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoint::GetColor(unsigned char& r, unsigned char& g, unsigned char& b) const
	{
		if ( FLAG_SOME_SET(FWaypointStop, iFlags) )
		{
			r = 0x00;
			g = 0x00;
			b = 0xFF;  // Blue effect, stop.
		}
		else if ( FLAG_SOME_SET(FWaypointCamper | FWaypointSniper, iFlags) )
		{
			r = 0x66;
			g = 0x00;
			b = 0x00;  // Dark red effect, camper / sniper.
		}
		else if ( FLAG_SOME_SET(FWaypointLadder, iFlags) )
		{
			r = 0xFF;
			g = 0x00;
			b = 0x00;  // Red effect, ladder.
		}
		else if ( FLAG_SOME_SET(FWaypointWeapon, iFlags) )
		{
			r = 0xFF;
			g = 0xFF;
			b = 0x00;  // Light yellow effect, weapon.
		}
		else if ( FLAG_SOME_SET(FWaypointAmmo, iFlags) )
		{
			r = 0x66;
			g = 0x66;
			b = 0x00;  // Dark yellow effect, ammo.
		}
		else if ( FLAG_SOME_SET(FWaypointHealth, iFlags) )
		{
			r = 0xFF;
			g = 0xFF;
			b = 0xFF;  // Light white effect, health.
		}
		else if ( FLAG_SOME_SET(FWaypointHealthMachine, iFlags) )
		{
			r = 0x66;
			g = 0x66;
			b = 0x66;  // Gray effect, health machine.
		}
		else if ( FLAG_SOME_SET(FWaypointArmor, iFlags) )
		{
			r = 0x00;
			g = 0xFF;
			b = 0x00;  // Light green effect, armor.
		}
		else if ( FLAG_SOME_SET(FWaypointArmorMachine, iFlags) )
		{
			r = 0x00;
			g = 0x66;
			b = 0x00;  // Dark green effect, armor machine.
		}
		else if ( FLAG_SOME_SET(FWaypointButton | FWaypointSeeButton, iFlags) )
		{
			r = 0x8A;
			g = 0x2B;
			b = 0xE2;  // Violet effect, button.
		}
		else
		{
			r = 0x00;
			g = 0xFF;
			b = 0xFF;  // Cyan effect, other flags.
		}
	}

	static void FindLadderDismounts(const char* ladderName, CUtlVector<edict_t*>& out)
	{
		out.Purge();

		for ( edict_t* ent = FIND_ENTITY_BY_TARGET(nullptr, ladderName); !FNullEnt(ent);
			  ent = FIND_ENTITY_BY_TARGET(ent, ladderName) )
		{
			if ( FClassnameIs(&ent->v, "info_ladder_dismount") )
			{
				out.AddToTail(ent);
			}
		}

		// Ensure lower dismounts come before upper ones.
		out.Sort(
			[](edict_t* const* a, edict_t* const* b) -> int
			{
				if ( (*a)->v.origin[VEC3_Z] < (*b)->v.origin[VEC3_Z] )
				{
					return -1;
				}

				if ( (*a)->v.origin[VEC3_Z] > (*b)->v.origin[VEC3_Z] )
				{
					return 1;
				}

				return 0;
			}
		);
	}

	// Assumes PVS is already set.
	static bool IsVisibleFromViewer(
		const Vector& point,
		const Vector& viewPoint,
		const Vector& viewAngles,
		TVisibilities visibility,
		bool checkPVS = true
	)
	{
		// For now, given it's difficult to determine the exact FOV/aspect ratio of the viewer on the
		// server side, assume an FOV of 70 degrees either side, and a square aspect ratio.
		static constexpr float TOTAL_HORIZ_FOV = 70.0f * 2.0f;

		if ( !CBotrixEngineUtil::IsInViewPrism(point, viewPoint, viewAngles, TOTAL_HORIZ_FOV, 1.0f) )
		{
			return false;
		}

		if ( checkPVS && !CBotrixEngineUtil::IsVisiblePVS(point) )
		{
			return false;
		}

		// Never pass PVS as true here, since this would reset the stored PVS that may
		// have been set elsewhere.
		return CBotrixEngineUtil::IsVisible(viewPoint, point, visibility, false);
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoint::Draw(TWaypointId iWaypointId, TWaypointDrawFlags iDrawType, float fDrawTime) const
	{
		unsigned char r, g, b;  // Red, green, blue.
		GetColor(r, g, b);

		Vector vEnd = Vector(vOrigin.x, vOrigin.y, vOrigin.z - CBotrixParameterVars::PLAYER_EYE);

		if ( FLAG_ALL_SET_OR_0(FWaypointDrawBeam, iDrawType) )
		{
			CBotrixEngineUtil::DrawBeam(vOrigin, vEnd, WIDTH, fDrawTime, r, g, b);
		}

		if ( FLAG_ALL_SET_OR_0(FWaypointDrawLine, iDrawType) )
		{
			DrawLines(vOrigin, fDrawTime, r, g, b);
		}

		if ( FLAG_ALL_SET_OR_0(FWaypointDrawBox, iDrawType) )
		{
			Vector vBoxOrigin(
				vOrigin.x - (CBotrixParameterVars::PLAYER_WIDTH / 2.0f),
				vOrigin.y - (CBotrixParameterVars::PLAYER_WIDTH / 2.0f),
				vOrigin.z - CBotrixParameterVars::PLAYER_EYE
			);

			const float halfPlayerWidth = CBotrixParameterVars::PLAYER_WIDTH / 2.0f;

			CBotrixEngineUtil::DrawBox(
				vOrigin - Vector(0.0f, 0.0f, CBotrixParameterVars::PLAYER_EYE),
				Vector(-halfPlayerWidth, -halfPlayerWidth, 0.0f),
				Vector(halfPlayerWidth, halfPlayerWidth, CBotrixParameterVars::PLAYER_HEIGHT),
				fDrawTime,
				r,
				g,
				b
			);
		}

		if ( FLAG_ALL_SET_OR_0(FWaypointDrawText, iDrawType) )
		{
			char buffer[CustomGeometry::MAX_TEXT_CHARS_PER_MSG];

			PlatformLib_SNPrintF(buffer, sizeof(buffer), "%d", iWaypointId);
			Vector v = vOrigin;
			v.z -= 20.0f;
			int i = 0;

			CBotrixEngineUtil::DrawTextAtLocation(v, i++, fDrawTime, 0xFF, 0xFF, 0xFF, buffer);

			if ( iAreaId != 0 )
			{
				CBotrixEngineUtil::DrawTextAtLocation(
					v,
					i++,
					fDrawTime,
					0xFF,
					0xFF,
					0xFF,
					CWaypoints::GetAreas()[iAreaId].c_str()
				);
			}

			if ( FLAG_SOME_SET(FWaypointButton | FWaypointSeeButton, iFlags) )
			{
				PlatformLib_SNPrintF(
					buffer,
					sizeof(buffer),
					FLAG_SOME_SET(FWaypointSeeButton, iFlags) ? "see button %d" : "button %d",
					CWaypoint::GetButton(iArgument)
				);

				CBotrixEngineUtil::DrawTextAtLocation(v, i++, fDrawTime, 0xFF, 0xFF, 0xFF, buffer);

				PlatformLib_SNPrintF(
					buffer,
					sizeof(buffer),
					FLAG_SOME_SET(FWaypointElevator, iFlags) ? "for elevator %d" : "for door %d",
					CWaypoint::GetDoor(iArgument)
				);

				CBotrixEngineUtil::DrawTextAtLocation(v, i++, fDrawTime, 0xFF, 0xFF, 0xFF, buffer);
			}
			else
			{
				CBotrixEngineUtil::DrawTextAtLocation(
					v,
					i++,
					fDrawTime,
					0xFF,
					0xFF,
					0xFF,
					CTypeToString::WaypointFlagsToString(iFlags, false).c_str()
				);
			}
		}
	}

	void
	CWaypoint::DrawLines(const Vector& pos, float fDrawTime, unsigned char r, unsigned char g, unsigned char b) const
	{
		using namespace CustomGeometry;

		WaypointMarkerPrimitive primitive {};
		primitive.location = pos;
		CPrimitiveMessageWriter(Category::BotrixDebugging)
			.WriteMessage((r << 24) | (g << 16) | (b << 8) | 0xFF, fDrawTime, primitive);
	}

	//********************************************************************************************************************
	void CWaypoints::Clear()
	{
		CPlayers::InvalidatePlayersWaypoints();
		ClearLocations();
		m_cGraph.clear();
		m_aAreas.clear();
		m_aAreas.push_back("default");  // New waypoints without area id will be put under this empty area id.
		m_aUnreachablePaths.clear();
		m_aVisTable.clear();
		bValidVisibilityTable = false;
	}

	//********************************************************************************************************************
	bool CWaypoints::Save()
	{
		char filePath[512];
		PlatformLib_SNPrintF(filePath, sizeof(filePath), "waypoints/%s.way", CBotrixServerPlugin::MapName());

		BLOG_I("Saving map waypoints to file: %s", filePath);

		uint32_t crc = 0;

		if ( !g_engfuncs.pfnGetMapCRC(&crc) )
		{
			BLOG_E("Could not get map CRC to save waypoint file!");
			return false;
		}

		struct writable_file_s* outFile = g_engfuncs.pfnOpenWritableFile(filePath);

		if ( !outFile )
		{
			BLOG_E("Could not open %s for writing", filePath);
			return false;
		}

		waypoint_header header;
		memset(&header, 0, sizeof(header));
		header.iFlags = 0;
		FLAG_SET(WAYPOINT_FILE_FLAG_AREAS, header.iFlags);
		FLAG_SET(WAYPOINT_FILE_FLAG_VISIBILITY, header.iFlags);
		header.iNumWaypoints = m_cGraph.size();
		header.iVersion = WAYPOINT_VERSION;
		header.mapCrc = crc;
		header.szFileType = *((int*)&WAYPOINT_FILE_HEADER_ID[0]);
		PlatformLib_StrCpy(header.szMapName, sizeof(header.szMapName), CBotrixServerPlugin::MapName());

		// Write header.
		g_engfuncs.pfnWriteElementsToFile(outFile, &header, sizeof(waypoint_header), 1);

		// Write waypoints data.
		for ( WaypointNodeIt it = m_cGraph.begin(); it != m_cGraph.end(); ++it )
		{
			g_engfuncs.pfnWriteElementsToFile(outFile, &it->vertex.vOrigin, sizeof(Vector), 1);
			g_engfuncs.pfnWriteElementsToFile(outFile, &it->vertex.iFlags, sizeof(TWaypointFlags), 1);
			g_engfuncs.pfnWriteElementsToFile(outFile, &it->vertex.iAreaId, sizeof(TAreaId), 1);
			g_engfuncs.pfnWriteElementsToFile(outFile, &it->vertex.iArgument, sizeof(TWaypointArgument), 1);
		}

		// Write waypoints neighbours.
		for ( WaypointNodeIt it = m_cGraph.begin(); it != m_cGraph.end(); ++it )
		{
			int iNumPaths = it->neighbours.size();
			g_engfuncs.pfnWriteElementsToFile(outFile, &iNumPaths, sizeof(int), 1);

			for ( WaypointArcIt arcIt = it->neighbours.begin(); arcIt != it->neighbours.end(); ++arcIt )
			{
				g_engfuncs
					.pfnWriteElementsToFile(outFile, &arcIt->target, sizeof(TWaypointId), 1);  // Save waypoint id.
				g_engfuncs
					.pfnWriteElementsToFile(outFile, &arcIt->edge.iFlags, sizeof(TPathFlags), 1);  // Save path flags.
				g_engfuncs.pfnWriteElementsToFile(
					outFile,
					&arcIt->edge.iArgument,
					sizeof(TPathArgument),
					1
				);  // Save path arguments.
			}
		}

		// Save area names.
		int iAreaNamesSize = m_aAreas.size();
		BASSERT(iAreaNamesSize >= 0, iAreaNamesSize = 0);

		g_engfuncs.pfnWriteElementsToFile(outFile, &iAreaNamesSize, sizeof(int), 1);  // Save area names size.

		for ( int i = 1; i < iAreaNamesSize; i++ )  // First area name is always empty, for new waypoints.
		{
			int iSize = m_aAreas[i].size();
			g_engfuncs.pfnWriteElementsToFile(outFile, &iSize, 1, sizeof(int));  // Save string size.
			g_engfuncs
				.pfnWriteElementsToFile(outFile, m_aAreas[i].c_str(), 1, iSize + 1);  // Write string & trailing 0.
		}

		// Save waypoint visibility table.
		BLOG_I("Saving waypoint visibility table, this may take a while.");
		m_aVisTable.resize(Size());
		for ( TWaypointId i = 0; i < Size(); ++i )
		{
			good::bitset& cVisibles = m_aVisTable[i];
			cVisibles.resize(Size());

			Vector vFrom = Get(i).vOrigin;
			CBotrixEngineUtil::SetPVSForVector(vFrom);

			for ( TWaypointId j = 0; j < Size(); ++j )
			{
				if ( i < j )
				{
					Vector vTo = Get(j).vOrigin;
					cVisibles.set(
						j,
						CBotrixEngineUtil::IsVisiblePVS(vTo) &&
							CBotrixEngineUtil::IsVisible(vFrom, vTo, EVisibilityWorld, false)
					);
				}
				else
				{
					cVisibles.set(j, (i == j) || m_aVisTable[j].test(i));
				}
			}

			g_engfuncs.pfnWriteElementsToFile(outFile, cVisibles.data(), 1, cVisibles.byte_size());
		}

		// Save items marks.
		BLOG_I("Saving items marks.");

		static_assert(
			sizeof(TItemIndex) == sizeof(TItemFlags),
			"Expected sizeof(TItemIndex) to be the same as sizeof(TItemFlags)"
		);

		const good::vector<TItemId>& aItems = CItems::GetObjectsFlags();
		int iSize = aItems.size() / 2;
		g_engfuncs.pfnWriteElementsToFile(outFile, &iSize, sizeof(int), 1);

		for ( int i = 0; i < iSize; ++i )
		{
			TItemIndex iIndex = aItems[i * 2] - CPlayers::Size();  // Items indexes are relatives to player's count.
			g_engfuncs.pfnWriteElementsToFile(outFile, &iIndex, sizeof(TItemIndex), 1);
			g_engfuncs.pfnWriteElementsToFile(outFile, &aItems[i * 2 + 1], sizeof(TItemFlags), 1);
		}

		g_engfuncs.pfnCloseWritableFile(outFile);

		bValidVisibilityTable = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------------------
	bool CWaypoints::Load()
	{
		Clear();

		char filePath[512];
		PlatformLib_SNPrintF(filePath, sizeof(filePath), "waypoints/%s.way", CBotrixServerPlugin::MapName());

		BLOG_I("Loading map waypoints file: %s", filePath);

		int fileLength = 0;
		byte* fileData = LOAD_FILE(filePath, &fileLength);

		if ( !fileData || fileLength < 1 )
		{
			BLOG_W("No waypoints for map %s:", CBotrixServerPlugin::MapName());
			BLOG_W("  File '%s' doesn't exist.", filePath);
			return false;
		}

		uint32_t crc = 0;

		if ( !g_engfuncs.pfnGetMapCRC(&crc) )
		{
			BLOG_E("Could not get map CRC to validate loaded waypoint file!");
			return false;
		}

		byte* cursor = fileData;
		byte* const end = fileData + fileLength;
		bool success = false;

		struct waypoint_header header;
		success = Read(cursor, end, header);
		BASSERT(success, Clear(); FREE_FILE(fileData); return false);

		if ( *((int*)&WAYPOINT_FILE_HEADER_ID[0]) != header.szFileType )
		{
			BLOG_E("Error loading waypoints: invalid file header.");
			FREE_FILE(fileData);
			return false;
		}

		if ( header.iVersion != WAYPOINT_VERSION )
		{
			BLOG_E("Error loading waypoints, version mismatch:");
			BLOG_E("  File version %d, current waypoint version %d.", header.iVersion, WAYPOINT_VERSION);
			FREE_FILE(fileData);
			return false;
		}

		if ( !FStrEq(CBotrixServerPlugin::MapName(), header.szMapName) )
		{
			BLOG_W("Warning loading waypoints, map name mismatch:");
			BLOG_W("  File map %s, current map %s.", header.szMapName, CBotrixServerPlugin::MapName());
		}

		if ( header.mapCrc != crc )
		{
			BLOG_W("Warning loading waypoints, map CRC mismatch:");
			BLOG_W("  Current map CRC 0x%08x, waypoint file stored CRC 0x%08x.", crc, header.mapCrc);
			BLOG_W("The map may have been updated since the last waypoint analyze.");
		}

		Vector vOrigin;
		TWaypointFlags iFlags = 0;
		int iNumPaths = 0, iArgument = 0;
		TAreaId iAreaId = 0;

		// Read waypoints information.
		for ( TWaypointId i = 0; i < header.iNumWaypoints; ++i )
		{
			success = Read(cursor, end, vOrigin);
			BASSERT(success, Clear(); FREE_FILE(fileData); return false);

			success = Read(cursor, end, iFlags);
			BASSERT(success, Clear(); FREE_FILE(fileData); return false);

			success = Read(cursor, end, iAreaId);
			BASSERT(success, Clear(); FREE_FILE(fileData); return false);

			if ( FLAG_CLEARED(WAYPOINT_FILE_FLAG_AREAS, header.iFlags) )
			{
				iAreaId = 0;
			}

			success = Read(cursor, end, iArgument);
			BASSERT(success, Clear(); FREE_FILE(fileData); return false);

			Add(vOrigin, iFlags, iArgument, iAreaId);
		}

		// Read waypoints paths.
		TWaypointId iPathTo;
		TPathFlags iPathFlags = 0;
		TPathArgument iPathArgument = 0;

		for ( TWaypointId i = 0; i < header.iNumWaypoints; ++i )
		{
			WaypointGraph::node_it from = m_cGraph.begin() + i;

			success = Read(cursor, end, iNumPaths);
			BASSERT(
				success && (0 <= iNumPaths) && (iNumPaths < header.iNumWaypoints), Clear(); FREE_FILE(fileData);
				return false
			);

			m_cGraph[i].neighbours.reserve(iNumPaths);

			for ( int n = 0; n < iNumPaths; n++ )
			{
				success = Read(cursor, end, iPathTo);
				BASSERT(
					success && (0 <= iPathTo) && (iPathTo < header.iNumWaypoints), Clear(); FREE_FILE(fileData);
					return false
				);

				success = Read(cursor, end, iPathFlags);
				BASSERT(success, Clear(); FREE_FILE(fileData); return false);

				success = Read(cursor, end, iPathArgument);
				BASSERT(success, Clear(); FREE_FILE(fileData); return false);

				WaypointGraph::node_it to = m_cGraph.begin() + iPathTo;
				const float vDist = (to->vertex.vOrigin - from->vertex.vOrigin).Length();
				m_cGraph.add_arc(from, to, CWaypointPath(vDist, iPathFlags, iPathArgument));
			}
		}

		int iAreaNamesSize = 0;

		if ( FLAG_SOME_SET(WAYPOINT_FILE_FLAG_AREAS, header.iFlags) )
		{
			// Read area names.
			success = Read(cursor, end, iAreaNamesSize);
			BASSERT(success, Clear(); FREE_FILE(fileData); return false);
			// BASSERT( (0 <= iAreaNamesSize) && (iAreaNamesSize <= header.iNumWaypoints), Clear();fclose(f);return
			// false );

			m_aAreas.reserve(iAreaNamesSize);
			m_aAreas.push_back("default");  // New waypoints without area id will be put under this empty area id.

			for ( int i = 1; i < iAreaNamesSize; i++ )
			{
				int iStrSize = 0;
				success = Read(cursor, end, iStrSize);
				BASSERT(success && iStrSize < 512, Clear(); FREE_FILE(fileData); return false);

				if ( iStrSize > 0 )
				{
					// NFTODO: Don't really like this method.
					char areaNameStr[512];
					success = ReadInto(cursor, end, areaNameStr, iStrSize + 1);  // Read also trailing 0.
					BASSERT(success, Clear(); FREE_FILE(fileData); return false);

					good::string sArea(areaNameStr, true, true, iStrSize);
					m_aAreas.push_back(sArea);
				}
			}
		}
		else
		{
			m_aAreas.push_back("default");  // New waypoints without area id will be put under this empty area id.
		}

		// Check for areas names.
		iAreaNamesSize = m_aAreas.size();
		for ( TWaypointId i = 0; i < header.iNumWaypoints; ++i )
		{
			if ( m_cGraph[i].vertex.iAreaId >= iAreaNamesSize )
			{
				BreakDebugger();
				m_cGraph[i].vertex.iAreaId = 0;
			}
		}

		bValidVisibilityTable = header.iFlags & WAYPOINT_FILE_FLAG_VISIBILITY;
		if ( bValidVisibilityTable )
		{
			m_aVisTable.resize(header.iNumWaypoints);

			for ( TWaypointId i = 0; i < Size(); ++i )
			{
				m_aVisTable[i].resize(header.iNumWaypoints);
				int iByteSize = m_aVisTable[i].byte_size();

				success = ReadInto(cursor, end, m_aVisTable[i].data(), iByteSize);

				if ( !success )
				{
					BLOG_E("Invalid waypoints visibility table.");
					Clear();
					FREE_FILE(fileData);
					return false;
				}
			}

			BLOG_I("Waypoints visibility table loaded.");
		}
		else
		{
			BLOG_W("No waypoints visibility table in file.");
		}

		// Load items marks.
		int iItemsCount = 0;

		if ( Read(cursor, end, iItemsCount) && iItemsCount > 0 )
		{
			good::vector<TItemId> aItems;
			aItems.resize(iItemsCount * 2);

			success = ReadInto(cursor, end, &aItems[0], iItemsCount * 2 * sizeof(TItemIndex));
			BASSERT(success, Clear(); FREE_FILE(fileData); return false);

			for ( int i = 0; i < iItemsCount; ++i )
			{
				aItems[i * 2] += CPlayers::Size();
				BLOG_D(
					"Object id %d: %s.",
					aItems[i * 2],
					CTypeToString::EntityClassFlagsToString(aItems[i * 2 + 1]).c_str()
				);
			}

			CItems::SetObjectsFlags(aItems);

			BLOG_I("Loaded %d object marks.", iItemsCount);
		}

		FREE_FILE(fileData);
		return true;
	}

	//----------------------------------------------------------------------------------------------------------------
	TWaypointId CWaypoints::GetRandomNeighbour(TWaypointId iWaypoint, TWaypointId iTo, bool bVisible)
	{
		const WaypointNode::arcs_t& aNeighbours = GetNode(iWaypoint).neighbours;

		if ( !aNeighbours.size() )
		{
			return EWaypointIdInvalid;
		}

		TWaypointId iResult = rand() % aNeighbours.size();

		if ( bValidVisibilityTable && CWaypoint::IsValid(iTo) )
		{
			for ( int i = 0; i < aNeighbours.size(); ++i )
			{
				TWaypointId iNeighbour = aNeighbours[iResult].target;
				if ( m_aVisTable[iNeighbour].test(iTo) == bVisible )
				{
					return iNeighbour;
				}

				if ( ++iResult == aNeighbours.size() )
				{
					iResult = 0;
				}
			}
		}

		return aNeighbours[iResult].target;
	}

	//----------------------------------------------------------------------------------------------------------------
	TWaypointId CWaypoints::GetNearestNeighbour(TWaypointId iWaypoint, TWaypointId iTo, bool bVisible)
	{
		GoodAssert(bValidVisibilityTable);
		const WaypointNode::arcs_t& aNeighbours = GetNode(iWaypoint).neighbours;

		if ( !aNeighbours.size() )
		{
			return EWaypointIdInvalid;
		}

		Vector vTo = Get(iTo).vOrigin;

		TWaypointId iResult = aNeighbours[0].target;
		float fMinDist = (vTo - Get(iResult).vOrigin).LengthSquared();
		bool bResultVisibleOk = (m_aVisTable[iResult].test(iTo) == bVisible);

		for ( int i = 1; i < aNeighbours.size(); ++i )
		{
			TWaypointId iNeighbour = aNeighbours[i].target;
			bool bVisibleNeighbourOk = (m_aVisTable[iNeighbour].test(iTo) == bVisible);

			if ( bResultVisibleOk && !bVisibleNeighbourOk )
			{
				continue;
			}

			Vector& vOrigin = Get(iNeighbour).vOrigin;
			float fDist = (vTo - vOrigin).LengthSquared();

			if ( fDist < fMinDist )
			{
				bVisible = bVisibleNeighbourOk;
				fMinDist = fDist;
				iResult = iNeighbour;
			}
		}

		return iResult;
	}

	//----------------------------------------------------------------------------------------------------------------
	TWaypointId CWaypoints::GetFurthestNeighbour(TWaypointId iWaypoint, TWaypointId iTo, bool bVisible)
	{
		GoodAssert(bValidVisibilityTable);
		const WaypointNode::arcs_t& aNeighbours = GetNode(iWaypoint).neighbours;

		if ( !aNeighbours.size() )
		{
			return EWaypointIdInvalid;
		}

		Vector vTo = Get(iTo).vOrigin;

		TWaypointId iResult = aNeighbours[0].target;
		float fMaxDist = (vTo - Get(iResult).vOrigin).LengthSquared();
		bool bResultVisibleOk = (m_aVisTable[iResult].test(iTo) == bVisible);

		for ( int i = 1; i < aNeighbours.size(); ++i )
		{
			TWaypointId iNeighbour = aNeighbours[i].target;
			bool bVisibleNeighbourOk = (m_aVisTable[iNeighbour].test(iTo) == bVisible);

			if ( bResultVisibleOk && !bVisibleNeighbourOk )
			{
				continue;
			}

			Vector& vOrigin = Get(iNeighbour).vOrigin;
			float fDist = (vTo - vOrigin).LengthSquared();

			if ( fDist > fMaxDist )
			{
				bVisible = bVisibleNeighbourOk;
				fMaxDist = fDist;
				iResult = iNeighbour;
			}
		}

		return iResult;
	}

	//----------------------------------------------------------------------------------------------------------------
	CWaypointPath* CWaypoints::GetPath(TWaypointId iFrom, TWaypointId iTo)
	{
		WaypointGraph::node_t& from = m_cGraph[iFrom];

		for ( WaypointGraph::arc_it it = from.neighbours.begin(); it != from.neighbours.end(); ++it )
		{
			if ( it->target == iTo )
			{
				return &(it->edge);
			}
		}

		return NULL;
	}

	//----------------------------------------------------------------------------------------------------------------
	TWaypointId CWaypoints::Add(const Vector& vOrigin, TWaypointFlags iFlags, int iArgument, int iAreaId)
	{
		CWaypoint w(vOrigin, iFlags, iArgument, static_cast<TAreaId>(iAreaId));

		// lol, this is not working because m_cGraph.begin() is called first. wtf?
		// TWaypointId id = m_cGraph.add_node(w) - m_cGraph.begin();
		CWaypoints::WaypointNodeIt it(m_cGraph.add_node(w));
		TWaypointId id = it - m_cGraph.begin();

		it->neighbours.reserve(8);

		AddLocation(id, vOrigin);
		bValidVisibilityTable = false;
		return id;
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoints::Remove(TWaypointId id, bool bResetPlayers)
	{
		if ( bResetPlayers )
			CPlayers::InvalidatePlayersWaypoints();

		DecrementLocationIds(id);
		m_cGraph.delete_node(m_cGraph.begin() + id);
		bValidVisibilityTable = false;
	}

	//----------------------------------------------------------------------------------------------------------------
	bool CWaypoints::AddPath(TWaypointId iFrom, TWaypointId iTo, float fDistance, TPathFlags iFlags)
	{
		if ( !CWaypoint::IsValid(iFrom) || !CWaypoint::IsValid(iTo) || (iFrom == iTo) || HasPath(iFrom, iTo) )
		{
			return false;  // Can happen with commands.
		}

		WaypointGraph::node_it from = m_cGraph.begin() + iFrom;
		WaypointGraph::node_it to = m_cGraph.begin() + iTo;

		if ( fDistance <= 0.0f )
		{
			fDistance = (to->vertex.vOrigin - from->vertex.vOrigin).Length();
		}

		CWaypointPath cPath(fDistance, iFlags);

		if ( FLAG_SOME_SET(FPathJump | FPathBreak, iFlags) )
		{
			// Jump / break after half seconds, and maintain (duck/shoot) during 1 second.
			cPath.SetActionTime(5);
			cPath.SetActionDuration(10);
		}

		m_cGraph.add_arc(from, to, cPath);
		return true;
	}

	//----------------------------------------------------------------------------------------------------------------
	bool CWaypoints::RemovePath(TWaypointId iFrom, TWaypointId iTo)
	{
		if ( !CWaypoint::IsValid(iFrom) || !CWaypoint::IsValid(iTo) || (iFrom == iTo) || !HasPath(iFrom, iTo) )
			return false;

		m_cGraph.delete_arc(m_cGraph.begin() + iFrom, m_cGraph.begin() + iTo);
		return true;
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoints::CreatePathsWithAutoFlags(
		TWaypointId iWaypoint1,
		TWaypointId iWaypoint2,
		bool bIsCrouched,
		int iMaxDistance,
		bool bShowHelp
	)
	{
		BASSERT(CWaypoints::IsValid(iWaypoint1) && CWaypoints::IsValid(iWaypoint2), return);

		WaypointNode& w1 = m_cGraph[iWaypoint1];
		WaypointNode& w2 = m_cGraph[iWaypoint2];

		if ( FLAG_SOME_SET(FWaypointLadder, w1.vertex.iFlags) || FLAG_SOME_SET(FWaypointLadder, w2.vertex.iFlags) )
		{
			return;
		}

		float fDist = (w2.vertex.vOrigin - w1.vertex.vOrigin).Length();

		Vector v1 = w1.vertex.vOrigin;
		Vector v2 = w2.vertex.vOrigin;
		bool bCrouch = bIsCrouched;

		TReach iReach = CBotrixEngineUtil::GetWaypointReachableInfoFromTo(
			CBotrixEngineUtil::TraceDirection::EFirstToSecond,
			v1,
			v2,
			bCrouch,
			0,
			static_cast<float>(iMaxDistance * iMaxDistance),
			bShowHelp
		);

		if ( iReach != EReachNotReachable )
		{
			TPathFlags iFlags = static_cast<TPathFlags>(
				(iReach == EReachNeedJump)         ? FPathJump
					: (iReach == EReachFallDamage) ? FPathDamage
												   : FPathNone
			);

			if ( bIsCrouched )
			{
				FLAG_SET(FPathCrouch, iFlags);
			}

			AddPath(iWaypoint1, iWaypoint2, fDist, iFlags);
		}

		iReach = CBotrixEngineUtil::GetWaypointReachableInfoFromTo(
			CBotrixEngineUtil::TraceDirection::ESecondToFirst,
			v2,
			v1,
			bCrouch,
			0,
			static_cast<float>(iMaxDistance * iMaxDistance),
			bShowHelp
		);

		if ( iReach != EReachNotReachable )
		{
			TPathFlags iFlags = static_cast<TPathFlags>(
				(iReach == EReachNeedJump)         ? FPathJump
					: (iReach == EReachFallDamage) ? FPathDamage
												   : FPathNone
			);

			if ( bIsCrouched )
			{
				FLAG_SET(FPathCrouch, iFlags);
			}

			AddPath(iWaypoint2, iWaypoint1, fDist, iFlags);
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoints::CreateAutoPaths(TWaypointId id, bool bIsCrouched, float fMaxDistance, bool bShowHelp)
	{
		WaypointNode& w = m_cGraph[id];
		Vector vOrigin = w.vertex.vOrigin;

		int minX, minY, minZ, maxX, maxY, maxZ;
		int x = GetBucketX(vOrigin.x);
		int y = GetBucketY(vOrigin.y);
		int z = GetBucketZ(vOrigin.z);
		GetBuckets(x, y, z, minX, minY, minZ, maxX, maxY, maxZ);

		for ( x = minX; x <= maxX; ++x )
		{
			for ( y = minY; y <= maxY; ++y )
			{
				for ( z = minZ; z <= maxZ; ++z )
				{
					Bucket& bucket = m_cBuckets[x][y][z];

					for ( Bucket::iterator it = bucket.begin(); it != bucket.end(); ++it )
					{
						if ( *it == id || FLAG_SOME_SET(FPathLadder, Get(*it).iFlags) || HasPath(*it, id) ||
							 HasPath(id, *it) )
						{
							continue;
						}

						CreatePathsWithAutoFlags(id, *it, bIsCrouched, static_cast<int>(fMaxDistance), bShowHelp);
					}
				}
			}
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoints::DecrementLocationIds(TWaypointId id)
	{
		BASSERT(CWaypoint::IsValid(id), return);

		// Shift waypoints indexes, all waypoints with index > id.
		for ( int x = 0; x < BUCKETS_SIZE_X; ++x )
		{
			for ( int y = 0; y < BUCKETS_SIZE_Y; ++y )
			{
				for ( int z = 0; z < BUCKETS_SIZE_Z; ++z )
				{
					Bucket& bucket = m_cBuckets[x][y][z];

					for ( Bucket::iterator it = bucket.begin(); it != bucket.end(); ++it )
					{
						if ( *it > id )
						{
							--(*it);
						}
					}
				}
			}
		}

		// Remove waypoint id from bucket.
		Vector vOrigin = m_cGraph[id].vertex.vOrigin;
		Bucket& bucket = m_cBuckets[GetBucketX(vOrigin.x)][GetBucketY(vOrigin.y)][GetBucketZ(vOrigin.z)];
		bucket.erase(find(bucket, id));
	}

	//----------------------------------------------------------------------------------------------------------------
	TWaypointId CWaypoints::GetNearestWaypoint(
		const Vector& vOrigin,
		const good::bitset* aOmit,
		bool bNeedVisible,
		float fMaxDistance,
		TWaypointFlags iFlags
	)
	{
		TWaypointId result = EWaypointIdInvalid;

		float sqDist = SQR(fMaxDistance);
		float sqMinDistance = sqDist;

		int minX, minY, minZ, maxX, maxY, maxZ;
		int x = GetBucketX(vOrigin.x);
		int y = GetBucketY(vOrigin.y);
		int z = GetBucketZ(vOrigin.z);

		GetBuckets(x, y, z, minX, minY, minZ, maxX, maxY, maxZ);

		if ( bNeedVisible )
		{
			CBotrixEngineUtil::SetPVSForVector(vOrigin);
		}

		for ( x = minX; x <= maxX; ++x )
		{
			for ( y = minY; y <= maxY; ++y )
			{
				for ( z = minZ; z <= maxZ; ++z )
				{
					Bucket& bucket = m_cBuckets[x][y][z];

					for ( Bucket::iterator it = bucket.begin(); it != bucket.end(); ++it )
					{
						TWaypointId iWaypoint = *it;

						if ( aOmit && aOmit->test(iWaypoint) )
						{
							continue;
						}

						WaypointNode& node = m_cGraph[iWaypoint];

						if ( FLAG_SOME_SET_OR_0(iFlags, node.vertex.iFlags) )
						{
							float distTo = (node.vertex.vOrigin - vOrigin).LengthSquared();

							if ( (distTo <= sqDist) && (distTo < sqMinDistance) )
							{
								if ( !bNeedVisible ||
									 (CBotrixEngineUtil::IsVisiblePVS(node.vertex.vOrigin) &&
									  CBotrixEngineUtil::IsVisible(
										  vOrigin,
										  node.vertex.vOrigin,
										  EVisibilityWorld,
										  false
									  )) )
								{
									result = iWaypoint;
									sqMinDistance = distTo;
								}
							}
						}
					}
				}
			}
		}

		return result;
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoints::GetNearestWaypoints(
		good::vector<TWaypointId>& aResult,
		const Vector& vOrigin,
		bool bNeedVisible,
		float fMaxDistance
	)
	{
		float fMaxDistSqr = SQR(fMaxDistance);

		int minX, minY, minZ, maxX, maxY, maxZ;
		int x = GetBucketX(vOrigin.x);
		int y = GetBucketY(vOrigin.y);
		int z = GetBucketZ(vOrigin.z);

		GetBuckets(x, y, z, minX, minY, minZ, maxX, maxY, maxZ);

		if ( bNeedVisible )
		{
			CBotrixEngineUtil::SetPVSForVector(vOrigin);
		}

		for ( x = minX; x <= maxX; ++x )
		{
			for ( y = minY; y <= maxY; ++y )
			{
				for ( z = minZ; z <= maxZ; ++z )
				{
					Bucket& bucket = m_cBuckets[x][y][z];

					for ( Bucket::iterator it = bucket.begin(); it != bucket.end(); ++it )
					{
						TWaypointId iWaypoint = *it;
						const WaypointNode& node = m_cGraph[iWaypoint];

						if ( fabs(vOrigin.z - node.vertex.vOrigin.z) <= fMaxDistance )
						{
							float fDistToSqr = (node.vertex.vOrigin.Make2D() - vOrigin.Make2D()).LengthSquared();

							if ( fDistToSqr <= fMaxDistSqr &&
								 (!bNeedVisible ||
								  (CBotrixEngineUtil::IsVisiblePVS(node.vertex.vOrigin) &&
								   CBotrixEngineUtil::IsVisible(
									   vOrigin,
									   node.vertex.vOrigin,
									   EVisibilityWorld,
									   false
								   ))) )
							{
								aResult.push_back(iWaypoint);
							}
						}
					}
				}
			}
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	TWaypointId CWaypoints::GetAnyWaypoint(TWaypointFlags iFlags)
	{
		if ( CWaypoints::Size() == 0 )
			return EWaypointIdInvalid;

		TWaypointId id = rand() % CWaypoints::Size();
		for ( TWaypointId i = id; i >= 0; --i )
			if ( FLAG_SOME_SET_OR_0(iFlags, CWaypoints::Get(i).iFlags) )
				return i;
		for ( TWaypointId i = id + 1; i < CWaypoints::Size(); ++i )
			if ( FLAG_SOME_SET_OR_0(iFlags, CWaypoints::Get(i).iFlags) )
				return i;
		return EWaypointIdInvalid;
	}

	//----------------------------------------------------------------------------------------------------------------
	TWaypointId CWaypoints::GetAimedWaypoint(const Vector& vOrigin, const Vector& ang)
	{
		int x = GetBucketX(vOrigin.x);
		int y = GetBucketY(vOrigin.y);
		int z = GetBucketZ(vOrigin.z);

		// Draw only waypoints from nearest buckets.
		int minX, minY, minZ, maxX, maxY, maxZ;
		GetBuckets(x, y, z, minX, minY, minZ, maxX, maxY, maxZ);

		// Get visible clusters from player's position.
		CBotrixEngineUtil::SetPVSForVector(vOrigin);

		TWaypointId iResult = EWaypointIdInvalid;
		float highestDP = -1.0f;
		float lowestDistanceSq = std::numeric_limits<float>::max();

		// We want to calculate based on looking at the centre of a waypoint.
		const float offsetToCentreOfWaypoint =
			-CBotrixParameterVars::PLAYER_EYE + (CBotrixParameterVars::PLAYER_HEIGHT / 2.0f);

		Vector viewDir;
		AngleVectors(ang, viewDir, nullptr, nullptr);

		for ( x = minX; x <= maxX; ++x )
		{
			for ( y = minY; y <= maxY; ++y )
			{
				for ( z = minZ; z <= maxZ; ++z )
				{
					Bucket& bucket = m_cBuckets[x][y][z];
					for ( Bucket::iterator it = bucket.begin(); it != bucket.end(); ++it )
					{
						WaypointNode& node = m_cGraph[*it];

						// Check if waypoint is in pvs from player's position.
						if ( IsVisibleFromViewer(node.vertex.vOrigin, vOrigin, ang, EVisibilityWorld) )
						{
							Vector vRelative(node.vertex.vOrigin);
							vRelative.z += offsetToCentreOfWaypoint;
							vRelative -= vOrigin;

							float distanceSq = vRelative.LengthSquared();
							float dp = DotProduct(vRelative.Normalize(), viewDir);

							if ( dp > highestDP || (dp == highestDP && distanceSq < lowestDistanceSq) )
							{
								highestDP = dp;
								lowestDistanceSq = distanceSq;
								iResult = *it;
							}
						}
					}
				}
			}
		}

		return iResult;
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoints::Draw(CBotrixClient* pClient)
	{
		float fDrawTime = CWaypoint::DRAW_INTERVAL + (2.0f * gpGlobals->frametime);  // Add two frames to not flicker.

		if ( pClient->iWaypointDrawFlags != FWaypointDrawNone )
		{
			Vector vOrigin;
			vOrigin = pClient->GetHead();
			int x = GetBucketX(vOrigin.x);
			int y = GetBucketY(vOrigin.y);
			int z = GetBucketZ(vOrigin.z);

			Vector viewAngles;
			pClient->GetEyeAngles(viewAngles);

			// Draw only waypoints from nearest buckets.
			int minX, minY, minZ, maxX, maxY, maxZ;
			GetBuckets(x, y, z, minX, minY, minZ, maxX, maxY, maxZ);

			// Get visible clusters from player's position.
			CBotrixEngineUtil::SetPVSForVector(vOrigin);

			for ( x = minX; x <= maxX; ++x )
			{
				for ( y = minY; y <= maxY; ++y )
				{
					for ( z = minZ; z <= maxZ; ++z )
					{
						Bucket& bucket = m_cBuckets[x][y][z];
						for ( Bucket::iterator it = bucket.begin(); it != bucket.end(); ++it )
						{
							WaypointNode& node = m_cGraph[*it];

							// Check if waypoint is in pvs from player's position.
							if ( IsVisibleFromViewer(node.vertex.vOrigin, vOrigin, viewAngles, EVisibilityWorld) )
							{
								node.vertex.Draw(*it, pClient->iWaypointDrawFlags, fDrawTime);
							}
						}
					}
				}
			}

			for ( TAnalyzeWaypoints j = 0; j < EAnalyzeWaypointsTotal; ++j )
			{
				good::vector<Vector>& aPositions = m_aWaypointsToAddOmitInAnalyze[j];

				unsigned char r[3] = {0x00, 0xFF, 0x00};
				unsigned char g[3] = {0xFF, 0x00, 0x00};
				unsigned char b[3] = {0x00, 0x00, 0xFF};

				for ( int i = 0; i < aPositions.size(); ++i )
				{
					Vector posOrigin = aPositions[i];
					posOrigin.x += 0.3f;
					posOrigin.y += 0.3f;
					Vector vEnd = posOrigin;
					vEnd.x += 0.3f;
					vEnd.y += 0.3f;
					vEnd.z -= CBotrixParameterVars::PLAYER_EYE;

					if ( CBotrixEngineUtil::IsVisiblePVS(aPositions[i]) &&
						 CBotrixEngineUtil::IsVisible(posOrigin, aPositions[i], EVisibilityWorld) )
					{
						CBotrixEngineUtil::DrawLine(posOrigin, vEnd, fDrawTime, r[j], g[j], b[j]);
					}
				}
			}
		}

		if ( pClient->iPathDrawFlags != FPathDrawNone )
		{
			// Draw nearest waypoint paths.
			if ( CWaypoint::IsValid(pClient->iCurrentWaypoint) )
			{
				if ( pClient->iPathDrawFlags != FPathDrawNone )
				{
					DrawWaypointPaths(pClient->iCurrentWaypoint, pClient->iPathDrawFlags);
				}

				CWaypoint& w = CWaypoints::Get(pClient->iCurrentWaypoint);
				CBotrixEngineUtil::DrawTextAtLocation(w.vOrigin, 0, fDrawTime, 0xFF, 0xFF, 0xFF, "Current");
			}

			if ( CWaypoint::IsValid(pClient->iDestinationWaypoint) )
			{
				CWaypoint& w = CWaypoints::Get(pClient->iDestinationWaypoint);
				Vector v(w.vOrigin);
				v.z -= 10.0f;
				CBotrixEngineUtil::DrawTextAtLocation(v, 0, fDrawTime, 0xFF, 0xFF, 0xFF, "Destination");

				const float halfPlayerWidth = CBotrixParameterVars::PLAYER_WIDTH / 2.0f;

				CBotrixEngineUtil::DrawBox(
					w.vOrigin - Vector(0.0f, 0.0f, CBotrixParameterVars::PLAYER_EYE),
					Vector(-halfPlayerWidth, -halfPlayerWidth, 0.0f),
					Vector(halfPlayerWidth, halfPlayerWidth, CBotrixParameterVars::PLAYER_HEIGHT),
					fDrawTime,
					0xFF,
					0xFF,
					0xFF
				);
			}
		}

		if ( bValidVisibilityTable && (pClient->iVisiblesDrawFlags != FPathDrawNone) &&
			 CWaypoint::IsValid(pClient->iCurrentWaypoint) )
		{
			DrawVisiblePaths(pClient->iCurrentWaypoint, pClient->iVisiblesDrawFlags);
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoints::MarkUnreachablePath(TWaypointId iWaypointFrom, TWaypointId iWaypointTo)
	{
		if ( CWaypoint::iUnreachablePathFailuresToDelete <= 0 )
			return;

		GoodAssert(IsValid(iWaypointFrom) && IsValid(iWaypointTo) && HasPath(iWaypointFrom, iWaypointTo));

		Vector vFrom = Get(iWaypointFrom).vOrigin;
		Vector vTo = Get(iWaypointTo).vOrigin;

		for ( int i = 0; i < m_aUnreachablePaths.size(); ++i )
		{
			unreachable_path_t& cPath = m_aUnreachablePaths[i];
			if ( cPath.vFrom == vFrom && vTo == vTo )
			{
				if ( ++cPath.iFailedCount >= CWaypoint::iUnreachablePathFailuresToDelete )
				{
					BLOG_I("Removing unreachable path from %d to %d.", iWaypointFrom, iWaypointTo);
					RemovePath(iWaypointFrom, iWaypointTo);
					m_aUnreachablePaths.erase(i);
				}
				return;
			}
		}

		// Add new unreachable path.
		unreachable_path_t cPath;
		cPath.iFailedCount = 1;
		cPath.vFrom = vFrom;
		cPath.vTo = vTo;
		m_aUnreachablePaths.push_back(cPath);
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoints::AddLadderDismounts(const Vector& ladderNormal, TWaypointId iBottom, TWaypointId iTop)
	{
		const float fMaxHeight = CBotrixParameterVars::CalcMaxHeightOfCrouchJump();

		Vector vZ(0, 0, 1);
		Vector vDirection(ladderNormal);
		vDirection.z = 0;
		vDirection = vDirection.Normalize();

		if ( VectorIsNull(vDirection) )
		{
			BULOG_W(m_pAnalyzer, "2D ladder normal was null.");
			return;
		}

		Vector vPerpendicular = CrossProduct(vDirection, vZ);

		Vector vDirections[4];
		vDirections[0] = vDirection * CBotrixParameterVars::PLAYER_WIDTH;
		vDirections[1] = -vDirection * CBotrixParameterVars::PLAYER_WIDTH;
		vDirections[2] = vPerpendicular * CBotrixParameterVars::PLAYER_WIDTH;
		vDirections[3] = -vPerpendicular * CBotrixParameterVars::PLAYER_WIDTH;

		TWaypointId iWaypoints[2] = {iBottom, iTop};

		for ( int i = 0; i < 2; ++i )
		{
			Vector vWaypointEye = Get(iWaypoints[i]).vOrigin;
			Vector vLadderWaypointGround = vWaypointEye;
			vLadderWaypointGround.z -= CBotrixParameterVars::PLAYER_EYE;

			bool bFound = false;
			for ( int j = 0; j < 4; ++j )
			{
				Vector vNew = vWaypointEye + vDirections[j];
				Vector vCandidate = CBotrixEngineUtil::GetGroundVec(vNew);  // Not hull ground.
				vCandidate.z += CBotrixParameterVars::PLAYER_EYE;

				if ( vWaypointEye.z - vCandidate.z > fMaxHeight )  // Too high.
				{
					continue;
				}

				if ( !CBotrixEngineUtil::IsVisible(vWaypointEye, vCandidate, EVisibilityWaypoints, false) )
				{
					continue;
				}

				Vector vCandidateGround =
					CBotrixEngineUtil::GetHumanHullGroundVec(vNew, CBotrixEngineUtil::PositionInHull::Eye);

				CBotrixEngineUtil::HumanHullTrace(vLadderWaypointGround, vCandidateGround);

				if ( CBotrixEngineUtil::TraceHitSomething() )
				{
					continue;
				}

				vCandidate = vCandidateGround;
				vCandidate.z += CBotrixParameterVars::PLAYER_EYE;

				TWaypointId iDismount =
					CWaypoints::GetNearestWaypoint(vCandidate, NULL, false, CBotrixParameterVars::PLAYER_WIDTH / 2.0f);

				if ( iDismount != EWaypointIdInvalid )
				{
					BULOG_D(
						m_pAnalyzer,
						"Chose waypoint %d for %s ladder dismount",
						iDismount,
						i == 0 ? "bottom" : "top"
					);
				}
				else
				{
					iDismount = Add(vCandidate);
					BULOG_D(
						m_pAnalyzer,
						"  added waypoint %d (%s ladder dismount) at (%.0f, %.0f, %.0f)",
						iDismount,
						i == 0 ? "bottom" : "top",
						vWaypointEye.x,
						vWaypointEye.y,
						vWaypointEye.z
					);
				}

				AddPath(iWaypoints[i], iDismount, 0.0f, FPathLadder | FPathJump);
				AddPath(iDismount, iWaypoints[i], 0.0f, FPathLadder);

				bFound = true;
			}

			if ( !bFound )
			{
				BULOG_W(
					m_pAnalyzer,
					"Ladder waypoint %d, couldn't create %s dismount waypoints.",
					iWaypoints[i],
					i == 0 ? "bottom" : "top"
				);
			}
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoints::Analyze(edict_t* pClient, bool bShowLines)
	{
		m_pAnalyzer = pClient;
		CWaypoint::bShowAnalyzePotentialWaypoints = bShowLines;

		m_iAnalyzeStep = EAnalyzeStepNeighbours;
		m_iCurrentAnalyzeWaypoint = 0;
		m_fAnalyzeWaypointsForNextFrame = 0;

		BULOG_W(pClient, "Started analyzing waypoints.");

		static const TItemType aItemTypes[] = {
			EItemTypePlayerSpawn,
			EItemTypeHealth,
			EItemTypeArmor,
			EItemTypeWeapon,
			EItemTypeAmmo,
		};

		const float fPlayerHalfWidth = CBotrixParameterVars::PLAYER_WIDTH / 2.0f;

		float fAnalyzeDistance = static_cast<float>(CWaypoint::iAnalyzeDistance);
		float fAnalyzeDistanceExtra = fAnalyzeDistance * 1.9f;  // To include diagonal, almost but not 2 (Pythagoras).

		BULOG_I(pClient, "Adding waypoints at spawn / items / ladder positions.");

		for ( int iType = 0; iType < SIZE_OF_ARRAY_AS_INT(aItemTypes); ++iType )
		{
			TItemType iItemType = aItemTypes[iType];

			const good::vector<CItem>& items = CItems::GetItems(iItemType);

			BULOG_I(
				pClient,
				"  - %d items of type %s",
				items.size(),
				CTypeToString::EntityTypeToString(iItemType).c_str()
			);

			for ( TItemIndex i = 0; i < items.size(); ++i )
			{
				if ( items[i].IsFree() || items[i].IsTaken() )
				{
					continue;
				}

				bool bUse = FLAG_SOME_SET(FItemUse, items[i].pItemClass->iFlags);
				TWaypointFlags iFlags = CWaypoint::GetFlagsFor(iItemType);
				int iArgument = 0;
				Vector vOrigin;
				Vector vPos;

				if ( bUse )
				{
					FLAG_CLEAR(FWaypointHealth | FWaypointArmor, iFlags);  // Leave only chargers, not items.

					edict_t* edict = items[i].pEdict;
					Vector vMins = Vector(edict->v.origin) + Vector(edict->v.mins);
					Vector vMaxs = Vector(edict->v.origin) + Vector(edict->v.maxs);
					Vector vMid = (vMins + vMaxs) / 2.0f;
					vOrigin = (vMaxs - vMins) / 2.0f;
					float fItemHalfWidth = MAX2(vOrigin.x, vOrigin.y);

					Vector angles(edict->v.angles);

					Vector vDirection;
					AngleVectors(angles, vDirection, nullptr, nullptr);
					vDirection *= fItemHalfWidth + fPlayerHalfWidth + 5.0f;

					vPos = vMid + vDirection;
					vPos = CBotrixEngineUtil::GetHumanHullGroundVec(vPos, CBotrixEngineUtil::PositionInHull::Eye);
					vPos.z += CBotrixParameterVars::PLAYER_EYE;

					if ( fabsf(vPos.z - vMid.z) > CBotrixParameterVars::PLAYER_HEIGHT )
					{
						// Too high or need to fall to grab, probably needs to grab with gravity gun.
						continue;
					}

					VectorAngles(vMid - vPos, angles);
					CBotrixEngineUtil::DeNormalizeAngle(angles.x);
					CBotrixEngineUtil::DeNormalizeAngle(angles.y);

					CWaypoint::SetFirstAngle(static_cast<int>(angles.x), static_cast<int>(angles.y), iArgument);
				}
				else
				{
					FLAG_CLEAR(
						FWaypointHealthMachine | FWaypointArmorMachine,
						iFlags
					);  // Leave only items, not chargers.

					vOrigin = items[i].CurrentPosition();
					vPos = vOrigin;
					vPos.z += CBotrixParameterVars::PLAYER_EYE;

					Vector vGround =
						CBotrixEngineUtil::GetHumanHullGroundVec(vPos, CBotrixEngineUtil::PositionInHull::Eye);
					vGround.z += CBotrixParameterVars::PLAYER_EYE;

					if ( fabs(vPos.z - vGround.z) > CBotrixParameterVars::PLAYER_HEIGHT )
					{
						// Too high or need to fall to grab, probably needs to grab with gravity gun.
						continue;
					}

					vPos = vGround;
				}

				TWaypointId iWaypoint = CWaypoints::GetNearestWaypoint(vPos, NULL, false, fPlayerHalfWidth);

				if ( iWaypoint == EWaypointIdInvalid )
				{
					iWaypoint = Add(vPos, iFlags, iArgument);
					BULOG_D(
						m_pAnalyzer,
						"  added waypoint %d (%s %d at (%.0f, %.0f, %.0f)) at (%.0f, %.0f, %.0f)",
						iWaypoint,
						items[i].pItemClass->sClassName.c_str(),
						i,
						vOrigin.x,
						vOrigin.y,
						vOrigin.z,
						vPos.x,
						vPos.y,
						vPos.z
					);
				}

				CreateAutoPaths(iWaypoint, false, fAnalyzeDistanceExtra, false);
			}
		}

		// Add ladder waypoints.
		const good::vector<CItem>& items = CItems::GetItems(EItemTypeLadder);
		for ( TItemIndex i = 0; i < items.size(); ++i )
		{
			if ( items[i].IsFree() )
			{
				continue;
			}

			const CItem& item = items[i];
			Vector vMins(item.pEdict->v.mins);
			Vector vMaxs(item.pEdict->v.maxs);
			Vector centre = vMins + ((vMaxs - vMins) / 2.0f);

			const char* ladderName = STRING(item.pEdict->v.targetname);

			if ( !ladderName || !(*ladderName) )
			{
				BULOG_W(
					m_pAnalyzer,
					"Cannot create ladder dismount waypoints for ladder at (%f %f %f) with no targetname!",
					centre[0],
					centre[1],
					centre[2]
				);

				continue;
			}

			CUtlVector<edict_t*> dismountPoints;
			FindLadderDismounts(ladderName, dismountPoints);

			if ( dismountPoints.Count() != 2 )
			{
				BULOG_W(
					m_pAnalyzer,
					"Expected 2 info_ladder_dismount entities for ladder %s, but found %d",
					ladderName,
					dismountPoints.Count()
				);

				continue;
			}

			Vector estimatedNormal = Vector(dismountPoints[0]->v.origin) - centre;
			estimatedNormal.z = 0.0f;
			estimatedNormal = estimatedNormal.Normalize();

			Vector estimatedAngles;
			VectorAngles(estimatedNormal, estimatedAngles);
			NormalizeAngles(estimatedAngles);

			if ( estimatedAngles.y < 0.0f )
			{
				estimatedAngles.y += 360.0f;
			}

			// Snap normal to one of 12 different possibilities.
			int outOfTwelve = static_cast<int>((estimatedAngles.y / 360.0f) * 12.0f) % 12;
			estimatedAngles.y = static_cast<float>(outOfTwelve) * (360.0f / 12.0f);
			AngleVectors(Vector(0.0f, estimatedAngles.y, 0.0f), estimatedNormal, nullptr, nullptr);

			// Currently, we assume that the lower dismount position is where the player
			// hull will start from when climbing the ladder.
			Vector ladderBottom(dismountPoints[0]->v.origin);
			Vector ladderTop = ladderBottom;
			ladderTop.z = dismountPoints[1]->v.origin[VEC3_Z];

			ladderBottom.z += CBotrixParameterVars::PLAYER_EYE;
			ladderBottom =
				CBotrixEngineUtil::GetHumanHullGroundVec(ladderBottom, CBotrixEngineUtil::PositionInHull::Eye);
			ladderBottom.z += CBotrixParameterVars::PLAYER_EYE + 2.0f;

			ladderTop.z += CBotrixParameterVars::PLAYER_EYE + 2.0f;
			ladderTop = ladderTop - (fPlayerHalfWidth * estimatedNormal);

			TWaypointId w1 = CWaypoints::GetNearestWaypoint(ladderBottom, NULL, false, fPlayerHalfWidth);

			if ( w1 == EWaypointIdInvalid )
			{
				w1 = Add(ladderBottom, FWaypointLadder);

				BULOG_D(
					m_pAnalyzer,
					"  added waypoint %d (bottom ladder %d) at (%.0f, %.0f, %.0f)",
					w1,
					i,
					ladderBottom.x,
					ladderBottom.y,
					ladderBottom.z
				);
			}
			else
			{
				FLAG_SET(FWaypointLadder, Get(w1).iFlags);
				BULOG_D(m_pAnalyzer, "Set ladder attribute for bottom ladder waypoint %d", w1);
			}

			TWaypointId w2 = CWaypoints::GetNearestWaypoint(ladderTop, NULL, false, fPlayerHalfWidth);

			if ( w2 == EWaypointIdInvalid )
			{
				w2 = Add(ladderTop, FWaypointLadder);

				BULOG_D(
					m_pAnalyzer,
					"  added waypoint %d (top ladder %d) at (%.0f, %.0f, %.0f)",
					w2,
					i,
					ladderTop.x,
					ladderTop.y,
					ladderTop.z
				);
			}
			else
			{
				FLAG_SET(FWaypointLadder, Get(w2).iFlags);
				BULOG_D(m_pAnalyzer, "Set ladder attribute for top ladder waypoint %d", w2);
			}

			float fDist = (ladderTop - ladderBottom).Length();

			if ( HasPath(w1, w2) )
			{
				FLAG_SET(FPathLadder, GetPath(w1, w2)->iFlags);
				BULOG_D(m_pAnalyzer, "Marked existing path %d -> %d as ladder", w1, w2);
			}
			else
			{
				AddPath(w1, w2, fDist, FPathLadder);
				BULOG_D(m_pAnalyzer, "Added %d -> %d as ladder path", w1, w2);
			}

			if ( HasPath(w2, w1) )
			{
				FLAG_SET(FPathLadder, GetPath(w2, w1)->iFlags);
				BULOG_D(m_pAnalyzer, "Marked existing path %d -> %d as ladder", w2, w1);
			}
			else
			{
				AddPath(w2, w1, fDist, FPathLadder);
				BULOG_D(m_pAnalyzer, "Added %d -> %d as ladder path", w2, w1);
			}

			CreateAutoPaths(w1, false, fAnalyzeDistanceExtra, false);
			CreateAutoPaths(w2, false, fAnalyzeDistanceExtra, false);

			AddLadderDismounts(estimatedNormal, w1, w2);
		}

		BULOG_I(pClient, "Adding waypoints at added positions ('botrix waypoint analyze add').");
		good::vector<Vector>& aAdded = m_aWaypointsToAddOmitInAnalyze[EAnalyzeWaypointsAdd];
		for ( int i = 0; i < aAdded.size(); ++i )
		{
			Vector& vPos = aAdded[i];

			TWaypointId iWaypoint = CWaypoints::GetNearestWaypoint(vPos, NULL, false, fPlayerHalfWidth);

			if ( iWaypoint == EWaypointIdInvalid )
			{
				iWaypoint = Add(vPos);
				BULOG_D(m_pAnalyzer, "  added waypoint %d at (%.0f, %.0f, %.0f)", iWaypoint, vPos.x, vPos.y, vPos.z);
			}

			CreateAutoPaths(iWaypoint, false, fAnalyzeDistanceExtra, false);
		}

		if ( Size() < 1 )
		{
			StopAnalyzing();
			BULOG_W(pClient, "No waypoints to analyze (no player spawn entities on the map?).");
			BULOG_W(pClient, "Please add some waypoints manually and run the command again.");
			BULOG_W(pClient, "Stopped analyzing waypoints.");
			return;
		}

		BULOG_I(m_pAnalyzer, "Adding new waypoints.");
		m_aWaypointsNeighbours.resize(2048);
	}

	void CWaypoints::StopAnalyzing()
	{
		m_aWaypointsNeighbours = good::vector<CNeighbour>();
		m_iAnalyzeStep = EAnalyzeStepTotal;
		m_pAnalyzer = NULL;
	}

	void CWaypoints::AnalyzeStep()
	{
		GoodAssert(m_iAnalyzeStep < EAnalyzeStepTotal);
		static int iOldSize;

		// Search for analyzer in players, else remove him. Can't rely on CPlayers::GetIndex() because the server is
		// reusing edicts.
		if ( m_pAnalyzer )
		{
			bool bFound = false;
			for ( TPlayerIndex iPlayer = 0; iPlayer < CPlayers::Size(); ++iPlayer )
			{
				if ( CPlayers::Get(iPlayer)->GetEdict() == m_pAnalyzer )
				{
					bFound = true;
					break;
				}
			}

			if ( !bFound )
			{
				m_pAnalyzer = NULL;
			}
		}

		// Check more waypoints in inters step, as less traces are required.
		float fToAnalyze = m_fAnalyzeWaypointsForNextFrame;
		fToAnalyze += CWaypoint::fAnalyzeWaypointsPerFrame * (1 + m_iAnalyzeStep * 4);
		int iToAnalyze = (int)fToAnalyze;
		m_fAnalyzeWaypointsForNextFrame = fToAnalyze - iToAnalyze;

		if ( m_iAnalyzeStep < EAnalyzeStepDeleteOrphans )
		{
			float fAnalyzeDistance = static_cast<float>(CWaypoint::iAnalyzeDistance);
			float fAnalyzeDistanceExtra =
				fAnalyzeDistance * 1.9f;  // To include diagonal, almost but not 2 (Pythagoras).
			float fAnalyzeDistanceExtraSqr = fAnalyzeDistanceExtra * 1.9f;
			fAnalyzeDistanceExtraSqr *= fAnalyzeDistanceExtraSqr;

			for ( int i = 0; i < iToAnalyze && m_iCurrentAnalyzeWaypoint < Size(); ++i, ++m_iCurrentAnalyzeWaypoint )
			{
				TWaypointId iWaypoint = m_iCurrentAnalyzeWaypoint;
				CWaypoint& cWaypoint = Get(iWaypoint);

				if ( FLAG_SOME_SET(FWaypointLadder, cWaypoint.iFlags) )
				{
					continue;  // Don't analyze ladder waypoints here, this will be done in the last step.
				}

				Vector vPos = cWaypoint.vOrigin;
				if ( good::find(m_aWaypointsToAddOmitInAnalyze[EAnalyzeWaypointsOmit], vPos) !=
					 m_aWaypointsToAddOmitInAnalyze[EAnalyzeWaypointsOmit].end() )
				{
					continue;
				}

				CNeighbour neighbours = m_aWaypointsNeighbours[iWaypoint];
				for ( int x = -1; x <= 1; ++x )
				{
					for ( int y = -1; y <= 1; ++y )
					{
						if ( (x == 0 && y == 0) || neighbours.a[x + 1][y + 1] )
						{
							continue;
						}

						Vector vNew = vPos;

						if ( m_iAnalyzeStep == EAnalyzeStepNeighbours )
						{
							// First check if there is a waypoint near final position.
							vNew.x += fAnalyzeDistance * x;
							vNew.y += fAnalyzeDistance * y;

							if ( AnalyzeWaypoint(
									 iWaypoint,
									 vPos,
									 vNew,
									 fAnalyzeDistance,
									 fAnalyzeDistanceExtra,
									 fAnalyzeDistanceExtraSqr
								 ) )
								neighbours.a[x + 1][y + 1] =
									true;  // Position is already occupied or new waypoint is added.
						}
						else  // m_iAnalyzeStep == EAnalyzeStepInters
						{
							if ( x == 1 && y == 1 )  // Omit (1, 1), as there are no adjacent points up / right to it.
							{
								continue;
							}

							if ( m_aWaypointsNeighbours[iWaypoint].a[x + 1][y + 1] )  // Convert [-1..1] to [0..2].
							{
								continue;  // Don't use neighbours here, as it can be updated.
							}

							int incX = x + 1;  // Adjacent point on X.

							if ( incX <= 1 && !(incX == 0 && y == 0) &&  // Omit (0, 0) and (2, y).
								 !m_aWaypointsNeighbours[iWaypoint].a[incX + 1][y + 1] )
							{
								vNew.x += fAnalyzeDistance * (x + incX / 2.0f);  // Will be -1/2 or 1/2.
								vNew.y += fAnalyzeDistance * y;

								if ( AnalyzeWaypoint(
										 iWaypoint,
										 vPos,
										 vNew,
										 fAnalyzeDistance,
										 fAnalyzeDistanceExtra,
										 fAnalyzeDistanceExtraSqr
									 ) )
									neighbours.a[x + 1][y + 1] = true;
							}

							int incY = y + 1;  // Adjacent point on Y.
							if ( incY <= 1 && !(x == 0 && incY == 0) &&  // Omit (0, 0) and (x, 2).
								 !m_aWaypointsNeighbours[iWaypoint].a[x + 1][incY + 1] )
							{
								vNew = vPos;
								vNew.x += fAnalyzeDistance * x;
								vNew.y += fAnalyzeDistance * (y + incY / 2.0f);  // Will be -1/2 or 1/2.

								if ( AnalyzeWaypoint(
										 iWaypoint,
										 vPos,
										 vNew,
										 fAnalyzeDistance,
										 fAnalyzeDistanceExtra,
										 fAnalyzeDistanceExtraSqr
									 ) )
								{
									neighbours.a[x + 1][y + 1] = true;
								}
							}
						}
					}
				}  // for x.. y..

				m_aWaypointsNeighbours[iWaypoint] = neighbours;
			}
		}
		else
		{
			// Remove waypoints without paths.
			good::vector<Vector>& aToOmit = m_aWaypointsToAddOmitInAnalyze[EAnalyzeWaypointsOmit];
			TWaypointId i = m_iCurrentAnalyzeWaypoint;

			for ( ; iToAnalyze > 0 && i >= 0; --i, --iToAnalyze )
			{
				if ( m_cGraph[i].neighbours.size() == 0 ||
					 good::find(aToOmit, m_cGraph[i].vertex.vOrigin) != aToOmit.end() )
				{
					BULOG_D(m_pAnalyzer, "  removing waypoint %d.", i);
					Remove(i--);
					break;  // Don't remove more that 1 waypoint per frame, will hang up for couple of seconds.
				}
			}

			m_iCurrentAnalyzeWaypoint = i;
		}

		if ( m_iCurrentAnalyzeWaypoint < 0 || m_iCurrentAnalyzeWaypoint >= Size() )  // Got out of bounds.
		{
			switch ( m_iAnalyzeStep )
			{
				case EAnalyzeStepNeighbours:
				{
					BULOG_I(m_pAnalyzer, "Checking for missing spots.");
					m_iCurrentAnalyzeWaypoint = 0;
					m_bIsAnalyzeStepAddedWaypoints = false;
					++m_iAnalyzeStep;
					break;
				}

				case EAnalyzeStepInters:
				{
					if ( m_bIsAnalyzeStepAddedWaypoints )
					{
						--m_iAnalyzeStep;
						BULOG_I(m_pAnalyzer, "Checking new waypoints from missing spots.");
						m_iCurrentAnalyzeWaypoint = 0;
					}
					else
					{
						++m_iAnalyzeStep;
						iOldSize = Size();  // Save size before erasing waypoints.
						BULOG_I(m_pAnalyzer, "Erasing orphans / omitted waypoints.");
						m_iCurrentAnalyzeWaypoint = Size() - 1;
					}

					break;
				}

				case EAnalyzeStepDeleteOrphans:
				{
					BULOG_I(m_pAnalyzer, "Removed %d orphan / omitted waypoints.", iOldSize - Size());

					StopAnalyzing();  // Stop analyzing, no more waypoints.
					BULOG_I(m_pAnalyzer, "Total waypoints: %d.", Size());
					BULOG_W(m_pAnalyzer, "Stopped analyzing waypoints.");
					break;
				}

				default:
				{
					GoodAssert(false);
					break;
				}
			}
		}
	}

	bool CWaypoints::AnalyzeWaypoint(
		TWaypointId iWaypoint,
		Vector& vPos,
		Vector& vNew,
		float fAnalyzeDistance,
		float fAnalyzeDistanceExtra,
		float fAnalyzeDistanceExtraSqr
	)
	{
		static constexpr float HALF_PLAYER_WIDTH = CBotrixParameterVars::PLAYER_WIDTH / 2.0f;
		static constexpr float HALF_PLAYER_WIDTH_SQ = SQR(HALF_PLAYER_WIDTH);

		static good::vector<TWaypointId> aNearWaypoints(16);

		const int contents = g_engfuncs.pfnPointContents(vNew);

		if ( contents == CONTENTS_SOLID || contents == CONTENTS_CLIP || contents == CONTENTS_LADDER )
		{
			return false;  // Ignore, if inside some solid brush.
		}

		const float fJumpHeight = CBotrixParameterVars::CalcMaxHeightOfCrouchJump();

		Vector vGround =
			CBotrixEngineUtil::GetHumanHullGroundVec(vNew, CBotrixEngineUtil::PositionInHull::Eye, nullptr, 4.0f);

		// Check if on a border.
		Vector vHullGround = vGround;
		Vector vFineGround = CBotrixEngineUtil::GetGroundVec(vNew);

		if ( vHullGround.z - vFineGround.z > fJumpHeight )
		{
			// On a border (line trace hits lower ground), try to get off from there.
			static float directions[4][3] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}};

			for ( int i = 0; i < 4; ++i )
			{
				Vector vDirection = Vector(directions[i][0], directions[i][1], directions[i][2]);
				vDirection *= HALF_PLAYER_WIDTH;

				Vector vDisplaced = vNew + vDirection;
				vHullGround =
					CBotrixEngineUtil::GetHumanHullGroundVec(vDisplaced, CBotrixEngineUtil::PositionInHull::Eye);

				if ( fabs(vHullGround.z - vGround.z) < CBotrixParameterVars::GetStepSize() )  // Small difference.
				{
					vFineGround = CBotrixEngineUtil::GetGroundVec(vDisplaced);

					if ( fabs(vHullGround.z - vFineGround.z) <
						 CBotrixParameterVars::GetStepSize() )  // Small difference.
					{
						vGround = vHullGround;
						break;
					}
				}
			}
		}

		Vector vNewEyePos = vGround;
		vNewEyePos.z += CBotrixParameterVars::PLAYER_EYE;

		if ( CWaypoint::bShowAnalyzePotentialWaypoints )
		{
			Vector v = vNewEyePos;
			v.z -= CBotrixParameterVars::PLAYER_EYE;
			CBotrixEngineUtil::DrawLine(vNewEyePos, v, 10, 255, 255, 255);
		}

		aNearWaypoints.clear();
		CWaypoints::GetNearestWaypoints(aNearWaypoints, vNewEyePos, true, fAnalyzeDistance / 1.41f);

		bool bSkip = false;

		const good::vector<Vector>& aPositions = m_aWaypointsToAddOmitInAnalyze[EAnalyzeWaypointsDebug];
		bool showHelp =
			(CWaypoint::fAnalyzeWaypointsPerFrame < ANALIZE_HELP_IF_LESS_WAYPOINTS_PER_FRAME ||
			 good::find(aPositions, vNew) != aPositions.end() || good::find(aPositions, vPos) != aPositions.end());

		for ( int w = 0; !bSkip && w < aNearWaypoints.size(); ++w )
		{
			int iNear = aNearWaypoints[w];
			if ( iNear != iWaypoint && !HasPath(iWaypoint, iNear) && !HasPath(iNear, iWaypoint) )
			{
				CreatePathsWithAutoFlags(iWaypoint, iNear, false, static_cast<int>(fAnalyzeDistanceExtra), showHelp);
			}

			// If has path, set bSkip to true.
			if ( HasPath(iWaypoint, iNear) )
			{
				bSkip = true;
			}

			// If path is not adding somehow, but the waypoint is really close (half player's width or closer).
			if ( (vNewEyePos.Make2D() - Get(iNear).vOrigin.Make2D()).LengthSquared() <= HALF_PLAYER_WIDTH_SQ )
			{
				bSkip = true;
			}
		}

		if ( bSkip )
		{
			return true;  // No waypoint is added, but nearby paths are created.
		}

		bool bCrouch = false;
		TReach reach = CBotrixEngineUtil::GetWaypointReachableInfoFromTo(
			CBotrixEngineUtil::TraceDirection::EFirstToSecond,
			vPos,
			vNewEyePos,
			bCrouch,
			0.0f,
			fAnalyzeDistanceExtraSqr,
			showHelp
		);

		const bool shouldAdd = reach != EReachFallDamage && reach != EReachNotReachable;
		TWaypointId iNew = -1;

		if ( shouldAdd )
		{
			// The waypoint may have been adjusted, so check if there's a better one nearby
			iNew = CWaypoints::GetNearestWaypoint(vNewEyePos, NULL, false, HALF_PLAYER_WIDTH);
		}

		if ( shouldAdd )
		{
			if ( !CWaypoints::IsValid(iNew) )
			{
				iNew = Add(vNewEyePos);
				BULOG_D(
					m_pAnalyzer,
					"  added waypoint %d (from %d) at (%.0f, %.0f, %.0f)",
					iNew,
					iWaypoint,
					vNewEyePos.x,
					vNewEyePos.y,
					vNewEyePos.z
				);
			}

			m_bIsAnalyzeStepAddedWaypoints = true;

			TPathFlags iFlags = static_cast<TPathFlags>(bCrouch ? FPathCrouch : FPathNone);

			AddPath(
				iWaypoint,
				iNew,
				0,
				static_cast<TPathFlags>(iFlags | (reach == EReachNeedJump ? FPathJump : FPathNone))
			);

			bool bDestCrouch = false;
			reach = CBotrixEngineUtil::GetWaypointReachableInfoFromTo(
				CBotrixEngineUtil::TraceDirection::ESecondToFirst,
				vNewEyePos,
				vPos,
				bDestCrouch,
				0,
				fAnalyzeDistanceExtraSqr,
				showHelp
			);

			if ( reach != EReachFallDamage && reach != EReachNotReachable )
			{
				iFlags = static_cast<TPathFlags>(bDestCrouch ? FPathCrouch : FPathNone);

				AddPath(
					iNew,
					iWaypoint,
					0,
					static_cast<TPathFlags>(iFlags | (reach == EReachNeedJump ? FPathJump : FPathNone))
				);
			}

			CreateAutoPaths(iNew, bCrouch, fAnalyzeDistanceExtra, showHelp);

			m_aWaypointsNeighbours.resize(Size());
			return true;  // New waypoint is added.
		}

		return false;  // No waypoint is added.
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoints::GetPathColor(TPathFlags iFlags, unsigned char& r, unsigned char& g, unsigned char& b)
	{
		if ( FLAG_SOME_SET(FPathDemo, iFlags) )
		{
			r = 0xFF;
			g = 0x00;
			b = 0xFF;  // Magenta effect, demo.
		}
		else if ( FLAG_SOME_SET(FPathBreak, iFlags) )
		{
			r = 0xFF;
			g = 0x00;
			b = 0x00;  // Red effect, break.
		}
		else if ( FLAG_SOME_SET(FPathSprint, iFlags) )
		{
			r = 0xFF;
			g = 0xFF;
			b = 0x00;  // Yellow effect, sprint.
		}
		else if ( FLAG_SOME_SET(FPathStop, iFlags) )
		{
			r = 0x66;
			g = 0x66;
			b = 0x00;  // Dark yellow effect, stop.
		}
		else if ( FLAG_SOME_SET(FPathLadder, iFlags) )
		{
			r = 0xFF;
			g = 0x80;
			b = 0x00;  // Orange effect, ladder.
		}
		else if ( FLAG_ALL_SET_OR_0(FPathJump | FPathCrouch, iFlags) )
		{
			r = 0x00;
			g = 0x00;
			b = 0x66;  // Dark blue effect, jump + crouch.
		}
		else if ( FLAG_SOME_SET(FPathJump, iFlags) )
		{
			r = 0x00;
			g = 0x00;
			b = 0xFF;  // Light blue effect, jump.
		}
		else if ( FLAG_SOME_SET(FPathCrouch, iFlags) )
		{
			r = 0x00;
			g = 0xFF;
			b = 0x00;  // Green effect, crouch.
		}
		else if ( FLAG_SOME_SET(FPathDoor | FPathElevator, iFlags) )
		{
			r = 0x8A;
			g = 0x2B;
			b = 0xE2;  // Violet effect, door.
		}
		else if ( FLAG_SOME_SET(FPathTotem, iFlags) )
		{
			r = 0x96;
			g = 0x48;
			b = 0x00;  // Brown effect, totem.
		}
		else
		{
			r = 0x00;
			g = 0xBB;
			b = 0xFF;  // Cyan effect, other flags.
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoints::DrawWaypointPaths(TWaypointId id, TPathDrawFlags iPathDrawFlags)
	{
		BASSERT(iPathDrawFlags != FPathDrawNone, return);

		WaypointNode& w = m_cGraph[id];

		unsigned char r, g, b;
		Vector diff(0, 0, -CBotrixParameterVars::PLAYER_EYE / 4.0f);
		float fDrawTime = CWaypoint::DRAW_INTERVAL + (2.0f * gpGlobals->frametime);  // Add two frames to not flick.

		for ( WaypointArcIt it = w.neighbours.begin(); it != w.neighbours.end(); ++it )
		{
			WaypointNode& n = m_cGraph[it->target];
			GetPathColor(it->edge.iFlags, r, g, b);

			if ( FLAG_SOME_SET(FPathDrawBeam, iPathDrawFlags) )
				CBotrixEngineUtil::DrawBeam(
					w.vertex.vOrigin + diff,
					n.vertex.vOrigin + diff,
					CWaypoint::PATH_WIDTH,
					fDrawTime,
					r,
					g,
					b
				);

			if ( FLAG_SOME_SET(FPathDrawLine, iPathDrawFlags) )
			{
				CBotrixEngineUtil::DrawLine(w.vertex.vOrigin + diff, n.vertex.vOrigin + diff, fDrawTime, r, g, b);
			}
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	void CWaypoints::DrawVisiblePaths(TWaypointId id, TPathDrawFlags iPathDrawFlags)
	{
		GoodAssert(bValidVisibilityTable && (iPathDrawFlags != FPathDrawNone));

		Vector vOrigin(Get(id).vOrigin);
		Vector diff(0, 0, -CBotrixParameterVars::PLAYER_EYE / 2.0f);

		const unsigned char r = 0xFF, g = 0xFF, b = 0xFF;
		float fDrawTime = CWaypoint::DRAW_INTERVAL + (2.0f * gpGlobals->frametime);  // Add two frames to not flick.

		for ( int i = 0; i < Size(); ++i )
		{
			if ( (i != id) && m_aVisTable[id].test(i) )
			{
				const CWaypoint& cWaypoint = Get(i);
				if ( FLAG_SOME_SET(FPathDrawBeam, iPathDrawFlags) )
				{
					CBotrixEngineUtil::DrawBeam(
						vOrigin + diff,
						cWaypoint.vOrigin + diff,
						CWaypoint::PATH_WIDTH,
						fDrawTime,
						r,
						g,
						b
					);
				}

				if ( FLAG_SOME_SET(FPathDrawLine, iPathDrawFlags) )
				{
					CBotrixEngineUtil::DrawLine(vOrigin + diff, cWaypoint.vOrigin + diff, fDrawTime, r, g, b);
				}
			}
		}
	}
}  // namespace Botrix
