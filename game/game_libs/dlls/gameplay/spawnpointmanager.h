#pragma once

#include <cstdint>
#include "standard_includes.h"
#include "spawnpointlist.h"

class CSpawnPointManager
{
public:
	enum SpawnFlag
	{
		Flag_AvoidDeathSite = 1 << 0,
		Flag_IgnoreNearbyPlayers = 1 << 1
	};

	enum SpawnPointType
	{
		SP_Default = 0,
		SP_Deathmatch,
		SP_CoOp,

		SP__Count
	};

	void Initialise();

	CBaseEntity* GetNextSpawnPoint(CBasePlayer* player, SpawnPointType spType = SP_Default, uint32_t flags = 0);

private:
	static bool SpawnPointValid(CBaseEntity* spawnPoint, CBasePlayer* player, uint32_t flags);
	static void KillPlayersAtPoint(const vec3_t& origin, CBasePlayer* player);

	CBaseEntity* GetNextCoOpSpawnPoint();
	CBaseEntity* GetNextDeathmatchSpawnPoint(CBasePlayer* player, uint32_t flags);
	CBaseEntity* GetNextDefaultSpawnPoint();
	CBaseEntity* GetNextValidDeathmatchSpawnPoint(CBasePlayer* player, uint32_t flags);

	static const char* const SpawnPointClassNames[SP__Count];

	CSpawnPointList m_Lists[SP__Count];
};
