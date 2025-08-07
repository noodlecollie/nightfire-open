#include "spawnpointmanager.h"
#include "game.h"

namespace
{
	static constexpr float SPAWN_POINT_CLEAR_RADIUS = 128.0f;

	inline CBaseEntity* NullSpawnPoint()
	{
		return GetClassPtrFromEdict<CBaseEntity>(INDEXENT(0));
	}
}  // namespace

const char* const CSpawnPointManager::SpawnPointClassNames[SP__Count] = {
	"info_player_start",
	"info_player_deathmatch",
	"info_player_coop"};

void CSpawnPointManager::Initialise()
{
	for ( uint32_t index = 0; index < SP__Count; ++index )
	{
		CSpawnPointList& list = m_Lists[index];
		const char* className = SpawnPointClassNames[index];

		if ( index == SP_Default )
		{
			// See if there is an override.
			if ( !FStringNull(gpGlobals->startspot) )
			{
				const char* overrideString = STRING(gpGlobals->startspot);

				if ( overrideString && *overrideString )
				{
					className = overrideString;
				}
			}
		}

		list.Initialise(className);
		list.RandomiseIndices();
	}
}

CBaseEntity* CSpawnPointManager::GetNextSpawnPoint(CBasePlayer* player, SpawnPointType spType, uint32_t flags)
{
	CBaseEntity* spawnPoint = nullptr;
	const char* spawnPointClassName = "";

	switch ( spType )
	{
		case SP_Deathmatch:
		{
			spawnPoint = GetNextDeathmatchSpawnPoint(player, flags);
			spawnPointClassName = SpawnPointClassNames[SP_Deathmatch];
			break;
		}
		case SP_CoOp:
		{
			spawnPoint = GetNextCoOpSpawnPoint();
			spawnPointClassName = SpawnPointClassNames[SP_CoOp];
			break;
		}
		default:
		{
			spawnPoint = GetNextDefaultSpawnPoint();
			spawnPointClassName = SpawnPointClassNames[SP_Default];
			break;
		}
	}

	if ( !spawnPoint )
	{
		ALERT(at_error, "Could not find any appropriate spawn points (%s) in map.\n", spawnPointClassName);
		spawnPoint = NullSpawnPoint();
	}

	return spawnPoint;
}

CBaseEntity* CSpawnPointManager::GetNextCoOpSpawnPoint()
{
	CBaseEntity* spawnPoint = m_Lists[SP_CoOp].GetNextSpawnPoint();
	return spawnPoint ? spawnPoint : GetNextDefaultSpawnPoint();
}

CBaseEntity* CSpawnPointManager::GetNextDeathmatchSpawnPoint(CBasePlayer* player, uint32_t flags)
{
	CSpawnPointList& list = m_Lists[SP_Deathmatch];
	bool killPlayersAtSpawnPoint = (flags & Flag_IgnoreNearbyPlayers);

	CBaseEntity* chosenSpawnPoint = nullptr;

	if ( list.SpawnPointCount() > 0 )
	{
		chosenSpawnPoint = GetNextValidDeathmatchSpawnPoint(player, flags);

		if ( !chosenSpawnPoint && !killPlayersAtSpawnPoint )
		{
			killPlayersAtSpawnPoint = true;
			chosenSpawnPoint = GetNextValidDeathmatchSpawnPoint(player, flags | Flag_IgnoreNearbyPlayers);
		}
	}
	else
	{
		chosenSpawnPoint = GetNextDefaultSpawnPoint();

		if ( !chosenSpawnPoint )
		{
			chosenSpawnPoint = NullSpawnPoint();
		}

		// Always kill here, to be safe.
		// If we can't find any deathmatch spawns, something is wrong with the map anyway.
		killPlayersAtSpawnPoint = true;
	}

	if ( chosenSpawnPoint && killPlayersAtSpawnPoint )
	{
		KillPlayersAtPoint(chosenSpawnPoint->pev->origin, player);
	}

	return chosenSpawnPoint;
}

CBaseEntity* CSpawnPointManager::GetNextValidDeathmatchSpawnPoint(CBasePlayer* player, uint32_t flags)
{
	CSpawnPointList& list = m_Lists[SP_Deathmatch];
	const size_t spawnPointCount = list.SpawnPointCount();

	// We may not want to choose a spawn point too near where the player just died,
	// to avoid them continually getting killed by an ongoing firefight.
	// This variable holds the first spawn point that was found to be valid but too
	// close to the player's death site - we fall back on this spawn point if we
	// can't find any better ones.
	CBaseEntity* spawnPointNearDeathSite = nullptr;

	// We only use this to count the number of spawn points we've checked.
	// We still want to rely on GetNextSpawnPoint() to get the actual entity,
	// since this will retain the last index we got to on a previous call.
	for ( size_t index = 0; index < spawnPointCount; ++index )
	{
		CBaseEntity* spawnPoint = list.GetNextSpawnPoint();

		if ( !SpawnPointValid(spawnPoint, player, flags) )
		{
			continue;
		}

		if ( (flags & Flag_AvoidDeathSite) && !spawnPointNearDeathSite && mp_respawn_avoid_radius.value > 0.0f )
		{
			Vector displacement = Vector(spawnPoint->pev->origin) - Vector(player->pev->origin);

			if ( displacement.Length() <= mp_respawn_avoid_radius.value )
			{
				spawnPointNearDeathSite = spawnPoint;
				continue;
			}
		}

		// Spawn point passed all checks, so we can use it.
		return spawnPoint;
	}

	// If we found a valid spawn point but it was too near to the player's death site,
	// it will be held in this. If we didn't find any fallback point, this will be
	// null (which we would have returned anyway).
	return spawnPointNearDeathSite;
}

CBaseEntity* CSpawnPointManager::GetNextDefaultSpawnPoint()
{
	return m_Lists[SP_Default].GetNextSpawnPoint();
}

bool CSpawnPointManager::SpawnPointValid(CBaseEntity* spawnPoint, CBasePlayer* player, uint32_t flags)
{
	if ( !spawnPoint || !spawnPoint->IsTriggered(player) )
	{
		return false;
	}

	if ( !(flags & Flag_IgnoreNearbyPlayers) )
	{
		for ( CBaseEntity* entInSphere =
				  UTIL_FindEntityInSphere(nullptr, spawnPoint->pev->origin, SPAWN_POINT_CLEAR_RADIUS);
			  !FNullEnt(entInSphere);
			  entInSphere = UTIL_FindEntityInSphere(entInSphere, spawnPoint->pev->origin, SPAWN_POINT_CLEAR_RADIUS) )
		{
			if ( entInSphere->IsPlayer() && static_cast<CBasePlayer*>(entInSphere) != player )
			{
				return false;
			}
		}
	}

	return true;
}

void CSpawnPointManager::KillPlayersAtPoint(const vec3_t& origin, CBasePlayer* player)
{
	for ( CBaseEntity* entInSphere = UTIL_FindEntityInSphere(nullptr, origin, SPAWN_POINT_CLEAR_RADIUS);
		  !FNullEnt(entInSphere);
		  entInSphere = UTIL_FindEntityInSphere(entInSphere, origin, SPAWN_POINT_CLEAR_RADIUS) )
	{
		if ( entInSphere->IsPlayer() )
		{
			CBasePlayer* otherPlayer = static_cast<CBasePlayer*>(entInSphere);

			if ( otherPlayer != player && otherPlayer->pev->deadflag == DEAD_NO )
			{
				entvars_t* worldVars = VARS(INDEXENT(0));
				entInSphere->TakeDamage(worldVars, worldVars, entInSphere->pev->max_health, DMG_GENERIC);
			}
		}
	}
}
