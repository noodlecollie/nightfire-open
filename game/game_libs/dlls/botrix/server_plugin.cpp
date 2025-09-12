#include "botrix/server_plugin.h"
#include "botrix/defines.h"
#include "botrix/waypoint.h"
#include "botrix/item.h"
#include "botrix/bot.h"
#include "botrix/botrixconfig.h"
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
cvar_t CBotrixServerPlugin::m_TraceLogCvar = CONSTRUCT_CVAR_T("botrix_log_trace", 0, FCVAR_PRIVILEGED);
cvar_t CBotrixServerPlugin::m_WaypointAutoAnalyzeCvar =
	CONSTRUCT_CVAR_T("botrix_waypoint_auto_analyze", 0, FCVAR_PRIVILEGED);
bool CBotrixServerPlugin::m_bSpawnedRegisterBots = false;
CBotrixBotFactory CBotrixServerPlugin::m_BotFactory;

void CBotrixServerPlugin::Init()
{
	// TODO: Move these so that they get initialised with the rest of the cvars.
	// I think that because they're here, and CBotrixServerPlugin::Init() is only
	// run when we know we're going to be running a multiplayer server, the cvars
	// don't exist when the engine parses input from the command line, so we can't
	// set them on launch.
	CVAR_REGISTER(&m_TraceLogCvar);
	CVAR_REGISTER(&m_WaypointAutoAnalyzeCvar);

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

	CConfiguration::SetClientAccessLevel("STEAM_ID_LAN", FCommandAccessAll);

	BLOG_W("Using default teams (unassigned, spectator, unknown1, unknown2).");
	CBotrixMod::aTeamsNames.push_back("unassigned");
	CBotrixMod::aTeamsNames.push_back("spectator");
	CBotrixMod::aTeamsNames.push_back("unknown1");
	CBotrixMod::aTeamsNames.push_back("unknown2");

	BLOG_W("Using default bot name (Botrix).");
	CBotrixMod::aBotNames.push_back("Botrix");

	m_pConsoleCommands = new CBotrixCommand();

	// Load mod configuration.
	CConfiguration::LoadProgrammatic();
}

void CBotrixServerPlugin::Deinit()
{
	CConfiguration::Unload();
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

	if ( m_TraceLogCvar.value != 0.0f )
	{
		CBotrixEngineUtil::iLogLevel = good::ELogLevelTrace;
	}
}

bool CBotrixServerPlugin::WaypointAutoAnalyzeEnabled()
{
	return m_WaypointAutoAnalyzeCvar.value != 0.0f;
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

	CConfiguration::RefreshWeaponConfig();

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
