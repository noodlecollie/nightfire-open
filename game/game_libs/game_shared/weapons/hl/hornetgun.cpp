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
#if !defined(OEM_BUILD) && !defined(HLDEMO_BUILD)

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "hornet.h"
#include "gamerules.h"
#include "ammodefs.h"
#include "EnginePublicAPI/event_args.h"
#include "eventConstructor/eventConstructor.h"

enum hgun_e
{
	HGUN_IDLE1 = 0,
	HGUN_FIDGETSWAY,
	HGUN_FIDGETSHAKE,
	HGUN_DOWN,
	HGUN_UP,
	HGUN_SHOOT
};

enum firemode_e
{
	FIREMODE_TRACK = 0,
	FIREMODE_FAST
};

LINK_ENTITY_TO_CLASS(weapon_hornetgun, CHgun)

BOOL CHgun::IsUseable(void)
{
	return TRUE;
}

void CHgun::Spawn()
{
	Precache();
	m_iId = WEAPON_HORNETGUN;
	SET_MODEL(ENT(pev), "models/w_hgun.mdl");

	m_iDefaultAmmo = HIVEHAND_DEFAULT_GIVE;
	m_iFirePhase = 0;

	FallInit();  // get ready to fall down.
}

void CHgun::Precache(void)
{
	PRECACHE_MODEL("models/v_hgun.mdl");
	PRECACHE_MODEL("models/w_hgun.mdl");
	PRECACHE_MODEL("models/p_hgun.mdl");

	m_usHornetFire = PRECACHE_EVENT(1, "events/firehornet.sc");

	UTIL_PrecacheOther("hornet");
}

int CHgun::AddToPlayer(CBasePlayer* pPlayer)
{
	if ( CBasePlayerWeapon::AddToPlayer(pPlayer) )
	{
#ifndef CLIENT_DLL
		if ( g_pGameRules->IsMultiplayer() )
		{
			// in multiplayer, all hivehands come full.
			pPlayer->m_rgAmmo[PrimaryAmmoIndex()] = AmmoDef_Hornet.MaxCarry;
		}
#endif
		MESSAGE_BEGIN(MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev);
		WRITE_BYTE(m_iId);
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

int CHgun::GetItemInfo(ItemInfo* p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = AmmoDef_Hornet.AmmoName;
	p->iMaxAmmo1 = AmmoDef_Hornet.MaxCarry;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 3;
	p->iPosition = 3;
	p->iId = m_iId = WEAPON_HORNETGUN;
	p->iFlags = ITEM_FLAG_NOAUTOSWITCHEMPTY | ITEM_FLAG_NOAUTORELOAD;
	p->iWeight = HORNETGUN_WEIGHT;

	return 1;
}

BOOL CHgun::Deploy()
{
	return DefaultDeploy("models/v_hgun.mdl", "models/p_hgun.mdl", HGUN_UP, "hive");
}

void CHgun::Holster(int)
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;
	SendWeaponAnim(HGUN_DOWN);

	//!!!HACKHACK - can't select hornetgun if it's empty! no way to get ammo for it, either.
	if ( !m_pPlayer->m_rgAmmo[PrimaryAmmoIndex()] )
	{
		m_pPlayer->m_rgAmmo[PrimaryAmmoIndex()] = 1;
	}
}

void CHgun::PrimaryAttack()
{
	Reload();

	if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
	{
		return;
	}
#ifndef CLIENT_DLL
	UTIL_MakeVectors(m_pPlayer->pev->v_angle);

	CBaseEntity* pHornet = CBaseEntity::Create(
		"hornet",
		m_pPlayer->GetGunPosition() + Vector(gpGlobals->v_forward) * 16 + Vector(gpGlobals->v_right) * 8 +
			Vector(gpGlobals->v_up) * -12,
		m_pPlayer->pev->v_angle,
		m_pPlayer->edict());
	(Vector(gpGlobals->v_forward) * 300).CopyToArray(pHornet->pev->velocity);

	m_flRechargeTime = gpGlobals->time + 0.5f;
#endif
	m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;

	m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = DIM_GUN_FLASH;

	int flags;
#if defined(CLIENT_WEAPONS)
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	using namespace EventConstructor;
	CEventConstructor event;

	event << Flags(flags) << Invoker(m_pPlayer->edict()) << EventIndex(m_usHornetFire) << IntParam1(FIREMODE_TRACK);

	event.Send();

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_flNextPrimaryAttack = m_flNextPrimaryAttack + 0.25f;

	if ( m_flNextPrimaryAttack < UTIL_WeaponTimeBase() )
	{
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.25f;
	}

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
}

void CHgun::SecondaryAttack(void)
{
	Reload();

	if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
	{
		return;
	}

	// Wouldn't be a bad idea to completely predict these, since they fly so fast...
#ifndef CLIENT_DLL
	CBaseEntity* pHornet;
	Vector vecSrc;

	UTIL_MakeVectors(m_pPlayer->pev->v_angle);

	vecSrc = m_pPlayer->GetGunPosition() + Vector(gpGlobals->v_forward) * 16 + Vector(gpGlobals->v_right) * 8 +
		Vector(gpGlobals->v_up) * -12;

	m_iFirePhase++;
	switch ( m_iFirePhase )
	{
		case 1:
			vecSrc = Vector(vecSrc) + Vector(gpGlobals->v_up) * 8;
			break;
		case 2:
			vecSrc = Vector(vecSrc) + Vector(gpGlobals->v_up) * 8;
			vecSrc = Vector(vecSrc) + Vector(gpGlobals->v_right) * 8;
			break;
		case 3:
			vecSrc = Vector(vecSrc) + Vector(gpGlobals->v_right) * 8;
			break;
		case 4:
			vecSrc = Vector(vecSrc) + Vector(gpGlobals->v_up) * -8;
			vecSrc = Vector(vecSrc) + Vector(gpGlobals->v_right) * 8;
			break;
		case 5:
			vecSrc = Vector(vecSrc) + Vector(gpGlobals->v_up) * -8;
			break;
		case 6:
			vecSrc = Vector(vecSrc) + Vector(gpGlobals->v_up) * -8;
			vecSrc = Vector(vecSrc) + Vector(gpGlobals->v_right) * -8;
			break;
		case 7:
			vecSrc = Vector(vecSrc) + Vector(gpGlobals->v_right) * -8;
			break;
		case 8:
			vecSrc = Vector(vecSrc) + Vector(gpGlobals->v_up) * 8;
			vecSrc = Vector(vecSrc) + Vector(gpGlobals->v_right) * -8;
			m_iFirePhase = 0;
			break;
	}

	pHornet = CBaseEntity::Create("hornet", vecSrc, m_pPlayer->pev->v_angle, m_pPlayer->edict());
	(Vector(gpGlobals->v_forward) * 1200).CopyToArray(pHornet->pev->velocity);
	UTIL_VecToAngles(pHornet->pev->velocity).CopyToArray(pHornet->pev->angles);

	pHornet->SetThink(&CHornet::StartDart);

	m_flRechargeTime = gpGlobals->time + 0.5f;
#endif
	int flags;
#if defined(CLIENT_WEAPONS)
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	using namespace EventConstructor;
	CEventConstructor event;

	event << Flags(flags) << Invoker(m_pPlayer->edict()) << EventIndex(m_usHornetFire) << IntParam1(FIREMODE_FAST);

	event.Send();

	m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;
	m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = DIM_GUN_FLASH;

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.1f;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
}

void CHgun::Reload(void)
{
	if ( static_cast<uint32_t>(m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]) >= AmmoDef_Hornet.MaxCarry )
	{
		return;
	}

	while ( static_cast<uint32_t>(m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]) < AmmoDef_Hornet.MaxCarry &&
			m_flRechargeTime < gpGlobals->time )
	{
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]++;
		m_flRechargeTime += 0.5;
	}
}

void CHgun::WeaponIdle(void)
{
	Reload();

	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	int iAnim;
	float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1);
	if ( flRand <= 0.75 )
	{
		iAnim = HGUN_IDLE1;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 30.0f / 16.0f * 2.0f;
	}
	else if ( flRand <= 0.875 )
	{
		iAnim = HGUN_FIDGETSWAY;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 40.0f / 16.0f;
	}
	else
	{
		iAnim = HGUN_FIDGETSHAKE;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 35.0f / 16.0f;
	}
	SendWeaponAnim(iAnim);
}
#endif
