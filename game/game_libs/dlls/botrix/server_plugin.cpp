#include "botrix/server_plugin.h"
#include "botrix/defines.h"
#include "botrix/waypoint.h"
#include "botrix/item.h"
#include "botrix/bot.h"
#include "enginecallback.h"
#include "standard_includes.h"
#include <good/string_utils.h>
#include "botrix/console_commands.h"
#include "types.h"
#include "weapon.h"
#include "weaponregistry.h"
#include "weaponattributes/weaponatts_collection.h"
#include "utils/mp_utils.h"
#include "botrix/botregister.h"
#include "botrix/console_vars.h"

float CBotrixServerPlugin::m_fFpsEnd = 0.0f;
int CBotrixServerPlugin::m_iFramesCount = 0;
struct edict_s* CBotrixServerPlugin::m_pListenServerClient = nullptr;
bool CBotrixServerPlugin::m_bTeamPlay = false;
bool CBotrixServerPlugin::m_bMapRunning = false;
good::string CBotrixServerPlugin::m_MapName;
int CBotrixServerPlugin::m_iFPS = 60;
float CBotrixServerPlugin::m_fTime = 0.0f;
float CBotrixServerPlugin::m_fEngineTime = 0.0f;
CBotrixCommand* CBotrixServerPlugin::m_pConsoleCommands = nullptr;
bool CBotrixServerPlugin::m_bSpawnedRegisterBots = false;
CBotrixBotFactory CBotrixServerPlugin::m_BotFactory;

static constexpr const char* const TEAM_NAME_UNASSIGNED = "unassigned";
static constexpr const char* const TEAM_NAME_SPECTATOR = "spectator";
static constexpr const char* const TEAM_NAME_MI6 = "mi6";
static constexpr const char* const TEAM_NAME_PHOENIX = "phoenix";

static TWeaponType BotWeaponTypeToBotrixWeaponType(WeaponAtts::BotWeaponType weaponType)
{
	switch ( weaponType )
	{
		case WeaponAtts::BotWeaponType::Melee:
		{
			return EWeaponMelee;
		}

		case WeaponAtts::BotWeaponType::GrenadeProjectile:
		{
			return EWeaponGrenade;
		}

		case WeaponAtts::BotWeaponType::RemoteDetonation:
		{
			return EWeaponRemoteDetonation;
		}

		case WeaponAtts::BotWeaponType::HitscanSingleShot:
		{
			return EWeaponPistol;
		}

		case WeaponAtts::BotWeaponType::HitscanContinuous:
		{
			return EWeaponRifle;
		}

		case WeaponAtts::BotWeaponType::Shotgun:
		{
			return EWeaponShotgun;
		}

		case WeaponAtts::BotWeaponType::RocketProjectile:
		{
			return EWeaponRocket;
		}

		default:
		{
			ASSERT(false);
			return EWeaponIdInvalid;
		}
	}
}

static int BotWeaponPreferenceToBotrixWeaponPreference(WeaponAtts::BotWeaponPreference weaponPreference)
{
	const int ret = static_cast<int>(weaponPreference) - static_cast<int>(WeaponAtts::BotWeaponPreference::Lowest);
	ASSERTSZ(ret < EBotIntelligenceTotal, "Expected preference to be within range of bot intelligence level");
	return ret;
}

static int BotWeaponTeamRestrictionToBotrixTeamIndex(WeaponAtts::BotWeaponTeamRestriction weaponTeamRestriction)
{
	int teamIndex = -1;

	switch ( weaponTeamRestriction )
	{
		case WeaponAtts::BotWeaponTeamRestriction::MI6:
		{
			teamIndex = CBotrixMod::GetTeamIndex(TEAM_NAME_MI6);
			break;
		}

		case WeaponAtts::BotWeaponTeamRestriction::Phoenix:
		{
			teamIndex = CBotrixMod::GetTeamIndex(TEAM_NAME_PHOENIX);
			break;
		}

		default:
		{
			ASSERT(false);
			teamIndex = CBotrixMod::GetTeamIndex(TEAM_NAME_UNASSIGNED);
			break;
		}
	}

	ASSERT(teamIndex >= 0);
	return teamIndex;
}

void CBotrixServerPlugin::Init()
{
	good::log::bLogToStdOut = false;  // Disable log to stdout, Msg() will print there.
	good::log::bLogToStdErr = false;  // Disable log to stderr, Warning() will print there.
	good::log::iStdErrLevel = good::ELogLevelWarning;  // Log warnings and errors to stderr.
	CBotrixEngineUtil::iLogLevel = good::ELogLevelTrace;
	good::log::iLogLevel = good::ELogLevelTrace;  // Trace before loading config.ini

	// Get game/mod and botrix base directories.
	good::string sIniPath;

	BLOG_W("Using default mod config without items/weapons.");
	CBotrixMod::sModName = "nightfire_open";
	CBotrixEngineUtil::iLogLevel = good::ELogLevelInfo;
	good::log::iFileLogLevel = good::ELogLevelDebug;

	BLOG_W("Using default teams (unassigned, spectator, unknown1, unknown2).");
	CBotrixMod::aTeamsNames.push_back("unassigned");
	CBotrixMod::aTeamsNames.push_back("spectator");
	CBotrixMod::aTeamsNames.push_back("unknown1");
	CBotrixMod::aTeamsNames.push_back("unknown2");

	BLOG_W("Using default bot name (Botrix).");
	CBotrixMod::aBotNames.push_back("Botrix");

	m_pConsoleCommands = new CBotrixCommand();

	LoadItemsAndWeapons();
}

void CBotrixServerPlugin::Deinit()
{
	CBotrixMod::UnLoad();
	CPlayers::Clear();

	delete m_pConsoleCommands;
	m_pConsoleCommands = nullptr;
}

void CBotrixServerPlugin::ServerActivate()
{
	BLOG_I("Botrix ServerActivate");
	PrepareLevel();
	ActivateLevel();
}

void CBotrixServerPlugin::ServerDeactivate()
{
	BLOG_I("Botrix ServerDeactivate");
	m_bMapRunning = false;
	m_MapName = "";
	m_bTeamPlay = false;
	m_bSpawnedRegisterBots = false;
}

void CBotrixServerPlugin::ClientDisconnect(struct edict_s* entity)
{
	if ( m_bMapRunning )
	{
		CPlayers::PlayerDisconnected(entity);
	}

	if ( entity == m_pListenServerClient )
	{
		m_pListenServerClient = nullptr;
	}
}

void CBotrixServerPlugin::ClientPutInServer(struct edict_s* entity)
{
	if ( !entity )
	{
		return;
	}

	if ( !IS_DEDICATED_SERVER() && !m_pListenServerClient )
	{
		m_pListenServerClient = entity;
	}

	if ( m_bMapRunning )
	{
		int iIdx = CPlayers::GetIndex(entity);
		GoodAssert(iIdx >= 0);

		CPlayers::PlayerConnected(entity);
		CBotrixMod::AddFrameEvent(iIdx, EFrameEventActivated);
	}
}

bool CBotrixServerPlugin::ClientCommand(CBasePlayer* /* player */, const char* /* command */)
{
	// TODO
	return false;
}

void CBotrixServerPlugin::ClientTeamChange(struct edict_s* entity, int newTeam)
{
	if ( m_bMapRunning )
	{
		int iIdx = CPlayers::GetIndex(entity);
		GoodAssert(iIdx >= 0);

		CPlayer* pPlayer = CPlayers::Get(iIdx);

		if ( pPlayer && pPlayer->IsBot() )
		{
			((CBotrixBot*)pPlayer)->ChangeTeam(newTeam);
		}
	}
}

void CBotrixServerPlugin::ClientRespawn(struct edict_s* entity)
{
	if ( m_bMapRunning )
	{
		int iIdx = CPlayers::GetIndex(entity);
		GoodAssert(iIdx >= 0);

		CBotrixMod::AddFrameEvent(iIdx, EFrameEventRespawned);
	}
}

void CBotrixServerPlugin::ClientHurt(struct edict_s* victim, struct edict_s* attacker)
{
	if ( m_bMapRunning )
	{
		int iVictim = CPlayers::GetIndex(victim);
		BASSERT(iVictim >= 0, return);

		int iAttacker = attacker ? CPlayers::GetIndex(attacker) : iVictim;  // May hurt himself.

		CPlayer* pPlayer = CPlayers::Get(iVictim);
		CPlayer* pPlayerAttacker = iAttacker >= 0 ? CPlayers::Get(iAttacker) : nullptr;

		if ( pPlayer && pPlayerAttacker && pPlayer->IsBot() )
		{
			((CBotrixBot*)pPlayer)->HurtBy(iAttacker, pPlayerAttacker, static_cast<int>(victim->v.health));
		}
	}
}

void CBotrixServerPlugin::ClientKilled(struct edict_s* victim, struct edict_s* killer)
{
	if ( m_bMapRunning )
	{
		int iVictim = CPlayers::GetIndex(victim);
		BASSERT(iVictim >= 0, return);

		CPlayer* pPlayerActivator = CPlayers::Get(iVictim);

		if ( pPlayerActivator )
		{
			pPlayerActivator->Dead();
		}

		if ( killer )
		{
			int iAttacker = CPlayers::GetIndex(killer);

			// Attacker may not be a valid player if it was the world that killed the victim.

			if ( iAttacker >= 0 )
			{
				CPlayer* pPlayerAttacker = CPlayers::Get(iAttacker);

				if ( pPlayerAttacker && pPlayerAttacker->IsBot() )
				{
					((CBotrixBot*)pPlayerAttacker)->KilledEnemy(iVictim, pPlayerActivator);
				}
			}
		}
	}
}

void CBotrixServerPlugin::Think()
{
	CBotrixServerPlugin::UpdateLogLevel();

	if ( !m_bMapRunning )
	{
		return;
	}

	if ( CWaypoints::IsAnalyzing() )
	{
		CWaypoints::AnalyzeStep();
	}

	float fPrevEngineTime = m_fEngineTime;
	m_fEngineTime = gpGlobals->time;

	float fDiff = m_fEngineTime - fPrevEngineTime;

#if defined(_DEBUG)
	if ( fDiff > 1.0f )  // Too low fps, possibly debugging.
	{
		m_fTime += 0.1f;
	}
	else
	{
		m_fTime += fDiff;
	}
#else
	m_fTime += fDiff;
#endif

	// We do this here for now, as if a real player is in the game then we know it's safe
	// to spawn bots. If we find a way of determining when it's safe otherwise, move this
	// logic there instead.
	if ( !m_bSpawnedRegisterBots )
	{
		for ( int clientIndex = 1; clientIndex <= gpGlobals->maxClients; ++clientIndex )
		{
			CBasePlayer* player = MPUtils::CBasePlayerFromIndex(clientIndex);

			if ( !player )
			{
				continue;
			}

			if ( player->IsNetClient() && player->IsAlive() )
			{
				SpawnBotsInRegister();
				m_bSpawnedRegisterBots = true;
				break;
			}
		}
	}

	CBotrixMod::Think();

	CItems::Update();
	CPlayers::PreThink();

#ifdef BOTRIX_SHOW_PERFORMANCE
	static const float fInterval = 10.0f;  // Print & reset every 10 seconds.
	static float fStart = 0.0f, fSum = 0.0f;
	static int iCount = 0;

	if ( fStart == 0.0f )
		fStart = fEngineTime;

	fSum += pEngineServer->Time() - fEngineTime;
	iCount++;

	if ( (fStart + fInterval) <= pEngineServer->Time() )
	{
		BLOG_T(
			"Botrix think time in %d frames (%.0f seconds): %.5f msecs",
			iCount,
			fInterval,
			fSum / (float)iCount * 1000.0f
		);
		fStart = fSum = 0.0f;
		iCount = 0;
	}
#endif
}

edict_t* CBotrixServerPlugin::GetListenServerClient()
{
	return m_pListenServerClient;
}

float CBotrixServerPlugin::GetTime()
{
	return m_fTime;
}

bool CBotrixServerPlugin::MapIsRunning()
{
	return m_bMapRunning;
}

const char* CBotrixServerPlugin::MapName()
{
	return m_MapName.c_str();
}

CBotrixCommand* CBotrixServerPlugin::GetConsoleCommandHandler()
{
	return m_pConsoleCommands;
}

void CBotrixServerPlugin::UpdateLogLevel()
{
	static cvar_t* developerCvar = nullptr;

	if ( !developerCvar )
	{
		developerCvar = CVAR_GET_POINTER("developer");
	}

	ASSERT(developerCvar);

	const int devLevel = static_cast<int>(developerCvar->value);

	switch ( devLevel )
	{
		case 4:
		case 5:
		{
			CBotrixEngineUtil::iLogLevel = good::ELogLevelDebug;
			break;
		}

		default:
		{
			CBotrixEngineUtil::iLogLevel = good::ELogLevelInfo;
			break;
		}
	}

	if ( CBotrixCvars::botrix_log_trace.value != 0.0f )
	{
		CBotrixEngineUtil::iLogLevel = good::ELogLevelTrace;
	}
}

bool CBotrixServerPlugin::WaypointAutoAnalyzeEnabled()
{
	return CBotrixCvars::botrix_waypoint_auto_analyze.value != 0.0f;
}

void CBotrixServerPlugin::RefreshWeaponConfig()
{
	CWeapons::Clear();
	CWeaponRegistry::StaticInstance().ForEach(&LoadWeapon);
}

void CBotrixServerPlugin::PrepareLevel()
{
	m_MapName.assign(STRING(gpGlobals->mapname), good::string::npos, true);
	good::lower_case(m_MapName);

	const float teamplay = CVAR_GET_FLOAT("mp_teamplay");
	m_bTeamPlay = teamplay != 0.0f;

	m_bSpawnedRegisterBots = false;
	m_BotFactory.LoadBotProfiles();
}

void CBotrixServerPlugin::ActivateLevel()
{
	m_bMapRunning = true;

	const float maxPlayers = CVAR_GET_FLOAT("maxplayers");
	CPlayers::Init(static_cast<int>(maxPlayers));

	RefreshWeaponConfig();

	// Waypoints should be loaded after CPlayers::Size() is known.
	if ( CWaypoints::Load() )
	{
		BLOG_I("%d waypoints loaded for map %s.", CWaypoints::Size(), m_MapName.c_str());
	}

	// Waypoints can save object, so waypoints should be loaded first.
	CItems::MapLoaded();

	// May depend on items / waypoints.
	CBotrixMod::MapLoaded();

	BLOG_I("Level \"%s\" has been loaded.", m_MapName.c_str());

	if ( WaypointAutoAnalyzeEnabled() && CWaypoints::Size() <= CWaypoint::iWaypointsMaxCountToAnalyzeMap )
	{
		BLOG_I("Map has %d waypoints, running waypoint analyze to create more", CWaypoints::Size());
		CWaypoints::Analyze(NULL, false);
	}
}

void CBotrixServerPlugin::SpawnBotsInRegister()
{
	CBotRegister& reg = CBotRegister::StaticInstance();

	for ( uint32_t index = 0; index < reg.Count(); ++index )
	{
		const CUtlString profileName = reg.ProfileName(index);
		const CUtlString customName = reg.CustomName(index);

		m_BotFactory.TryCreateBot(profileName, customName);
	}
}

void CBotrixServerPlugin::LoadItemsAndWeapons()
{
	static const int respawnableFlags = CTypeToString::EntityClassFlagsFromString("respawnable");

	CBotrixServerPlugin::UpdateLogLevel();

	// TODO: Pull these values out and set them in a readable way.
	CBotrixMod::LoadDefaults();

	CBotrixMod::aTeamsNames.push_back(TEAM_NAME_UNASSIGNED);
	CBotrixMod::aTeamsNames.push_back(TEAM_NAME_SPECTATOR);
	CBotrixMod::aTeamsNames.push_back(TEAM_NAME_MI6);
	CBotrixMod::aTeamsNames.push_back(TEAM_NAME_PHOENIX);

	CBotrixMod::iUnassignedTeam = CBotrixMod::GetTeamIndex(TEAM_NAME_UNASSIGNED);
	CBotrixMod::iSpectatorTeam = CBotrixMod::GetTeamIndex(TEAM_NAME_SPECTATOR);

	CItems::AddItemClassFor(EItemTypePlayerSpawn, CItemClass("info_player_deathmatch", 0));
	CItems::AddItemClassFor(EItemTypePlayerSpawn, CItemClass("info_player_coop", 0));

	CItems::AddItemClassFor(EItemTypeArmor, CItemClass("item_battery", respawnableFlags));
	CItems::AddItemClassFor(EItemTypeHealth, CItemClass("item_healthkit", respawnableFlags));

	CItems::AddItemClassFor(EItemTypeLadder, CItemClass("func_ladder", FEntityNone));

	RefreshWeaponConfig();

	// Very important!
	CBotrixMod::Prepare();
}

void CBotrixServerPlugin::LoadWeapon(const WeaponAtts::WACollection& atts)
{
	static const int respawnableFlags = CTypeToString::EntityClassFlagsFromString("respawnable");

	const WeaponAtts::WABotInterface& botIfc = atts.BotInterface;

	if ( botIfc.Preference == WeaponAtts::BotWeaponPreference::Never )
	{
		// Ignore this weapon.
		return;
	}

	good::unique_ptr<CWeapon> pWeapon(new CWeapon());

	pWeapon->iId = CWeapons::Size();
	pWeapon->pWeaponClass = CItems::GetItemClass(EItemTypeWeapon, atts.Core.Classname);

	if ( !pWeapon->pWeaponClass )
	{
		CItemClass cEntityClass;
		cEntityClass.sClassName = atts.Core.Classname;
		pWeapon->pWeaponClass = CItems::AddItemClassFor(EItemTypeWeapon, cEntityClass);
	}

	GoodAssert(pWeapon->pWeaponClass);

	// Ignore all class restrictions since we don't support them.
	pWeapon->iClass = ~0;

	pWeapon->iType = BotWeaponTypeToBotrixWeaponType(botIfc.Type);
	pWeapon->iBotPreference = BotWeaponPreferenceToBotrixWeaponPreference(botIfc.Preference);
	pWeapon->fHolsterTime = 0.0f;  // Currently unused

	int teamIndex = -1;

	if ( botIfc.TeamRestriction != WeaponAtts::BotWeaponTeamRestriction::None )
	{
		teamIndex = BotWeaponTeamRestrictionToBotrixTeamIndex(botIfc.TeamRestriction);
	}

	if ( teamIndex >= 0 )
	{
		// Make sure the unassigned (deathmatch) team can always use the weapon.
		pWeapon->iTeam = (1 << teamIndex) | (1 << CBotrixMod::iUnassignedTeam);
	}
	else
	{
		pWeapon->iTeam = ~0;
	}

	// Deal with ammo first, so that it's present for the attack modes.
	const WeaponAtts::WAAmmoDef& ammo = atts.Ammo;

	for ( size_t ammoIndex = 0; ammoIndex < SIZE_OF_ARRAY(ammo.PickupClassnames); ++ammoIndex )
	{
		const char* ammoPickupClassName = ammo.PickupClassnames[ammoIndex];

		if ( ammoPickupClassName && *ammoPickupClassName && !CItems::GetItemClass(EItemTypeAmmo, ammoPickupClassName) )
		{
			CItems::AddItemClassFor(EItemTypeAmmo, CItemClass(ammoPickupClassName, respawnableFlags));
		}
	}

	for ( size_t modeIndex = 0; modeIndex < 2; ++modeIndex )
	{
		const WeaponAtts::WABotAttackMode* attackMode =
			modeIndex == 0 ? botIfc.PrimaryAttackMode.get() : botIfc.SecondaryAttackMode.get();

		// Mode 0 must be valid.
		ASSERT(attackMode || modeIndex != 0);

		if ( !attackMode )
		{
			continue;
		}

		pWeapon->iClipSize[modeIndex] = static_cast<unsigned char>(attackMode->ClipSize);
		pWeapon->fDamage[modeIndex] = gSkillData.*(attackMode->BaseDamagePerShot);

		// Hold time: amount of time to hold the button down for.
		// Shot time: amount of time after releasing to wait before pressing again.
		const float delayPerShot = (1.0f / attackMode->AttackRate);

		if ( attackMode->AttackButtonPressTime > 0.9f * delayPerShot )
		{
			pWeapon->fHoldTime[modeIndex] = 0.9f * delayPerShot;
			pWeapon->fShotTime[modeIndex] = 0.1f * delayPerShot;
		}
		else
		{
			pWeapon->fHoldTime[modeIndex] = attackMode->AttackButtonPressTime;
			pWeapon->fShotTime[modeIndex] = delayPerShot - attackMode->AttackButtonPressTime;
		}

		// Add some extra delay so the bot doesn't refire inhumanly fast.
		pWeapon->fShotTime[modeIndex] += attackMode->MinExtraDelayBetweenShots;

		pWeapon->iReloadBy[modeIndex] = attackMode->ReloadStyle == WeaponAtts::BotWeaponReloadStyle::PerClip
			? static_cast<unsigned char>(attackMode->ClipSize)
			: 1;

		pWeapon->fReloadTime[modeIndex] = attackMode->ReloadDuration;
		pWeapon->fReloadStartTime[modeIndex] = attackMode->ReloadStartDelay;
		pWeapon->iDefaultAmmo[modeIndex] = static_cast<unsigned char>(attackMode->DefaultAmmoOnFirstPickup);
		pWeapon->iMaxAmmo[modeIndex] = static_cast<unsigned char>(attackMode->MaxAmmo);
		pWeapon->iAttackBullets[modeIndex] = static_cast<unsigned char>(attackMode->AmmoDecrementPerAttackCycle);
		pWeapon->fMinDistanceSqr[modeIndex] = attackMode->MinEffectiveRange * attackMode->MinEffectiveRange;
		pWeapon->fMaxDistanceSqr[modeIndex] = attackMode->MaxEffectiveRange * attackMode->MaxEffectiveRange;
		pWeapon->iParabolicDistance0[modeIndex] = static_cast<int>(attackMode->ParabolicDistanceAt0Degrees);
		pWeapon->iParabolicDistance45[modeIndex] = static_cast<int>(attackMode->ParabolicDistanceAt45Degrees);

		if ( pWeapon->fMaxDistanceSqr[modeIndex] <= 0.0f )
		{
			pWeapon->fMaxDistanceSqr[modeIndex] = CBotrixEngineUtil::iMaxMapSizeSqr;
		}
		else if ( pWeapon->fMaxDistanceSqr[modeIndex] <= pWeapon->fMinDistanceSqr[modeIndex] )
		{
			pWeapon->fMaxDistanceSqr[modeIndex] = pWeapon->fMinDistanceSqr[modeIndex];
		}

		// NFTODO: Support separate ammo for secondary attack mode?
		for ( size_t index = 0; index < SIZE_OF_ARRAY(ammo.PickupClassnames); ++index )
		{
			const char* pickupClassname = ammo.PickupClassnames[index];

			if ( !pickupClassname || !(*pickupClassname) )
			{
				continue;
			}

			const CItemClass* pAmmoClass = CItems::GetItemClass(EItemTypeAmmo, pickupClassname);
			ASSERT(pAmmoClass);

			pWeapon->aAmmos[modeIndex].push_back(pAmmoClass);
			pWeapon->aAmmoBullets[modeIndex].push_back(attackMode->AmmoGivenOnPickup);
		}
	}

	if ( botIfc.PrimaryAttackMode && botIfc.SecondaryAttackMode )
	{
		pWeapon->iFlags[1] |= FWeaponSameBullets;
	}

	// Sanity:
	switch ( pWeapon->iType )
	{
		case EWeaponMelee:
		case EWeaponPhysics:
		{
			pWeapon->iAttackBullets[0] = 0;
			pWeapon->iAttackBullets[1] = 0;
			break;
		}

		case EWeaponRocket:
		case EWeaponGrenade:
		case EWeaponRemoteDetonation:
		{
			pWeapon->iClipSize[0] = 1;
			break;
		}

		default:
		{
			break;
		}
	}

	CWeapons::Add(CWeaponWithAmmo(pWeapon.release()));
}
