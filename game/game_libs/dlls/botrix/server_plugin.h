#pragma once

#include "standard_includes.h"
#include <good/string.h>

struct edict_s;
class CBasePlayer;
class CBotrixCommand;

// TODO: Convert this into a non-static class.
class CBotrixServerPlugin
{
public:
	static void Init();
	static void Deinit();

	static void ServerActivate();
	static void ServerDeactivate();
	static void ClientDisconnect(struct edict_s* entity);
	static void ClientPutInServer(struct edict_s* entity);
	static bool ClientCommand(CBasePlayer* player, const char* command);
	static void ClientTeamChange(struct edict_s* entity, int newTeam);
	static void ClientRespawn(struct edict_s* entity);
	static void ClientHurt(struct edict_s* victim, struct edict_s* attacker);
	static void ClientKilled(struct edict_s* victim, struct edict_s* killer);
	static void Think();

	static struct edict_s* GetListenServerClient();
	static float GetTime();
	static bool MapIsRunning();
	static const char* MapName();
	static CBotrixCommand* GetConsoleCommandHandler();

	static void UpdateLogLevel();

private:
	static void PrepareLevel();
	static void ActivateLevel();
	static void SpawnBotsInRegister();

	static float m_fFpsEnd;  ///< Time of ending counting frames to calculate fps.
	static int m_iFramesCount;  ///< Count of frames since m_fFpsStart.

	static struct edict_s* m_pListenServerClient;

	static bool m_bTeamPlay;  ///< True if game is team based (like Counter-Strike), if false then it is deathmatch.
	static bool m_bMapRunning;  ///< True if map is currently running (LevelInit() was called by Source engine).
	static good::string m_MapName;  ///< Current map name (set at LevelInit()).

	static int m_iFPS;  ///< Frames per second. Updated each second.
	static float m_fTime;  ///< Current time.
	static float m_fEngineTime;  ///< Current engine time.

	static CBotrixCommand* m_pConsoleCommands;
	static cvar_t m_TraceLogCvar;

	static bool m_bSpawnedRegisterBots;
};
