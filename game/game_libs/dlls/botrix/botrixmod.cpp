#include <good/string_utils.h>
#include <good/string_buffer.h>

#include "botrix/botrixmod.h"
#include "botrix/players.h"
#include "botrix/server_plugin.h"
#include "botrix/defines.h"
#include "botrix/type2string.h"
#include "botrix/waypoint.h"
#include "botrix/item.h"
#include "botrix/bot.h"
#include "botrix/parameter_vars.h"
#include "MathLib/mathlib.h"
#include "utils/mp_utils.h"

//----------------------------------------------------------------------------------------------------------------
good::string CBotrixMod::sModName;

StringVector CBotrixMod::aBotNames;
CUtlVector<std::pair<TFrameEvent, TPlayerIndex>> CBotrixMod::m_aFrameEvents;

bool CBotrixMod::m_bMapHas[EItemTypeCanPickTotal];  // Health, armor, weapon, ammo.

float CBotrixMod::fMinNonStuckSpeed = 30;

StringVector CBotrixMod::aTeamsNames;
int CBotrixMod::iUnassignedTeam = 0;
int CBotrixMod::iSpectatorTeam = 1;

CUtlVector<TWeaponId> CBotrixMod::aDefaultWeapons;

// Next are console commands
bool CBotrixMod::bRemoveWeapons = false;

StringVector CBotrixMod::aClassNames;

bool CBotrixMod::bIntelligenceInBotName = true;
bool CBotrixMod::bHeadShotDoesMoreDamage = true;

float CBotrixMod::fSpawnProtectionTime = 0;
int CBotrixMod::iSpawnProtectionHealth = 0;

// TDeathmatchFlags CBotrixMod::iDeathmatchFlags = -1;

int CBotrixMod::iNearItemMaxDistanceSqr = SQR(312);
int CBotrixMod::iItemPickUpDistance = 100;

//----------------------------------------------------------------------------------------------------------------
bool CBotrixMod::LoadDefaults()
{
	m_aFrameEvents.Purge();
	m_aFrameEvents.EnsureCapacity(8);

	return true;
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixMod::Prepare()
{
	CWaypoint::iAnalyzeDistance = static_cast<int>(CBotrixParameterVars::PLAYER_WIDTH * 2.0f);
	CWaypoint::iDefaultDistance = static_cast<int>(CBotrixParameterVars::PLAYER_WIDTH * 4.0f);
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixMod::MapLoaded()
{
	// TODO: move this to items.
	for ( TItemType iType = 0; iType < EItemTypeCanPickTotal; ++iType )
	{
		m_bMapHas[iType] = false;

		if ( CItems::GetItems(iType).size() > 0 )  // Check if has items.
		{
			m_bMapHas[iType] = true;
		}
		else
		{
			// Check if map has waypoints of given type.
			TWaypointFlags iFlags = CWaypoint::GetFlagsFor(iType);

			for ( TWaypointId id = 0; id < CWaypoints::Size(); ++id )
			{
				if ( FLAG_SOME_SET(CWaypoints::Get(id).iFlags, iFlags) )
				{
					m_bMapHas[iType] = true;
					break;
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
const good::string& CBotrixMod::GetRandomBotName(TBotIntelligence iIntelligence)
{
	int iIdx = rand() % aBotNames.size();

	for ( int i = iIdx; i < aBotNames.size(); ++i )
	{
		if ( !IsNameTaken(aBotNames[i], iIntelligence) )
		{
			return aBotNames[i];
		}
	}

	for ( int i = iIdx - 1; i >= 0; --i )
	{
		if ( !IsNameTaken(aBotNames[i], iIntelligence) )
		{
			return aBotNames[i];
		}
	}

	if ( iIdx < 0 )  // All names taken.
	{
		iIdx = rand() % aBotNames.size();
	}

	return aBotNames[iIdx];
}

//----------------------------------------------------------------------------------------------------------------
bool CBotrixMod::IsNameTaken(const good::string& cName, TBotIntelligence iIntelligence)
{
	for ( TPlayerIndex iPlayer = 0; iPlayer < CPlayers::Size(); ++iPlayer )
	{
		const CPlayer* pPlayer = CPlayers::Get(iPlayer);

		if ( pPlayer && good::starts_with(good::string(pPlayer->GetName()), cName) && pPlayer->IsBot() &&
			 (iIntelligence == ((CBotrixBot*)pPlayer)->GetIntelligence()) )
		{
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixMod::Think()
{
	for ( int i = 0; i < m_aFrameEvents.Count(); ++i )
	{
		CPlayer* pPlayer = CPlayers::Get(m_aFrameEvents[i].second);
		if ( pPlayer == NULL )
		{
			BLOG_E(
				"Player with index %d is not present to receive event %d.",
				m_aFrameEvents[i].second,
				m_aFrameEvents[i].first
			);
		}
		else
		{
			switch ( m_aFrameEvents[i].first )
			{
				case EFrameEventActivated:
				{
					pPlayer->Activated();
					break;
				}

				case EFrameEventRespawned:
				{
					pPlayer->Respawned();
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

	m_aFrameEvents.Purge();
}

CPlayer* CBotrixMod::AddBot(
	const char* szName,
	TBotIntelligence iIntelligence,
	TTeam iTeam,
	int iParamsCount,
	const char** aParams
)
{
	if ( iParamsCount > 0 )
	{
		BLOG_E("CBotrixMod::AddBot(): Unknown parameter %s", aParams[0]);
		return nullptr;
	}

	CUtlString netName = MPUtils::SanitisePlayerNetName(szName);
	edict_t* pEdict = g_engfuncs.pfnCreateFakeClient(netName.String());

	if ( !pEdict )
	{
		BLOG_E("CBotrixMod::AddBot(): Error, couldn't add bot (no map or server full?)");
		return nullptr;
	}

	CBot_HL2DM* result = new CBot_HL2DM(pEdict, iIntelligence);
	result->ChangeTeam(iTeam);
	return result;
}
