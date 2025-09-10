#include "botrix/engine_util.h"
#include <limits>
#include "EnginePublicAPI/cvardef.h"
#include "EnginePublicAPI/eiface.h"
#include "botrix/botrixmod.h"
#include "botrix/waypoint.h"
#include "botrix/botrixgamerulesinterface.h"
#include "botrix/defines.h"
#include "enginecallback.h"
#include "standard_includes.h"
#include "gamerules.h"
#include "MathLib/utils.h"
#include "types.h"
#include "util.h"
#include "weapons.h"
#include "customGeometry/messageWriter.h"
#include "customGeometry/geometryItem.h"
#include "customGeometry/constructors/aabboxConstructor.h"
#include "customGeometry/rollingLineMessageWriter.h"
#include "customGeometry/primitiveMessageWriter.h"

good::TLogLevel CBotrixEngineUtil::iLogLevel = good::ELogLevelInfo;
int CBotrixEngineUtil::iTextTime = 20;  // Time in seconds to show text in CUtil::GetWaypointReachableInfoFromTo().
TraceResult CBotrixEngineUtil::m_TraceResult = {
	0,
	0,
	0,
	0,
	-1.0f,  // Fraction - used to signify here that the result has not yet been used
	{0.0f, 0.0f, 0.0f},
	0.0f,
	{0.0f, 0.0f, 0.0f},
	nullptr,
	0,
};
uint8_t CBotrixEngineUtil::pvs[MAX_PVS_DATA_LENGTH];

void CBotrixEngineUtil::Message(good::TLogLevel iLevel, struct edict_s* pEntity, const char* szMsg)
{
	bool shouldSendClientMessage = false;

	if ( pEntity )
	{
		if ( IS_DEDICATED_SERVER() )
		{
			shouldSendClientMessage = true;
		}
		else
		{
			edict_t* listenServerPlayer = nullptr;
			CHalfLifeMultiplay* mpGamerules = dynamic_cast<CHalfLifeMultiplay*>(g_pGameRules);
			CBotrixGameRulesInterface* bgri =
				mpGamerules ? dynamic_cast<CBotrixGameRulesInterface*>(mpGamerules->BotGameRulesInterface()) : nullptr;

			if ( bgri )
			{
				listenServerPlayer = bgri->GetListenServerClient();
			}

			shouldSendClientMessage = listenServerPlayer != pEntity;
		}
	}

	if ( shouldSendClientMessage )
	{
		CLIENT_PRINTF(pEntity, print_console, szMsg);
	}

	if ( iLevel >= iLogLevel )
	{
		ALERT_TYPE alertType = at_console;

		switch ( iLevel )
		{
			case good::ELogLevelTrace:
			case good::ELogLevelDebug:
			{
				alertType = at_aiconsole;
				break;
			}

			case good::ELogLevelInfo:
			{
				alertType = at_console;
				break;
			}

			case good::ELogLevelWarning:
			{
				alertType = at_warning;
				break;
			}

			case good::ELogLevelError:
			{
				alertType = at_error;
				break;
			}

			default:
			{
				break;
			}
		}

		ALERT(alertType, szMsg);
	}
}

void CBotrixEngineUtil::EntityHead(edict_t* pEntity, Vector& v)
{
	if ( !pEntity )
	{
		v = Vector();
		return;
	}

	v = Vector(pEntity->v.origin) + Vector(pEntity->v.view_ofs);
}

void CBotrixEngineUtil::SetPVSForVector(const Vector& v)
{
	const byte* pvsData = g_engfuncs.pfnGetPvsForPoint(v);

	if ( pvsData )
	{
		// This will always be MAX_PVS_DATA_LENGTH bytes long.
		memcpy(pvs, pvsData, MAX_PVS_DATA_LENGTH);
	}
	else
	{
		memset(pvs, 0xFF, MAX_PVS_DATA_LENGTH);
	}
}

bool CBotrixEngineUtil::IsVisiblePVS(const Vector& v)
{
	return g_engfuncs.pfnPointInPvs(v, pvs);
}

bool CBotrixEngineUtil::IsVisible(const Vector& vSrc, const Vector& vDest, TVisibility /* iVisibility */, bool bUsePVS)
{
	if ( bUsePVS )
	{
		SetPVSForVector(vSrc);

		if ( !IsVisiblePVS(vDest) )
		{
			return false;
		}
	}

	// NFTODO: We currently don't have the same trace filtering granularity as
	// the Source engine. We need to implement this in future for Ronin
	// collisions anyway, so this should be updated when that happens.
	TRACE_LINE(vSrc, vDest, TRUE, nullptr, &m_TraceResult);

	return !TraceHitSomething();
}

bool CBotrixEngineUtil::IsVisible(const Vector& vSrc, edict_t* pDest)
{
	Vector v;
	EntityHead(pDest, v);

	return IsVisible(vSrc, v, EVisibilityBots);
}

bool CBotrixEngineUtil::TraceHitSomething()
{
	return m_TraceResult.flFraction >= 0.0f && m_TraceResult.flFraction < 1.0f;
}

Vector
CBotrixEngineUtil::GetHullGroundVec(const Vector& vSrc, struct edict_s* ignoreEnt, int hull, float startSolidNudge)
{
	// The returned point is the middle of the hull, so this corrects to return the base.
	const Vector groundCorrection(0.0f, 0.0f, CBotrixMod::GetVar(EModVarPlayerHeight) / 2.0f);

	// If we're allowed to nudge, allow us to try more than one iteration.
	const size_t totalIterations = startSolidNudge > 0.0f ? 5 : 1;

	Vector vDest = vSrc;
	vDest.z = -iHalfMaxMapSize + groundCorrection.z;

	for ( size_t iteration = 0; iteration < totalIterations; ++iteration )
	{
		Vector nudge(0.0f, 0.0f, 0.0f);

		switch ( iteration )
		{
			case 1:
			{
				nudge.x = startSolidNudge;
				break;
			}

			case 2:
			{
				nudge.y = startSolidNudge;
				break;
			}

			case 3:
			{
				nudge.x = -startSolidNudge;
				break;
			}

			case 4:
			{
				nudge.y = -startSolidNudge;
				break;
			}

			default:
			{
				break;
			}
		}

		TRACE_HULL(vSrc + nudge, vDest, TRUE, hull, ignoreEnt, &m_TraceResult);

		if ( m_TraceResult.fStartSolid || m_TraceResult.fAllSolid )
		{
			if ( iteration < totalIterations - 1 )
			{
				// Try again with a different nudge to see if we can get a better trace.
				continue;
			}

			// Might be very slightly in the ground, in which case
			// this is already the ground position.
			// Not much else we can guess by this point.
			return vSrc - groundCorrection;
		}

		if ( m_TraceResult.flFraction >= 1.0f )
		{
			// There is no ground.
			// Certain computations check the result against -iHalfMaxMapSize,
			// so make sure we return exactly this.
			return Vector(vDest.x, vDest.y, -iHalfMaxMapSize);
		}

		return Vector(m_TraceResult.vecEndPos) - groundCorrection;
	}

	// Should never get here.
	ASSERT(false);
	return vSrc - groundCorrection;
}

Vector CBotrixEngineUtil::GetHumanHullGroundVec(
	const Vector& vSrc,
	PositionInHull posInHull,
	struct edict_s* ignoreEnt,
	float startSolidNudge
)
{
	Vector traceStart = vSrc;

	switch ( posInHull )
	{
		// Given the eye position, need the hull centre position.
		case PositionInHull::Eye:
		{
			traceStart.z -= CBotrixMod::GetVar(EModVarPlayerEye);
			traceStart.z += CBotrixMod::GetVar(EModVarPlayerHeight) / 2.0f;
			break;
		}

		// Given the feet position, need the hull centre position.
		case PositionInHull::Feet:
		{
			traceStart.z += CBotrixMod::GetVar(EModVarPlayerHeight) / 2.0f;
			break;
		};

		default:
		{
			break;
		}
	}

	// Result is always returned as the feet position.
	return GetHullGroundVec(traceStart, ignoreEnt, human_hull, startSolidNudge);
}

bool CBotrixEngineUtil::RayHitsEntity(edict_t* pEntity, const Vector& vSrc, const Vector& vDest)
{
	g_engfuncs.pfnTraceModel(vSrc, vDest, 0, pEntity, &m_TraceResult);
	return m_TraceResult.flFraction >= 0.95f;
}

TReach CBotrixEngineUtil::GetWaypointReachableInfoFromTo(
	TraceDirection direction,
	const Vector& vSrcEyePos,
	Vector& vDestEyePos,
	bool& bCrouch,
	float fDistanceSqr,
	float fMaxDistanceSqr,
	bool bShowHelp
)
{
	static constexpr float ROOT_TWO = 1.414f;
	static const unsigned char COL_CANREACH[3] = {0xEA, 0xFC, 0xEB};
	static const unsigned char COL_CANTREACH[3] = {0xFC, 0xEA, 0xEB};

	using namespace CustomGeometry;

	int color = direction == TraceDirection::EFirstToSecond ? 0xFF0000 : 0x00FF00;
	unsigned char r = GET_3RD_BYTE(color);
	unsigned char g = GET_2ND_BYTE(color);
	unsigned char b = GET_1ST_BYTE(color);

	Vector vOffset =
		direction == TraceDirection::EFirstToSecond ? Vector(0.25f, 0.25f, 2.0f) : Vector(-0.25f, -0.25f, 1.0f);

	if ( fDistanceSqr <= 0.0f )
	{
		fDistanceSqr = (vDestEyePos.Make2D() - vSrcEyePos.Make2D()).LengthSquared();
	}

	if ( fDistanceSqr > fMaxDistanceSqr )
	{
		return EReachNotReachable;
	}

	if ( !IsVisible(vSrcEyePos, vDestEyePos, EVisibilityWaypoints) )
	{
		return EReachNotReachable;
	}

	// Check if can swim there first.
	// TODO: May need a better check than this! What if they're
	// visible down a channel too narrow for the player to swim through?
	int iSrcContent = g_engfuncs.pfnPointContents(vSrcEyePos);
	int iDestContent = g_engfuncs.pfnPointContents(vDestEyePos);

	if ( iSrcContent == CONTENTS_WATER && iDestContent == CONTENTS_WATER )
	{
		return EReachReachable;
	}

	// Get all needed vars ready.
	float fPlayerEye = CBotrixMod::GetVar(EModVarPlayerEye);
	float fPlayerEyeCrouched = CBotrixMod::GetVar(EModVarPlayerEyeCrouched);

	// Vector vGroundMaxs = CBotrixMod::vPlayerCollisionHullMaxsGround;
	// vGroundMaxs.z = 1.0f;

	// Get ground positions.
	Vector vSrcGround = GetHumanHullGroundVec(vSrcEyePos, PositionInHull::Eye);

	if ( vSrcGround.z <= -iHalfMaxMapSize )
	{
		return EReachNotReachable;
	}

	Vector vDestGround = GetHumanHullGroundVec(vDestEyePos, PositionInHull::Eye);

	if ( vDestGround.z <= -iHalfMaxMapSize )
	{
		return EReachNotReachable;
	}

	// Check if take damage at fall.
	if ( vSrcGround.z - vDestGround.z >= CBotrixMod::GetVar(EModVarHeightForFallDamage) )
	{
		return EReachNotReachable;
	}

	// Try to get up if needed.
	float zDiff = vDestEyePos.z - vDestGround.z;

	if ( zDiff == 0 )
	{
		return EReachNotReachable;  // Can happens when the vDestGround is inside some solid.
	}

	if ( zDiff != fPlayerEye && zDiff != fPlayerEyeCrouched )
	{
		if ( !bCrouch )
		{
			// Try to stand up.
			vDestEyePos.z = vDestGround.z + fPlayerEye;
			HumanHullTrace(vDestGround, vDestEyePos);
			bCrouch = TraceHitSomething();
		}

		if ( bCrouch )
		{
			// Try to stand up crouching.
			vDestEyePos.z = vDestGround.z + fPlayerEyeCrouched;
			HumanHullTrace(vDestGround, vDestEyePos, head_hull);

			if ( TraceHitSomething() )
			{
				return EReachNotReachable;
			}
		}
	}

	// Draw waypoints until ground.
	if ( bShowHelp )
	{
		// TODO: draw from the ground position.
		DrawLine(vSrcEyePos + vOffset, vSrcGround + vOffset, static_cast<float>(iTextTime), r, g, b);
		DrawLine(vDestEyePos + vOffset, vDestGround + vOffset, static_cast<float>(iTextTime), r, g, b);
	}

	// Start off by assuming the destination is reachable.
	TReach iResult = EReachReachable;

	// Need to trace several times to know if can jump up all obstacles.
	Vector vHit = vSrcGround;
	Vector vDirection = vDestGround - vSrcGround;
	vDirection.z = 0.0f;  // We need only X-Y direction.
	vDirection = vDirection.Normalize();

	Vector vLastStair;
	bool bNeedJump = false;
	bool bHasStair = false;

	std::unique_ptr<CRollingLineMessageWriter> helperGeomWriter;

	if ( bShowHelp )
	{
		helperGeomWriter.reset(new CRollingLineMessageWriter(Category::BotrixDebugging));

		helperGeomWriter
			->BeginGeometry((static_cast<uint32_t>(color) << 8) | 0x000000FF, 1.0f, static_cast<float>(iTextTime));
	}

	float lastDistToTarget = std::numeric_limits<float>::max();

	for ( int i = 0; i < iMaxTraceRaysForReachable; ++i )
	{
		// Begin from where we last hit.
		Vector vStart = vHit;

		// Trace from hit point to the floor.
		TReach iReach = CanPassOrJump(vHit, vDirection);

		if ( bShowHelp )
		{
			helperGeomWriter->AddLine(vStart + vOffset, vHit + vOffset);
		}

		switch ( iReach )
		{
			case EReachNotReachable:
			{
				if ( bShowHelp )
				{
					DrawTextAtLocation(
						vHit,
						0,
						static_cast<float>(iTextTime),
						COL_CANTREACH[0],
						COL_CANTREACH[1],
						COL_CANTREACH[2],
						"Too High"
					);
				}

				return EReachNotReachable;
			}

			case EReachNeedJump:
			{
				if ( bNeedJump )
				{
					if ( bShowHelp )
					{
						DrawTextAtLocation(
							vHit,
							0,
							static_cast<float>(iTextTime),
							COL_CANTREACH[0],
							COL_CANTREACH[1],
							COL_CANTREACH[2],
							"Too High"
						);
					}

					return EReachNotReachable;
				}

				bNeedJump = true;
				break;
			}

			case EReachStairs:
			{
				float fDistSqr = (vHit.Make2D() - vLastStair.Make2D()).LengthSquared();

				if ( bHasStair && fDistSqr <= 9 )  // 3 units at least
				{
					if ( bShowHelp )
					{
						DrawTextAtLocation(
							vHit,
							0,
							static_cast<float>(iTextTime),
							COL_CANTREACH[0],
							COL_CANTREACH[1],
							COL_CANTREACH[2],
							"Slope"
						);
					}

					return EReachNotReachable;
				}

				bHasStair = true;
				vLastStair = vStart;
				break;
			}

			case EReachReachable:
			{
				break;
			}

			default:
			{
				GoodAssert(false);
				break;
			}
		}

		float distToTarget = (vDestGround.Make2D() - vHit.Make2D()).Length();

		if ( distToTarget < (ROOT_TWO / 2.0f) || distToTarget > lastDistToTarget + (ROOT_TWO / 2.0f) )
		{
			// We got to the waypoint, or are now getting further away, so no point continuing.
			break;
		}

		lastDistToTarget = distToTarget;
	}

	if ( bShowHelp )
	{
		helperGeomWriter->Finalise();
		helperGeomWriter.reset();
	}

	// Set text position.
	Vector vText = (vSrcGround + vDestGround) / 2;
	vText.z += direction == TraceDirection::EFirstToSecond ? 20 : 10;

	if ( !EqualVectors(vHit, vDestGround) )
	{
		if ( (vDestGround.Make2D() - vHit.Make2D()).Length() < ROOT_TWO )
		{
			// We actually made it to the waypoint location, but our Z changed.
			// Update the destination vector.
			vDestEyePos = vHit + Vector(0.0f, 0.0f, fPlayerEye);
		}
		else
		{
			iResult = EReachNotReachable;
		}
	}

	switch ( iResult )
	{
		case EReachReachable:
		{
			if ( bNeedJump )
			{
				iResult = EReachNeedJump;

				if ( bShowHelp )
				{
					DrawTextAtLocation(
						vText,
						0,
						static_cast<float>(iTextTime),
						COL_CANREACH[0],
						COL_CANREACH[1],
						COL_CANREACH[2],
						"Jump"
					);
				}
			}
			else if ( bShowHelp )
			{
				DrawTextAtLocation(
					vText,
					0,
					static_cast<float>(iTextTime),
					COL_CANREACH[0],
					COL_CANREACH[1],
					COL_CANREACH[2],
					"Walk"
				);
			}

			break;
		}

		case EReachFallDamage:
		{
			if ( bShowHelp )
			{
				DrawTextAtLocation(
					vText,
					0,
					static_cast<float>(iTextTime),
					COL_CANREACH[0],
					COL_CANREACH[1],
					COL_CANREACH[2],
					"Fall"
				);
			}

			break;
		}

		case EReachNotReachable:
		{
			if ( bShowHelp )
			{
				DrawTextAtLocation(
					vText,
					0,
					static_cast<float>(iTextTime),
					COL_CANTREACH[0],
					COL_CANTREACH[1],
					COL_CANTREACH[2],
					"Unreachable"
				);
			}

			break;
		}

		default:
		{
			GoodAssert(false);
			break;
		}
	}

	return iResult;
}

void CBotrixEngineUtil::NormalizeAngle(float& fAngle)
{
	if ( fAngle < 0.0f )
	{
		fAngle += 360.0f;
	}
	else if ( fAngle >= 360.0f )
	{
		fAngle -= 360.0f;
	}
}

void CBotrixEngineUtil::DeNormalizeAngle(float& fAngle)
{
	if ( fAngle < -180.0f )
	{
		fAngle += 360.0f;
	}
	else if ( fAngle >= 180.0f )
	{
		fAngle -= 360.0f;
	}
}

bool CBotrixEngineUtil::CanPassSlope(float fAngle, float fMaxSlope)
{
	DeNormalizeAngle(fAngle);
	fAngle = fabsf(fAngle);

	if ( fAngle <= 90.0f )
	{
		return fAngle < fMaxSlope;
	}
	else
	{
		return fAngle > 180.0f - fMaxSlope;
	}
}

void CBotrixEngineUtil::GetAngleDifference(Vector const& angOrigin, Vector const& angDestination, Vector& angDiff)
{
	angDiff = angDestination - angOrigin;

	DeNormalizeAngle(angDiff.x);
	DeNormalizeAngle(angDiff.y);
}

Vector CBotrixEngineUtil::GetGroundVec(const Vector& vSrc)
{
	Vector vDest = vSrc;
	vDest.z = -iHalfMaxMapSize;

	TRACE_LINE(vSrc, vDest, TRUE, nullptr, &m_TraceResult);
	return m_TraceResult.vecEndPos;
}

//----------------------------------------------------------------------------------------------------------------
// Returns true if can move forward when standing at vGround performing a jump (normal, with crouch, or maybe just
// walking). At return vGround contains new coord which is where player can get after jump.
//----------------------------------------------------------------------------------------------------------------
TReach CBotrixEngineUtil::CanPassOrJump(Vector& vGround, const Vector& vDirectionInc)
{
	// Try to walk unit.
	Vector vHit = vGround + vDirectionInc;

	HumanHullTrace(vGround, vHit);

	if ( m_TraceResult.fAllSolid )
	{
		// We should never have a trace entirely in a solid object.
		return EReachNotReachable;
	}

	if ( !TraceHitSomething() )
	{
		// Make sure to get the ground, in case we need to fall down off an obstacle.
		vGround = GetHumanHullGroundVec(vHit, PositionInHull::Feet);
		return EReachReachable;
	}

	if ( !EqualVectors(vGround, m_TraceResult.vecEndPos, 0.01f) )
	{
		// We hit something, but we can walk from the ground to this location.
		vGround = GetHumanHullGroundVec(m_TraceResult.vecEndPos, PositionInHull::Feet);
		return EReachReachable;
	}

	// We have butted up against an obstacle.
	// Try to walk over (one stair height) to see if we can clear it.
	float fMaxWalkHeight = CBotrixMod::GetVar(EModVarPlayerObstacleToJump);
	Vector vStair = vGround;
	vStair.z += fMaxWalkHeight;
	vHit.z += fMaxWalkHeight;

	HumanHullTrace(vStair, vHit);

	if ( !TraceHitSomething() )
	{
		vStair = vGround;
		vGround = GetHumanHullGroundVec(vHit, PositionInHull::Feet);

		if ( CanClimbSlope(vStair, vGround) )
		{
			// Check if can climb up without making the stair step.
			HumanHullTrace(vStair, vGround);

			if ( !TraceHitSomething() )
			{
				return EReachReachable;
			}
		}

		return EReachStairs;
	}

	// We can't make it over the obstacle with a step, so try jumping.
	float fJumpCrouched = CBotrixMod::GetVar(EModVarPlayerJumpHeightCrouched);
	vGround.z += fJumpCrouched;
	vHit.z += fJumpCrouched - fMaxWalkHeight;  // We added previously fMaxWalkHeight.

	HumanHullTrace(vGround, vHit);

	if ( !TraceHitSomething() )  // We can stand on vHit after jump.
	{
		vGround = GetHumanHullGroundVec(vHit, PositionInHull::Feet);
		return EReachNeedJump;
	}

	vGround = GetHumanHullGroundVec(m_TraceResult.vecEndPos, PositionInHull::Feet);
	return EReachNotReachable;  // Can't jump over.
}

TReach CBotrixEngineUtil::CanClimbSlope(const Vector& vSrc, const Vector& vDest)
{
	Vector vDiff = vDest - vSrc;
	Vector ang;
	VectorAngles(vDiff, ang);  // Get pitch to know if gradient is too big.

	if ( !CWaypoints::IsAnalyzing() )
	{
		BLOG_T("Slope angle %.2f", ang.x);
	}

	float fSlope = CBotrixMod::GetVar(EModVarSlopeGradientToSlideOff);
	return CanPassSlope(ang.x, fSlope) ? EReachReachable : EReachNotReachable;
}

// Trace a hull between two points on the ground.
// Because the hull trace expects the points provided to be at the centre
// of the hull, we accommodate this here and reset the eventual
// collision
void CBotrixEngineUtil::HumanHullTrace(const Vector& vGround1, const Vector& vGround2, int hull)
{
	const float halfPlayerHeight = CBotrixMod::GetVar(EModVarPlayerHeight) / 2.0f;

	Vector begin = vGround1;
	begin.z += halfPlayerHeight;

	Vector end = vGround2;
	end.z += halfPlayerHeight;

	TRACE_HULL(begin, end, ignore_monsters, hull, nullptr, &m_TraceResult);

	m_TraceResult.vecEndPos[VEC3_Z] -= halfPlayerHeight;
}

void CBotrixEngineUtil::DrawBeam(
	const Vector& v1,
	const Vector& v2,
	unsigned char iWidth,
	float fDrawTime,
	unsigned char r,
	unsigned char g,
	unsigned char b
)
{
	MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY);
	WRITE_BYTE(TE_BEAMPOINTS);
	WRITE_COORD(v1.x);
	WRITE_COORD(v1.y);
	WRITE_COORD(v1.z);
	WRITE_COORD(v2.x);
	WRITE_COORD(v2.y);
	WRITE_COORD(v2.z);
	WRITE_SHORT(g_sModelIndexLaser);
	WRITE_BYTE(0);  // frame start
	WRITE_BYTE(10);  // framerate
	WRITE_BYTE(static_cast<int>(fDrawTime * 10.0f));  // life, 10ths of a second
	WRITE_BYTE(iWidth);  // width
	WRITE_BYTE(0);  // noise
	WRITE_BYTE(r);  // r, g, b
	WRITE_BYTE(g);  // r, g, b
	WRITE_BYTE(b);  // r, g, b
	WRITE_BYTE(255);  // brightness
	WRITE_BYTE(10);  // speed
	MESSAGE_END();
}

void CBotrixEngineUtil::DrawLine(
	const Vector& v1,
	const Vector& v2,
	float fDrawTime,
	unsigned char r,
	unsigned char g,
	unsigned char b
)
{
	using namespace CustomGeometry;

	CGeometryItem geom;
	geom.SetDrawType(DrawType::Lines);
	geom.AddLine(v1, v2);
	geom.SetColour((r << 24) | (g << 16) | (b << 8) | 0xFF);
	geom.SetLifetimeSecs(fDrawTime);

	CMessageWriter(Category::BotrixDebugging).WriteMessage(geom);
}

void CBotrixEngineUtil::DrawWireBall(
	const Vector& location,
	float radius,
	float fDrawTime,
	unsigned char r,
	unsigned char g,
	unsigned char b
)
{
	using namespace CustomGeometry;

	CPrimitiveMessageWriter writer(CustomGeometry::Category::BotrixDebugging);
	WireBallPrimitive primitive {};
	primitive.origin = location;
	primitive.radius =
		static_cast<uint16_t>(bound(1.0f, radius, static_cast<float>(std::numeric_limits<uint16_t>::max())));
	primitive.numDivisions = 16;
	writer.WriteMessage((r << 24) | (g << 16) | (b << 8) | 0xFF, fDrawTime, primitive);
}

void CBotrixEngineUtil::DrawBox(
	const Vector& vOrigin,
	const Vector& vMins,
	const Vector& vMaxs,
	float fDrawTime,
	unsigned char r,
	unsigned char g,
	unsigned char b
)
{
	using namespace CustomGeometry;

	AABBoxPrimitive primitive {};
	primitive.mins = vOrigin + vMins;
	primitive.maxs = vOrigin + vMaxs;

	if ( !primitive.IsValid() )
	{
		return;
	}

	CPrimitiveMessageWriter(Category::BotrixDebugging)
		.WriteMessage((r << 24) | (g << 16) | (b << 8) | 0xFF, fDrawTime, primitive);
}

void CBotrixEngineUtil::DrawTextAtLocation(
	const Vector& vOrigin,
	int iLine,
	float fDrawTime,
	unsigned char r,
	unsigned char g,
	unsigned char b,
	const char* szText
)
{
	using namespace CustomGeometry;

	if ( !szText || !(*szText) )
	{
		return;
	}

	CGeometryItem item;
	item.SetDrawType(DrawType::Text);
	item.SetLifetimeSecs(fDrawTime);
	item.SetColour((r << 24) | (g << 16) | (b << 8) | 0xFF);
	item.SetText(vOrigin, CUtlString(szText), iLine);

	CMessageWriter(Category::BotrixDebugging).WriteMessage(item);
}
