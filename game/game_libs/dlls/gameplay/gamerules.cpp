/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/
//=========================================================
// GameRules.cpp
//=========================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "gamerules.h"
#include "teamplay_gamerules.h"
#include "skill.h"
#include "game.h"
#include "weaponregistry.h"
#include "weaponatts_collection.h"
#include "gameplay/gameplaySystems.h"
#include "gameplay/gameplaySystemsBase.h"
#include "spawnpointmanager.h"

DLL_GLOBAL CGameRules* g_pGameRules = NULL;
extern DLL_GLOBAL BOOL g_fGameOver;
extern int gmsgDeathMsg;  // client dll messages
extern int gmsgMOTD;

int g_teamplay = 0;

CGameRules::CGameRules()
{
}

CGameRules::~CGameRules()
{
}

void CGameRules::ServerActivate()
{
}

void CGameRules::ServerDeactivate()
{
}

//=========================================================
//=========================================================
BOOL CGameRules::CanHaveAmmo(CBasePlayer* pPlayer, const char* pszAmmoName, int iMaxCarry)
{
	int iAmmoIndex;

	if ( pszAmmoName )
	{
		iAmmoIndex = pPlayer->GetAmmoIndex(pszAmmoName);

		if ( iAmmoIndex > -1 )
		{
			if ( pPlayer->AmmoInventory(iAmmoIndex) < iMaxCarry )
			{
				// player has room for more of this type of ammo
				return TRUE;
			}
		}
	}

	return FALSE;
}

//=========================================================
//=========================================================
edict_t* CGameRules::GetPlayerSpawnSpot(CBasePlayer* pPlayer)
{
	CSpawnPointManager::SpawnPointType spType = CSpawnPointManager::SP_Default;
	uint32_t flags = 0;

	if ( IsDeathmatch() )
	{
		spType = CSpawnPointManager::SP_Deathmatch;
		flags |= CSpawnPointManager::Flag_AvoidDeathSite;
	}
	else if ( IsTeamplay() )
	{
		spType = CSpawnPointManager::SP_CoOp;
		flags |= CSpawnPointManager::Flag_AvoidDeathSite;
	}

	CGameplaySystemsBase* gpSys = GameplaySystems::GetBase();

	edict_t* pentSpawnSpot = gpSys->SpawnPointManager().GetNextSpawnPoint(pPlayer, spType, flags)->edict();

	VectorAdd(VARS(pentSpawnSpot)->origin, Vector(0, 0, 1), pPlayer->pev->origin);
	VectorClear(pPlayer->pev->v_angle);
	VectorClear(pPlayer->pev->velocity);
	VectorCopy(VARS(pentSpawnSpot)->angles, pPlayer->pev->angles);
	VectorClear(pPlayer->pev->punchangle);
	pPlayer->pev->fixangle = TRUE;

	return pentSpawnSpot;
}

//=========================================================
//=========================================================
BOOL CGameRules::CanHavePlayerItem(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon)
{
	// only living players can have items
	if ( pPlayer->pev->deadflag != DEAD_NO )
		return FALSE;

	if ( pWeapon->pszAmmo1() )
	{
		if ( !CanHaveAmmo(pPlayer, pWeapon->pszAmmo1(), pWeapon->iMaxAmmo1()) )
		{
			// we can't carry anymore ammo for this gun. We can only
			// have the gun if we aren't already carrying one of this type
			if ( pPlayer->HasPlayerItem(pWeapon) )
			{
				return FALSE;
			}
		}
	}
	else
	{
		// weapon doesn't use ammo, don't take another if you already have it.
		if ( pPlayer->HasPlayerItem(pWeapon) )
		{
			return FALSE;
		}
	}

	// note: will fall through to here if GetItemInfo doesn't fill the struct!
	return TRUE;
}

//=========================================================
// load the SkillData struct with the proper values based on the skill level.
//=========================================================
void CGameRules::RefreshSkillData(void)
{
	int iSkill;

	iSkill = (int)CVAR_GET_FLOAT("skill");
	g_iSkillLevel = iSkill;

	if ( iSkill < 1 )
	{
		iSkill = 1;
	}
	else if ( iSkill > 3 )
	{
		iSkill = 3;
	}

	gSkillData.iSkillLevel = iSkill;

	ALERT(at_console, "GAME SKILL LEVEL:%d\n", iSkill);

	// Turret
	gSkillData.turretHealth = GetSkillCvar("sk_turret_health");

	// MiniTurret
	gSkillData.miniturretHealth = GetSkillCvar("sk_miniturret_health");

	// Sentry Turret
	gSkillData.sentryHealth = GetSkillCvar("sk_sentry_health");

	// HEALTH/CHARGE
	gSkillData.suitchargerCapacity = GetSkillCvar("sk_suitcharger");
	gSkillData.batteryCapacity = GetSkillCvar("sk_battery");
	gSkillData.healthchargerCapacity = GetSkillCvar("sk_healthcharger");
	gSkillData.healthkitCapacity = GetSkillCvar("sk_healthkit");
	gSkillData.scientistHeal = GetSkillCvar("sk_scientist_heal");

	// monster damage adj
	gSkillData.monHead = GetSkillCvar("sk_monster_head");
	gSkillData.monChest = GetSkillCvar("sk_monster_chest");
	gSkillData.monStomach = GetSkillCvar("sk_monster_stomach");
	gSkillData.monLeg = GetSkillCvar("sk_monster_leg");
	gSkillData.monArm = GetSkillCvar("sk_monster_arm");

	// player damage adj
	gSkillData.plrHead = GetSkillCvar("sk_player_head");
	gSkillData.plrChest = GetSkillCvar("sk_player_chest");
	gSkillData.plrStomach = GetSkillCvar("sk_player_stomach");
	gSkillData.plrLeg = GetSkillCvar("sk_player_leg");
	gSkillData.plrArm = GetSkillCvar("sk_player_arm");

	// Nightfire weapons
	CWeaponRegistry::StaticInstance().ForEach(
		[](const WeaponAtts::WACollection& atts)
		{
			FOR_EACH_VEC(atts.SkillRecords, index)
			{
				atts.SkillRecords[index].UpdateSkillValue(gSkillData);
			}
		}
	);
}

//=========================================================
// instantiate the proper game rules object
//=========================================================

CGameRules* InstallGameRules(void)
{
	SERVER_COMMAND("exec game.cfg\n");
	SERVER_EXECUTE();

	if ( !gpGlobals->deathmatch )
	{
		// generic half-life
		g_teamplay = 0;
		return new CHalfLifeRules();
	}
	else
	{
		if ( teamplay.value > 0 )
		{
			// teamplay
			g_teamplay = 1;
			return new CHalfLifeTeamplay();
		}

		if ( (int)gpGlobals->deathmatch == 1 )
		{
			// vanilla deathmatch
			g_teamplay = 0;
			return new CHalfLifeMultiplay();
		}
		else
		{
			// vanilla deathmatch??
			g_teamplay = 0;
			return new CHalfLifeMultiplay();
		}
	}
}
