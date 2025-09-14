#include "botrix/botrixplayerinfo.h"
#include "standard_includes.h"
#include "gamerules.h"
#include "weapons.h"
#include "botrix/engine_util.h"
#include "botrix/mod.h"

namespace Botrix
{
	CPlayerInfo::CPlayerInfo(struct edict_s* player) :
		m_pPlayer(player)
	{
	}

	CBasePlayer* CPlayerInfo::GetPlayerEntity() const
	{
		if ( !m_pPlayer )
		{
			return nullptr;
		}

		CBasePlayer* playerEnt = dynamic_cast<CBasePlayer*>(CBaseEntity::Instance(m_pPlayer));
		ASSERT(playerEnt);

		return playerEnt;
	}

	int CPlayerInfo::GetPlayerEdictIndex() const
	{
		return ENTINDEX(m_pPlayer);
	}

	const char* CPlayerInfo::GetName() const
	{
		if ( !m_pPlayer )
		{
			return nullptr;
		}

		return STRING(m_pPlayer->v.netname);
	}

	int CPlayerInfo::GetUserID() const
	{
		return GETPLAYERUSERID(m_pPlayer);
	}

	const char* CPlayerInfo::GetNetworkIDString() const
	{
		return GETPLAYERAUTHID(m_pPlayer);
	}

	int CPlayerInfo::GetTeamIndex() const
	{
		if ( !m_pPlayer || !g_pGameRules )
		{
			return -1;
		}

		CBasePlayer* player = GetPlayerEntity();

		if ( !player )
		{
			return -1;
		}

		if ( !g_pGameRules->IsTeamplay() )
		{
			// Return the deathmatch team
			return CMod::iUnassignedTeam;
		}

		return g_pGameRules->GetTeamIndex(player->TeamID());
	}

	void CPlayerInfo::ChangeTeam(int newTeamIndex)
	{
		if ( !m_pPlayer || !g_pGameRules )
		{
			return;
		}

		g_pGameRules->ChangePlayerTeam(GetPlayerEntity(), g_pGameRules->GetIndexedTeamName(newTeamIndex), false, false);
	}

	int CPlayerInfo::GetFragCount() const
	{
		if ( !m_pPlayer )
		{
			return 0;
		}

		return static_cast<int>(m_pPlayer->v.frags);
	}

	int CPlayerInfo::GetDeathCount() const
	{
		if ( !m_pPlayer )
		{
			return 0;
		}

		CBasePlayer* player = GetPlayerEntity();

		if ( !player )
		{
			return 0;
		}

		return player->m_iDeaths;
	}

	bool CPlayerInfo::IsConnected() const
	{
		// Not sure if this is the most canonical way of checking this, but it's
		// the method used in the multiplayer gamerules, and I can't find an
		// official flag or anything for it.
		return m_pPlayer && (m_pPlayer->v.netname && (STRING(m_pPlayer->v.netname))[0] != 0);
	}

	int CPlayerInfo::GetArmorValue() const
	{
		return m_pPlayer ? static_cast<int>(m_pPlayer->v.armorvalue) : 0;
	}

	bool CPlayerInfo::IsHLTV() const
	{
		return g_engfuncs.pfnIsHLTVClient(m_pPlayer);
	}

	bool CPlayerInfo::IsPlayer() const
	{
		// Do this manually to avoid accidentally failing the assertion
		return m_pPlayer && dynamic_cast<CBasePlayer*>(CBaseEntity::Instance(m_pPlayer));
	}

	bool CPlayerInfo::IsFakeClient() const
	{
		if ( !m_pPlayer )
		{
			return false;
		}

		return (m_pPlayer->v.flags & FL_FAKECLIENT) != 0;
	}

	bool CPlayerInfo::IsDead() const
	{
		if ( !m_pPlayer )
		{
			return false;
		}

		// This is essentially !IsAlive(), which is what the bot code needs.
		return m_pPlayer->v.deadflag != DEAD_NO;
	}

	bool CPlayerInfo::IsInAVehicle() const
	{
		return false;
	}

	bool CPlayerInfo::IsObserver() const
	{
		if ( !m_pPlayer )
		{
			return false;
		}

		CBasePlayer* player = GetPlayerEntity();

		if ( !player )
		{
			return false;
		}

		return player->IsObserver();
	}

	Vector CPlayerInfo::GetAbsOrigin() const
	{
		if ( !m_pPlayer )
		{
			return Vector();
		}

		return m_pPlayer->v.origin;
	}

	Vector CPlayerInfo::GetEyePosition() const
	{
		Vector out;
		CBotrixEngineUtil::EntityHead(m_pPlayer, out);
		return out;
	}

	Vector CPlayerInfo::GetAbsAngles() const
	{
		if ( !m_pPlayer )
		{
			return Vector();
		}

		return m_pPlayer->v.angles;
	}

	Vector CPlayerInfo::GetEyeAngles() const
	{
		if ( !m_pPlayer )
		{
			return Vector();
		}

		return m_pPlayer->v.v_angle;
	}

	Vector CPlayerInfo::GetPlayerMins() const
	{
		if ( !m_pPlayer )
		{
			return Vector();
		}

		return m_pPlayer->v.mins;
	}

	Vector CPlayerInfo::GetPlayerMaxs() const
	{
		if ( !m_pPlayer )
		{
			return Vector();
		}

		return m_pPlayer->v.maxs;
	}

	const char* CPlayerInfo::GetWeaponName() const
	{
		if ( !m_pPlayer )
		{
			return nullptr;
		}

		CBasePlayer* player = GetPlayerEntity();

		if ( !player || !player->m_pActiveItem )
		{
			return nullptr;
		}

		return STRING(player->m_pActiveItem->pev->classname);
	}

	const char* CPlayerInfo::GetModelName() const
	{
		if ( !m_pPlayer )
		{
			return nullptr;
		}

		return STRING(m_pPlayer->v.model);
	}

	int CPlayerInfo::GetHealth() const
	{
		if ( !m_pPlayer )
		{
			return 0;
		}

		return static_cast<int>(m_pPlayer->v.health);
	}

	int CPlayerInfo::GetMaxHealth() const
	{
		if ( !m_pPlayer )
		{
			return 0;
		}

		return static_cast<int>(m_pPlayer->v.max_health);
	}

	bool CPlayerInfo::IsReplay() const
	{
		return false;
	}
}  // namespace Botrix
