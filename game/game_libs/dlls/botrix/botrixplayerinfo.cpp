#include "botrix/botrixplayerinfo.h"
#include "standard_includes.h"
#include "gamerules.h"
#include "weapons.h"
#include "botrix/engine_util.h"
#include "botrix/mod.h"

namespace Botrix
{
	CBotrixPlayerInfo::CBotrixPlayerInfo(struct edict_s* player) :
		m_pPlayer(player)
	{
	}

	CBasePlayer* CBotrixPlayerInfo::GetPlayerEntity() const
	{
		if ( !m_pPlayer )
		{
			return nullptr;
		}

		CBasePlayer* playerEnt = dynamic_cast<CBasePlayer*>(CBaseEntity::Instance(m_pPlayer));
		ASSERT(playerEnt);

		return playerEnt;
	}

	int CBotrixPlayerInfo::GetPlayerEdictIndex() const
	{
		return ENTINDEX(m_pPlayer);
	}

	const char* CBotrixPlayerInfo::GetName() const
	{
		if ( !m_pPlayer )
		{
			return nullptr;
		}

		return STRING(m_pPlayer->v.netname);
	}

	int CBotrixPlayerInfo::GetUserID() const
	{
		return GETPLAYERUSERID(m_pPlayer);
	}

	const char* CBotrixPlayerInfo::GetNetworkIDString() const
	{
		return GETPLAYERAUTHID(m_pPlayer);
	}

	int CBotrixPlayerInfo::GetTeamIndex() const
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

	void CBotrixPlayerInfo::ChangeTeam(int newTeamIndex)
	{
		if ( !m_pPlayer || !g_pGameRules )
		{
			return;
		}

		g_pGameRules->ChangePlayerTeam(GetPlayerEntity(), g_pGameRules->GetIndexedTeamName(newTeamIndex), false, false);
	}

	int CBotrixPlayerInfo::GetFragCount() const
	{
		if ( !m_pPlayer )
		{
			return 0;
		}

		return static_cast<int>(m_pPlayer->v.frags);
	}

	int CBotrixPlayerInfo::GetDeathCount() const
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

	bool CBotrixPlayerInfo::IsConnected() const
	{
		// Not sure if this is the most canonical way of checking this, but it's
		// the method used in the multiplayer gamerules, and I can't find an
		// official flag or anything for it.
		return m_pPlayer && (m_pPlayer->v.netname && (STRING(m_pPlayer->v.netname))[0] != 0);
	}

	int CBotrixPlayerInfo::GetArmorValue() const
	{
		return m_pPlayer ? static_cast<int>(m_pPlayer->v.armorvalue) : 0;
	}

	bool CBotrixPlayerInfo::IsHLTV() const
	{
		return g_engfuncs.pfnIsHLTVClient(m_pPlayer);
	}

	bool CBotrixPlayerInfo::IsPlayer() const
	{
		// Do this manually to avoid accidentally failing the assertion
		return m_pPlayer && dynamic_cast<CBasePlayer*>(CBaseEntity::Instance(m_pPlayer));
	}

	bool CBotrixPlayerInfo::IsFakeClient() const
	{
		if ( !m_pPlayer )
		{
			return false;
		}

		return (m_pPlayer->v.flags & FL_FAKECLIENT) != 0;
	}

	bool CBotrixPlayerInfo::IsDead() const
	{
		if ( !m_pPlayer )
		{
			return false;
		}

		// This is essentially !IsAlive(), which is what the bot code needs.
		return m_pPlayer->v.deadflag != DEAD_NO;
	}

	bool CBotrixPlayerInfo::IsInAVehicle() const
	{
		return false;
	}

	bool CBotrixPlayerInfo::IsObserver() const
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

	Vector CBotrixPlayerInfo::GetAbsOrigin() const
	{
		if ( !m_pPlayer )
		{
			return Vector();
		}

		return m_pPlayer->v.origin;
	}

	Vector CBotrixPlayerInfo::GetEyePosition() const
	{
		Vector out;
		CBotrixEngineUtil::EntityHead(m_pPlayer, out);
		return out;
	}

	Vector CBotrixPlayerInfo::GetAbsAngles() const
	{
		if ( !m_pPlayer )
		{
			return Vector();
		}

		return m_pPlayer->v.angles;
	}

	Vector CBotrixPlayerInfo::GetEyeAngles() const
	{
		if ( !m_pPlayer )
		{
			return Vector();
		}

		return m_pPlayer->v.v_angle;
	}

	Vector CBotrixPlayerInfo::GetPlayerMins() const
	{
		if ( !m_pPlayer )
		{
			return Vector();
		}

		return m_pPlayer->v.mins;
	}

	Vector CBotrixPlayerInfo::GetPlayerMaxs() const
	{
		if ( !m_pPlayer )
		{
			return Vector();
		}

		return m_pPlayer->v.maxs;
	}

	const char* CBotrixPlayerInfo::GetWeaponName() const
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

	const char* CBotrixPlayerInfo::GetModelName() const
	{
		if ( !m_pPlayer )
		{
			return nullptr;
		}

		return STRING(m_pPlayer->v.model);
	}

	int CBotrixPlayerInfo::GetHealth() const
	{
		if ( !m_pPlayer )
		{
			return 0;
		}

		return static_cast<int>(m_pPlayer->v.health);
	}

	int CBotrixPlayerInfo::GetMaxHealth() const
	{
		if ( !m_pPlayer )
		{
			return 0;
		}

		return static_cast<int>(m_pPlayer->v.max_health);
	}

	bool CBotrixPlayerInfo::IsReplay() const
	{
		return false;
	}
}  // namespace Botrix
