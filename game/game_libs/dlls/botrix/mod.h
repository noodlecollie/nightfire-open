#pragma once

#include <memory>
#include <good/ini_file.h>
#include <good/defines.h>
#include "MathLib/vec3.h"
#include "botrix/types.h"
#include "utlvector.h"

namespace Botrix
{
	class CPlayer;

	class CMod
	{
	public:  // Methods.
		/// Load all needed staff for mod.
		static bool LoadDefaults();

		/// Prepare for use, called after all needed vars are set.
		static void Prepare();

		/// Unload mod.
		static void UnLoad()
		{
			m_aFrameEvents.Purge();
			aTeamsNames.clear();
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

		static CPlayer*
		AddBot(const char* szName, TBotIntelligence iIntelligence, TTeam iTeam, int iParamsCount, const char** aParams);

	public:  // Static members.
		static StringVector aTeamsNames;  ///< Name of teams.
		static int iUnassignedTeam;  ///< Index of unassigned (deathmatch) team.
		static int iSpectatorTeam;  ///< Index of spectator team.

		static CUtlVector<TWeaponId> aDefaultWeapons;  ///< Default respawn weapons. Can be set by a console command.
		static bool bRemoveWeapons;  ///< If true, will remove all weapons from the bot on respawn. Can be set by a
									 ///< console command.

		static StringVector aClassNames;  ///< Name of player's classes.

		static bool bIntelligenceInBotName;  ///< Use bot's intelligence as part of his name.
		static bool bHeadShotDoesMoreDamage;  ///< HL2DM, CSS have that (true by default).

		static float fSpawnProtectionTime;  ///< Spawn protection time, 0 by default. Can be set by a console command.
		static int iSpawnProtectionHealth;  ///< Spawn protection health, 0 by default. Can be set by a console command.

		//    static TDeathmatchFlags iDeathmatchFlags;       ///< Flags for deathmatch mode.

		static float fMinNonStuckSpeed;  ///< Minimum velocity to consider that bot is moving and non-stuck.

		static int iNearItemMaxDistanceSqr;  ///< Max distance to consider item to be near to player.
		static int iItemPickUpDistance;  ///< Additional distance from player to item to consider it taken.
										 // Item is picked, if distance-to-player < player's-radius + item's-radius +
										 // this-distance.

	protected:  // Methods.
		friend class CConfiguration;  // Give access to next protected methods.

		// Returns true there is a player/bot with name cName.
		static bool IsNameTaken(const good::string& cName, TBotIntelligence iIntelligence);

	protected:  // Members.
		static bool m_bMapHas[EItemTypeCanPickTotal];  // To check if map has items or waypoints of types: health,
													   // armor, weapon, ammo.

		// Events that happend on this frame.
		static CUtlVector<std::pair<TFrameEvent, TPlayerIndex>> m_aFrameEvents;
	};
}  // namespace Botrix
