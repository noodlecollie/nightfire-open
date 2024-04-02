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

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"
#include "ammodefs.h"
#include "EnginePublicAPI/event_args.h"
#include "eventConstructor/eventConstructor.h"

LINK_ENTITY_TO_CLASS(weapon_mp5, CMP5)
LINK_ENTITY_TO_CLASS(weapon_9mmAR, CMP5)

//=========================================================
//=========================================================
int CMP5::SecondaryAmmoIndex(void)
{
	return m_iSecondaryAmmoType;
}

void CMP5::Spawn()
{
	pev->classname = MAKE_STRING("weapon_9mmAR");  // hack to allow for old names
	Precache();
	SET_MODEL(ENT(pev), "models/w_9mmAR.mdl");
	m_iId = WEAPON_MP5;

	m_iDefaultAmmo = MP5_DEFAULT_GIVE;

	FallInit();  // get ready to fall down.
}

void CMP5::Precache(void)
{
	PRECACHE_MODEL("models/v_9mmAR.mdl");
	PRECACHE_MODEL("models/w_9mmAR.mdl");
	PRECACHE_MODEL("models/p_9mmAR.mdl");

	m_iShell = PRECACHE_MODEL("models/shell.mdl");  // brass shellTE_MODEL

	PRECACHE_MODEL("models/grenade.mdl");  // grenade

	PRECACHE_MODEL("models/w_9mmARclip.mdl");
	PRECACHE_SOUND("items/9mmclip1.wav");

	PRECACHE_SOUND("items/clipinsert1.wav");
	PRECACHE_SOUND("items/cliprelease1.wav");

	PRECACHE_SOUND("weapons/hks1.wav");  // H to the K
	PRECACHE_SOUND("weapons/hks2.wav");  // H to the K
	PRECACHE_SOUND("weapons/hks3.wav");  // H to the K

	PRECACHE_SOUND("weapons/glauncher.wav");
	PRECACHE_SOUND("weapons/glauncher2.wav");

	PRECACHE_SOUND("weapons/357_cock1.wav");

	m_usMP5 = PRECACHE_EVENT(1, "events/mp5.sc");
	m_usMP52 = PRECACHE_EVENT(1, "events/mp52.sc");
}

int CMP5::GetItemInfo(ItemInfo* p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = AmmoDef_9mm.AmmoName;
	p->iMaxAmmo1 = AmmoDef_9mm.MaxCarry;
	p->pszAmmo2 = AmmoDef_M203Grenade.AmmoName;
	p->iMaxAmmo2 = AmmoDef_M203Grenade.MaxCarry;
	p->iMaxClip = MP5_MAX_CLIP;
	p->iSlot = 2;
	p->iPosition = 0;
	p->iFlags = 0;
	p->iId = m_iId = WEAPON_MP5;
	p->iWeight = MP5_WEIGHT;

	return 1;
}

int CMP5::AddToPlayer(CBasePlayer* pPlayer)
{
	if ( CBasePlayerWeapon::AddToPlayer(pPlayer) )
	{
		MESSAGE_BEGIN(MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev);
		WRITE_BYTE(m_iId);
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

BOOL CMP5::Deploy()
{
	return DefaultDeploy("models/v_9mmAR.mdl", "models/p_9mmAR.mdl", MP5_DEPLOY, "mp5");
}

void CMP5::PrimaryAttack()
{
	// don't fire underwater
	if ( m_pPlayer->pev->waterlevel == 3 )
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = 0.15f;
		return;
	}

	if ( m_iClip <= 0 )
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = 0.15f;
		return;
	}

	m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	m_iClip--;

	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecAiming = m_pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);
	Vector vecDir;
#ifdef CLIENT_DLL
	if ( !bIsMultiplayer() )
#else
	if ( !g_pGameRules->IsMultiplayer() )
#endif
	{
		// optimized multiplayer. Widened to make it easier to hit a moving player
		vecDir = m_pPlayer->FireBulletsPlayer(
			1,
			vecSrc,
			vecAiming,
			VECTOR_CONE_6DEGREES,
			8192,
			BULLET_PLAYER_MP5,
			2,
			0,
			m_pPlayer->pev,
			m_pPlayer->random_seed);
	}
	else
	{
		// single player spread
		vecDir = m_pPlayer->FireBulletsPlayer(
			1,
			vecSrc,
			vecAiming,
			VECTOR_CONE_3DEGREES,
			8192,
			BULLET_PLAYER_MP5,
			2,
			0,
			m_pPlayer->pev,
			m_pPlayer->random_seed);
	}

	int flags;
#if defined(CLIENT_WEAPONS)
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	using namespace EventConstructor;
	CEventConstructor event;

	event << Flags(flags) << Invoker(m_pPlayer->edict()) << EventIndex(m_usMP5) << FloatParam1(vecDir.x)
		  << FloatParam1(vecDir.y);

	event.Send();

	if ( !m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
	{
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}

	m_flNextPrimaryAttack = GetNextAttackDelay(0.1f);

	if ( m_flNextPrimaryAttack < UTIL_WeaponTimeBase() )
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.1f;

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
}

void CMP5::SecondaryAttack(void)
{
	// don't fire underwater
	if ( m_pPlayer->pev->waterlevel == 3 )
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = 0.15f;
		return;
	}

	if ( m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] == 0 )
	{
		PlayEmptySound();
		return;
	}

	m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = BRIGHT_GUN_FLASH;

	m_pPlayer->m_iExtraSoundTypes = bits_SOUND_DANGER;
	m_pPlayer->m_flStopExtraSoundTime = UTIL_WeaponTimeBase() + 0.2f;

	m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType]--;

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	UTIL_MakeVectors(Vector(m_pPlayer->pev->v_angle) + Vector(m_pPlayer->pev->punchangle));

	// we don't add in player velocity anymore.
	CGrenade::ShootContact(
		m_pPlayer->pev,
		Vector(m_pPlayer->pev->origin) + Vector(m_pPlayer->pev->view_ofs) + Vector(gpGlobals->v_forward) * 16,
		Vector(gpGlobals->v_forward) * 800);

	int flags;
#if defined(CLIENT_WEAPONS)
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	using namespace EventConstructor;

	CEventConstructor event;
	event.InitSimple(flags, m_pPlayer->edict(), m_usMP52);

	event.Send();

	m_flNextPrimaryAttack = GetNextAttackDelay(1);
	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 1;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 5;  // idle pretty soon after shooting.

	if ( !m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] )
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
}

void CMP5::Reload(void)
{
	if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 || m_iClip == MP5_MAX_CLIP )
		return;

	DefaultReload(MP5_MAX_CLIP, MP5_RELOAD, 1.5);
}

void CMP5::WeaponIdle(void)
{
	ResetEmptySound();

	m_pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);

	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	int iAnim;
	switch ( RANDOM_LONG(0, 1) )
	{
		case 0:
			iAnim = MP5_LONGIDLE;
			break;
		default:
		case 1:
			iAnim = MP5_IDLE1;
			break;
	}

	SendWeaponAnim(iAnim);

	m_flTimeWeaponIdle = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);  // how long till we do this again.
}

BOOL CMP5::IsUseable()
{
	// Can be used if the player has AR grenades. - Solokiller
	return CBasePlayerWeapon::IsUseable() || m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] > 0;
}

class CMP5AmmoClip : public CBasePlayerAmmo
{
	void Spawn(void)
	{
		Precache();
		SET_MODEL(ENT(pev), "models/w_9mmARclip.mdl");
		CBasePlayerAmmo::Spawn();
	}
	void Precache(void)
	{
		PRECACHE_MODEL("models/w_9mmARclip.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo(CBaseEntity* pOther)
	{
		int bResult = (pOther->GiveAmmo(AMMO_MP5CLIP_GIVE, AmmoDef_9mm.AmmoName, AmmoDef_9mm.MaxCarry) != -1);
		if ( bResult )
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
		}
		return bResult;
	}
};

LINK_ENTITY_TO_CLASS(ammo_mp5clip, CMP5AmmoClip)
LINK_ENTITY_TO_CLASS(ammo_9mmAR, CMP5AmmoClip)

class CMP5Chainammo : public CBasePlayerAmmo
{
	void Spawn(void)
	{
		Precache();
		SET_MODEL(ENT(pev), "models/w_chainammo.mdl");
		CBasePlayerAmmo::Spawn();
	}
	void Precache(void)
	{
		PRECACHE_MODEL("models/w_chainammo.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo(CBaseEntity* pOther)
	{
		int bResult = (pOther->GiveAmmo(AMMO_CHAINBOX_GIVE, AmmoDef_9mm.AmmoName, AmmoDef_9mm.MaxCarry) != -1);
		if ( bResult )
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
		}
		return bResult;
	}
};

LINK_ENTITY_TO_CLASS(ammo_9mmbox, CMP5Chainammo)

class CMP5AmmoGrenade : public CBasePlayerAmmo
{
	void Spawn(void)
	{
		Precache();
		SET_MODEL(ENT(pev), "models/w_ARgrenade.mdl");
		CBasePlayerAmmo::Spawn();
	}
	void Precache(void)
	{
		PRECACHE_MODEL("models/w_ARgrenade.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo(CBaseEntity* pOther)
	{
		int bResult =
			(pOther->GiveAmmo(AMMO_M203BOX_GIVE, AmmoDef_M203Grenade.AmmoName, AmmoDef_M203Grenade.MaxCarry) != -1);

		if ( bResult )
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
		}
		return bResult;
	}
};

LINK_ENTITY_TO_CLASS(ammo_mp5grenades, CMP5AmmoGrenade)
LINK_ENTITY_TO_CLASS(ammo_ARgrenades, CMP5AmmoGrenade)
