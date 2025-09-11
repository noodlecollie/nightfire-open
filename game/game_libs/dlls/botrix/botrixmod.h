#pragma once

#include <memory>
#include <good/ini_file.h>
#include <good/defines.h>
#include "MathLib/vec3.h"
#include "botrix/types.h"
#include "utlvector.h"

class CBotrixModDetail;
class CPlayer;

class CBotrixMod
{
public:  // Methods.
	static std::unique_ptr<CBotrixModDetail> pCurrentMod;

	/// Get mod id for this game mod.
	static TModId GetModId()
	{
		return m_iModId;
	}

	/// Load all needed staff for mod.
	static bool LoadDefaults(TModId iModId);

	/// Prepare for use, called after all needed vars are set.
	static void Prepare();

	/// Unload mod.
	static void UnLoad()
	{
		m_aFrameEvents.Purge();

		m_iModId = EModId_Invalid;
		sModName = "";
		aTeamsNames.clear();
		aBotNames.clear();
		pCurrentMod.reset();
	}

	/// Called when map finished loading items and waypoints.
	static void MapLoaded();

	/// Mod's think function.
	static void Think();

	/// Add frame event.
	static void AddFrameEvent(TPlayerIndex iPlayer, TFrameEvent iEvent)
	{
		std::pair<TFrameEvent, TPlayerIndex> pair(iEvent, iPlayer);
		int foundIndex = m_aFrameEvents.Find(pair);

		if ( foundIndex < 0 )  // Avoid firing events twice.
		{
			m_aFrameEvents.AddToTail(pair);
		}
	}

	/// Return true if map has items or waypoint's of given type.
	static bool HasMapItems(TItemType iEntityType)
	{
		return m_bMapHas[iEntityType];
	}

	/// Get team index from team name.
	static int GetTeamIndex(const good::string& sTeam)
	{
		for ( good::string::size_type i = 0; i < aTeamsNames.size(); ++i )
		{
			if ( sTeam == aTeamsNames[i] )
			{
				return i;
			}
		}

		return -1;
	}

	/// Get random bot name from [General] section, key bot_names.
	static const good::string& GetRandomBotName(TBotIntelligence iIntelligence);

	/// Get var value for needed class.
	static float GetVar(TModVar iVar, TClass iClass = 0)
	{
		GoodAssert(0 <= iVar && iVar < EModVarTotal);
		int iIndex = iClass < m_aVars[iVar].Count() ? iClass : 0;
		return m_aVars[iVar][iIndex];
	}

	static void SetVars(const CUtlVector<float>* aVars)
	{
		for ( int iVar = 0; iVar < EModVarTotal; ++iVar )
		{
			if ( aVars[iVar].Count() > 0 )
			{
				m_aVars[iVar] = aVars[iVar];
			}
		}
	}

	static const char* GetLastError();

public:  // Static members.
	static good::string sModName;  ///< Mod name.
	static StringVector aTeamsNames;  ///< Name of teams.
	static int iUnassignedTeam;  ///< Index of unassigned (deathmatch) team.
	static int iSpectatorTeam;  ///< Index of spectator team.

	static CUtlVector<TWeaponId> aDefaultWeapons;  ///< Default respawn weapons. Can be set by a console command.
	static bool
		bRemoveWeapons;  ///< If true, will remove all weapons from the bot on respawn. Can be set by a console command.

	static StringVector aBotNames;  ///< Available bot names.
	static StringVector aClassNames;  ///< Name of player's classes.

	static bool bIntelligenceInBotName;  ///< Use bot's intelligence as part of his name.
	static bool bHeadShotDoesMoreDamage;  ///< HL2DM, CSS have that (true by default).
	static bool bUseModels;  ///< HL2DM, CSS have that (true by default).

	static float fSpawnProtectionTime;  ///< Spawn protection time, 0 by default. Can be set by a console command.
	static int iSpawnProtectionHealth;  ///< Spawn protection health, 0 by default. Can be set by a console command.

	//    static TDeathmatchFlags iDeathmatchFlags;       ///< Flags for deathmatch mode.

	static Vector vPlayerCollisionHull;  ///< Maxs of player collision box with origin in (0, 0, 0).
	static Vector vPlayerCollisionHullCrouched;  ///< Maxs of crouched player collision box with origin in (0, 0, 0).

	static Vector vPlayerCollisionHullMins;  // Those are for centered hull in (0, 0, 0).
	static Vector vPlayerCollisionHullMaxs;

	static Vector vPlayerCollisionHullCrouchedMins;
	static Vector vPlayerCollisionHullCrouchedMaxs;

	static Vector vPlayerCollisionHullMaxsGround;

	static float fMinNonStuckSpeed;  ///< Minimum velocity to consider that bot is moving and non stucked.

	static int iPlayerRadius;  ///< Player's radius (used to check if bot is stucked).
	static int iNearItemMaxDistanceSqr;  ///< Max distance to consider item to be near to player.
	static int iItemPickUpDistance;  ///< Additional distance from player to item to consider it taken.
									 // Item is picked, if distance-to-player < player's-radius + item's-radius +
									 // this-distance.

	// TODO: These should be cvars
	static int iPointTouchSquaredXY;  ///< Squared distance to consider that we are touching waypoint.
	static int iPointTouchSquaredZ;  ///< Z distance to consider that we are touching waypoint. Should be no more than
									 ///< player can jump.
	static int iPointTouchLadderSquaredZ;  ///< Z distance to consider that we are touching waypoint while on ladder.

protected:  // Methods.
	friend class CConfiguration;  // Give access to next protected methods.

	// Returns true there is a player/bot with name cName.
	static bool IsNameTaken(const good::string& cName, TBotIntelligence iIntelligence);

protected:  // Members.
	static TModId m_iModId;  // Mod id.

	// Mod vars.
	static CUtlVector<float> m_aVars[EModVarTotal];

	static bool m_bMapHas[EItemTypeCanPickTotal];  // To check if map has items or waypoints of types: health, armor,
												   // weapon, ammo.

	// Events that happend on this frame.
	static CUtlVector<std::pair<TFrameEvent, TPlayerIndex>> m_aFrameEvents;
};

class CBotrixModDetail
{
public:  // Methods.
	/// Constructor. Initializes console commands.
	CBotrixModDetail();

	//------------------------------------------------------------------------------------------------------------
	// Implementation of IMod inteface.
	//------------------------------------------------------------------------------------------------------------
	/// Process configuration file.
	bool ProcessConfig(const good::ini_file& cIni);

	/// Called when map is loaded, after waypoints and items has been loaded.
	void MapLoaded()
	{
	}

	/// Called when map is unloaded.
	void MapFinished()
	{
	}

	/// Add bot with given name, intelligence, class and other optional parameters.
	CPlayer* AddBot(
		const char* szName,
		TBotIntelligence iIntelligence,
		TTeam iTeam,
		TClass iClass,
		int iParamsCount,
		const char** aParams
	);

	/// Get waypoint type count.
	int GetWaypointTypeCount()
	{
		return 0;
	}

	/// Get waypoint type names.
	const good::string* GetWaypointTypeNames()
	{
		return NULL;
	}

	/// Get waypoint type colors.
	const int* GetWaypointTypeColors()
	{
		return NULL;
	}

	/// Get waypoint path count.
	int GetWaypointPathCount()
	{
		return 0;
	}

	/// Get waypoints path names.
	const good::string* GetWaypointPathNames()
	{
		return NULL;
	}

	/// Get waypoints path colors.
	const int* GetWaypointPathColors()
	{
		return NULL;
	}

	/// Get chat count.
	int GetChatCount()
	{
		return 0;
	}

	/// Get chat names.
	const good::string* GetChatNames()
	{
		return NULL;
	}

	/// Mod think function.
	void Think()
	{
	}

	//------------------------------------------------------------------------------------------------------------
	/// Get random bot model.
	const good::string* GetRandomModel(int iTeam)
	{
		GoodAssert((iTeam != CBotrixMod::iSpectatorTeam) && (iTeam != CBotrixMod::iUnassignedTeam));
		return m_aModels[iTeam].size() ? &m_aModels[iTeam][rand() % m_aModels[iTeam].size()] : NULL;
	}

	/// Get team for model.
	TTeam GetTeamOfModel(const good::string& sModel)
	{
		for ( int iTeam = 0; iTeam < m_aModels.Count(); ++iTeam )
		{
			StringVector::const_iterator it = good::find(m_aModels[iTeam], sModel);
			if ( it != m_aModels[iTeam].end() )
			{
				return iTeam;
			}
		}

		return -1;
	}

	const char* GetLastError() const;

protected:
	good::string m_sLastError;
	CUtlVector<StringVector> m_aModels;  ///< Available models for teams.
};
