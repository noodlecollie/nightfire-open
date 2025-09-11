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
#include "botrix/constants.h"
#include "MathLib/mathlib.h"
#include "utils/mp_utils.h"

//----------------------------------------------------------------------------------------------------------------
std::unique_ptr<CBotrixModDetail> CBotrixMod::pCurrentMod;
TModId CBotrixMod::m_iModId;
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
bool CBotrixMod::bUseModels = true;

float CBotrixMod::fSpawnProtectionTime = 0;
int CBotrixMod::iSpawnProtectionHealth = 0;

// TDeathmatchFlags CBotrixMod::iDeathmatchFlags = -1;
CUtlVector<float> CBotrixMod::m_aVars[EModVarTotal];

Vector CBotrixMod::vPlayerCollisionHull;
Vector CBotrixMod::vPlayerCollisionHullCrouched;

Vector CBotrixMod::vPlayerCollisionHullMins;
Vector CBotrixMod::vPlayerCollisionHullMaxs;

Vector CBotrixMod::vPlayerCollisionHullCrouchedMins;
Vector CBotrixMod::vPlayerCollisionHullCrouchedMaxs;

Vector CBotrixMod::vPlayerCollisionHullMaxsGround;

int CBotrixMod::iPlayerRadius;

int CBotrixMod::iNearItemMaxDistanceSqr = SQR(312);
int CBotrixMod::iItemPickUpDistance = 100;

int CBotrixMod::iPointTouchSquaredXY;
int CBotrixMod::iPointTouchSquaredZ;
int CBotrixMod::iPointTouchLadderSquaredZ;

//----------------------------------------------------------------------------------------------------------------
bool CBotrixMod::LoadDefaults(TModId iModId)
{
	m_iModId = iModId;

	m_aFrameEvents.Purge();
	m_aFrameEvents.EnsureCapacity(8);

	const float defaults[EModVarTotal] = {
		/*max_health = */ 100.0f,
		/*max_armor = */ 100.0f,

		/*player_width = */ 32.0f,
		/*player_height = */ 72.0f,
		/*player_height_crouched = */ 36.0f,
		/*player_eye = */ 64.0f,
		/*player_eye_crouched = */ 36.0f,

		/*velocity_crouch = */ 250.0f * 0.3f,
		/*velocity_walk = */ 250.0f,
		/*velocity_run = */ 250.0f,
		/*velocity_sprint = */ 327.5f,

		/*obstacle_height_no_jump = */ 18.0f,
		/*jump_height = */ 20.0f,
		/*jump_height_crouched = */ 56.0f,

		/*max_height_no_fall_damage = */ 185.0f,
		/*max_slope_gradient = */ 45.0f,
	};

	for ( TModVar iVar = 0; iVar < EModVarTotal; ++iVar )
	{
		m_aVars[iVar].Purge();
		m_aVars[iVar].AddToTail(defaults[iVar]);
	}

	pCurrentMod.reset(new CBotrixModDetail());

	return true;
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixMod::Prepare()
{
	const float fWidth = CBotrixConstants::PLAYER_WIDTH;
	const float fHalfWidth = fWidth / 2.0f;
	const float fHeight = CBotrixConstants::PLAYER_HEIGHT;
	const float fHeightCrouched = CBotrixConstants::PLAYER_HEIGHT_CROUCHED;
	const float fJumpCrouched = m_aVars[EModVarPlayerJumpHeightCrouched][0];

	iPlayerRadius = static_cast<int>(rsqrt(2 * SQR(fHalfWidth)));  // Pythagoras.
	iPointTouchSquaredXY = static_cast<int>(SQR(fWidth / 4));
	iPointTouchSquaredZ = static_cast<int>(SQR(fJumpCrouched));
	iPointTouchLadderSquaredZ = SQR(5);

	CWaypoint::iAnalyzeDistance = static_cast<int>(fWidth * 2);
	CWaypoint::iDefaultDistance = static_cast<int>(fWidth * 4);

	// Get max collision hull, so bot doesn't stack (when auto-create waypoints).
	// fWidth *= FastSqrt( 2.0f );
	// fHalfWidth = fWidth / 2.0f;

	vPlayerCollisionHull = Vector(fWidth, fWidth, fHeight);
	vPlayerCollisionHullMins = Vector(-fHalfWidth, -fHalfWidth, 0);
	vPlayerCollisionHullMaxs = Vector(fHalfWidth, fHalfWidth, fHeight);
	vPlayerCollisionHullMaxsGround = Vector(fHalfWidth, fHalfWidth, 1.0f);

	vPlayerCollisionHullCrouched = Vector(fWidth, fWidth, fHeightCrouched);
	vPlayerCollisionHullCrouchedMins = Vector(-fHalfWidth, -fHalfWidth, 0);
	vPlayerCollisionHullCrouchedMaxs = Vector(fHalfWidth, fHalfWidth, fHeightCrouched);
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

	if ( pCurrentMod )
	{
		pCurrentMod->MapLoaded();
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
	if ( pCurrentMod )
	{
		pCurrentMod->Think();
	}

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

const char* CBotrixMod::GetLastError()
{
	return pCurrentMod ? pCurrentMod->GetLastError() : "";
}

CBotrixModDetail::CBotrixModDetail()
{
	m_aModels.EnsureCount(CBotrixMod::aTeamsNames.size());
}

//----------------------------------------------------------------------------------------------------------------
bool CBotrixModDetail::ProcessConfig(const good::ini_file& cIni)
{
	static constexpr size_t BUFFER_SIZE = 1024;
	static char s_Buffer[BUFFER_SIZE];

	// Find section "<mod name>.models".
	good::string_buffer sbBuffer(s_Buffer, BUFFER_SIZE, false);
	sbBuffer = CBotrixMod::sModName;
	sbBuffer << ".models";

	good::ini_file::const_iterator it = cIni.find(sbBuffer);
	if ( it != cIni.end() )
	{
		StringVector aModels;
		m_aModels.EnsureCount(CBotrixMod::aTeamsNames.size());

		good::ini_section::const_iterator models = it->find("use models");
		if ( models != it->end() )
		{
			int value = CTypeToString::BoolFromString(models->value);
			CBotrixMod::bUseModels = value == 0 ? false : true;
		}

		// Get player models.
		for ( int i = 0; i < CBotrixMod::aTeamsNames.size(); ++i )
		{
			sbBuffer = "models ";
			sbBuffer << CBotrixMod::aTeamsNames[i];

			good::ini_section::const_iterator modelsIt = it->find(sbBuffer);
			if ( modelsIt != it->end() )
			{
				sbBuffer = modelsIt->value;
				good::escape(sbBuffer);
				good::split((good::string)sbBuffer, m_aModels[i], ',', true);

				BLOG_D("Model names for team %s:", CBotrixMod::aTeamsNames[i].c_str());
				for ( int j = 0; j < m_aModels[i].size(); ++j )
				{
					BLOG_D("  %s", m_aModels[i][j].c_str());
				}
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------
CPlayer* CBotrixModDetail::AddBot(
	const char* szName,
	TBotIntelligence iIntelligence,
	TTeam iTeam,
	TClass /*iClass*/,
	int iParamsCount,
	const char** aParams
)
{
	if ( iParamsCount > 0 )
	{
		static constexpr size_t BUFFER_SIZE = 256;
		static char s_Buffer[BUFFER_SIZE];

		good::string_buffer sb(s_Buffer, BUFFER_SIZE, false);
		sb << "Unknown parameter: " << aParams[0];
		m_sLastError = sb;
		return NULL;
	}

	CUtlString netName = MPUtils::SanitisePlayerNetName(szName);
	edict_t* pEdict = g_engfuncs.pfnCreateFakeClient(netName.String());

	if ( !pEdict )
	{
		m_sLastError = "Error, couldn't add bot (no map or server full?).";
		return NULL;
	}

	CBot_HL2DM* result = new CBot_HL2DM(pEdict, iIntelligence);
	result->ChangeModel(iTeam);
	return result;
}

const char* CBotrixModDetail::GetLastError() const
{
	return m_sLastError.c_str();
}
