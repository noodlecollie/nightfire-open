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
#include "gamerules.h"
#include "ammodefs.h"
#include "radialdamage.h"
#include "EnginePublicAPI/event_args.h"
#include "eventConstructor/eventConstructor.h"
#include "MathLib/angles.h"

#ifndef CLIENT_DLL
#define BOLT_AIR_VELOCITY 2000
#define BOLT_WATER_VELOCITY 1000

extern BOOL gPhysicsInterfaceInitialized;

// UNDONE: Save/restore this?  Don't forget to set classname and LINK_ENTITY_TO_CLASS()
//
// OVERLOADS SOME ENTVARS:
//
// speed - the ideal magnitude of my velocity
class CCrossbowBolt : public CBaseEntity
{
	void Spawn(void);
	void Precache(void);
	int Classify(void);
	void EXPORT BubbleThink(void);
	void EXPORT BoltTouch(CBaseEntity* pOther);
	void EXPORT ExplodeThink(void);

	int m_iTrail;

public:
	static CCrossbowBolt* BoltCreate(void);
};

LINK_ENTITY_TO_CLASS(crossbow_bolt, CCrossbowBolt)

CCrossbowBolt* CCrossbowBolt::BoltCreate(void)
{
	// Create a new entity with CCrossbowBolt private data
	CCrossbowBolt* pBolt = GetClassPtr<CCrossbowBolt>();
	pBolt->pev->classname = MAKE_STRING("crossbow_bolt");  // g-cont. enable save\restore
	pBolt->Spawn();

	return pBolt;
}

void CCrossbowBolt::Spawn()
{
	Precache();
	pev->movetype = MOVETYPE_FLY;
	pev->solid = SOLID_BBOX;

	pev->gravity = 0.5;

	SET_MODEL(ENT(pev), "models/crossbow_bolt.mdl");

	UTIL_SetOrigin(pev, pev->origin);
	UTIL_SetSize(pev, Vector(0, 0, 0), Vector(0, 0, 0));

	SetTouch(&CCrossbowBolt::BoltTouch);
	SetThink(&CCrossbowBolt::BubbleThink);
	pev->nextthink = gpGlobals->time + 0.2f;
}

void CCrossbowBolt::Precache()
{
	PRECACHE_MODEL("models/crossbow_bolt.mdl");
	PRECACHE_SOUND("weapons/xbow_hitbod1.wav");
	PRECACHE_SOUND("weapons/xbow_hitbod2.wav");
	PRECACHE_SOUND("weapons/xbow_fly1.wav");
	PRECACHE_SOUND("weapons/xbow_hit1.wav");
	PRECACHE_SOUND("fvox/beep.wav");
	m_iTrail = PRECACHE_MODEL("sprites/streak.spr");
}

int CCrossbowBolt::Classify(void)
{
	return CLASS_NONE;
}

void CCrossbowBolt::BoltTouch(CBaseEntity* pOther)
{
	SetTouch(NULL);
	SetThink(NULL);

	if ( pOther->pev->takedamage )
	{
		TraceResult tr = UTIL_GetGlobalTrace();
		entvars_t* pevOwner;

		pevOwner = VARS(pev->owner);

		// UNDONE: this needs to call TraceAttack instead
		ClearMultiDamage();

		if ( pOther->IsPlayer() )
		{
			pOther->TraceAttack(
				pevOwner,
				gSkillData.plrDmgCrossbowClient,
				Vector(pev->velocity).Normalize(),
				&tr,
				DMG_NEVERGIB);
		}
		else
		{
			pOther->TraceAttack(
				pevOwner,
				gSkillData.plrDmgCrossbowMonster,
				Vector(pev->velocity).Normalize(),
				&tr,
				DMG_BULLET | DMG_NEVERGIB);
		}

		ApplyMultiDamage(pev, pevOwner);

		VectorClear(pev->velocity);
		// play body "thwack" sound
		switch ( RANDOM_LONG(0, 1) )
		{
			case 0:
				EMIT_SOUND(ENT(pev), CHAN_BODY, "weapons/xbow_hitbod1.wav", 1, ATTN_NORM);
				break;
			case 1:
				EMIT_SOUND(ENT(pev), CHAN_BODY, "weapons/xbow_hitbod2.wav", 1, ATTN_NORM);
				break;
		}

		if ( !g_pGameRules->IsMultiplayer() )
		{
			Killed(pev, GIB_NEVER);
		}
	}
	else
	{
		EMIT_SOUND_DYN(
			ENT(pev),
			CHAN_BODY,
			"weapons/xbow_hit1.wav",
			RANDOM_FLOAT(0.95f, 1.0f),
			ATTN_NORM,
			0,
			98 + RANDOM_LONG(0, 7));

		SetThink(&CBaseEntity::SUB_Remove);
		pev->nextthink =
			gpGlobals->time;  // this will get changed below if the bolt is allowed to stick in what it hit.

		if ( FClassnameIs(pOther->pev, "worldspawn") )
		{
			// if what we hit is static architecture, can stay around for a while.
			Vector vecDir = Vector(pev->velocity).Normalize();
			UTIL_SetOrigin(pev, Vector(pev->origin) - vecDir * 12);
			UTIL_VecToAngles(vecDir).CopyToArray(pev->angles);
			pev->solid = SOLID_NOT;
			pev->movetype = MOVETYPE_FLY;
			VectorClear(pev->velocity);
			pev->avelocity[ROLL] = 0;
			pev->angles[ROLL]= static_cast<float>(RANDOM_LONG(0, 360));
			pev->nextthink = static_cast<float>(gpGlobals->time + 10.0);
		}
		else if ( pOther->pev->movetype == MOVETYPE_PUSH || pOther->pev->movetype == MOVETYPE_PUSHSTEP )
		{
			Vector vecDir = Vector(pev->velocity).Normalize();
			UTIL_SetOrigin(pev, Vector(pev->origin) - vecDir * 12);
			UTIL_VecToAngles(vecDir).CopyToArray(pev->angles);
			pev->solid = SOLID_NOT;
			VectorClear(pev->velocity);
			pev->avelocity[ROLL] = 0;
			pev->angles[ROLL] = static_cast<float>(RANDOM_LONG(0, 360));
			pev->nextthink = static_cast<float>(gpGlobals->time + 10.0);

			if ( gPhysicsInterfaceInitialized )
			{
				// g-cont. Setup movewith feature
				pev->movetype = MOVETYPE_COMPOUND;  // set movewith type
				pev->aiment = ENT(pOther->pev);  // set parent
			}
		}

		if ( UTIL_PointContents(pev->origin) != CONTENTS_WATER )
		{
			UTIL_Sparks(pev->origin);
		}
	}

	if ( g_pGameRules->IsMultiplayer() )
	{
		SetThink(&CCrossbowBolt::ExplodeThink);
		pev->nextthink = static_cast<float>(gpGlobals->time + 0.1);
	}
}

void CCrossbowBolt::BubbleThink(void)
{
	pev->nextthink = static_cast<float>(gpGlobals->time + 0.1);

	if ( pev->waterlevel == 0 )
		return;

	UTIL_BubbleTrail(Vector(pev->origin) - Vector(pev->velocity) * 0.1f, pev->origin, 1);
}

void CCrossbowBolt::ExplodeThink(void)
{
	int iContents = UTIL_PointContents(pev->origin);
	int iScale;

	pev->dmg = 40;
	iScale = 10;

	MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pev->origin);
	WRITE_BYTE(TE_EXPLOSION);
	WRITE_COORD(pev->origin[VEC3_X]);
	WRITE_COORD(pev->origin[VEC3_Y]);
	WRITE_COORD(pev->origin[VEC3_Z]);
	if ( iContents != CONTENTS_WATER )
	{
		WRITE_SHORT(g_sModelIndexFireball);
	}
	else
	{
		WRITE_SHORT(g_sModelIndexWExplosion);
	}
	WRITE_BYTE(iScale);  // scale * 10
	WRITE_BYTE(15);  // framerate
	WRITE_BYTE(TE_EXPLFLAG_NONE);
	MESSAGE_END();

	entvars_t* pevOwner;

	if ( pev->owner )
		pevOwner = VARS(pev->owner);
	else
		pevOwner = NULL;

	pev->owner = NULL;  // can't traceline attack owner if this is set

	CRadialDamageInflictor damage;

	damage.SetOrigin(pev->origin);
	damage.SetInflictor(pev);
	damage.SetAttacker(pevOwner);
	damage.SetDamage(pev->dmg);
	damage.SetRadius(128);
	damage.SetDamageTypeBits(DMG_BLAST | DMG_ALWAYSGIB);

	damage.ApplyDamage();

	UTIL_Remove(this);
}
#endif

enum crossbow_e
{
	CROSSBOW_IDLE1 = 0,  // full
	CROSSBOW_IDLE2,  // empty
	CROSSBOW_FIDGET1,  // full
	CROSSBOW_FIDGET2,  // empty
	CROSSBOW_FIRE1,  // full
	CROSSBOW_FIRE2,  // reload
	CROSSBOW_FIRE3,  // empty
	CROSSBOW_RELOAD,  // from empty
	CROSSBOW_DRAW1,  // full
	CROSSBOW_DRAW2,  // empty
	CROSSBOW_HOLSTER1,  // full
	CROSSBOW_HOLSTER2  // empty
};

LINK_ENTITY_TO_CLASS(weapon_crossbow, CCrossbow)

void CCrossbow::Spawn()
{
	Precache();
	m_iId = WEAPON_CROSSBOW;
	SET_MODEL(ENT(pev), "models/w_crossbow.mdl");

	m_iDefaultAmmo = CROSSBOW_DEFAULT_GIVE;

	FallInit();  // get ready to fall down.
}

int CCrossbow::AddToPlayer(CBasePlayer* pPlayer)
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

void CCrossbow::Precache(void)
{
	PRECACHE_MODEL("models/w_crossbow.mdl");
	PRECACHE_MODEL("models/v_crossbow.mdl");
	PRECACHE_MODEL("models/p_crossbow.mdl");

	PRECACHE_SOUND("weapons/xbow_fire1.wav");
	PRECACHE_SOUND("weapons/xbow_reload1.wav");

	UTIL_PrecacheOther("crossbow_bolt");

	m_usCrossbow = PRECACHE_EVENT(1, "events/crossbow1.sc");
	m_usCrossbow2 = PRECACHE_EVENT(1, "events/crossbow2.sc");
}

int CCrossbow::GetItemInfo(ItemInfo* p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = AmmoDef_Bolt.AmmoName;
	p->iMaxAmmo1 = AmmoDef_Bolt.MaxCarry;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = CROSSBOW_MAX_CLIP;
	p->iSlot = 2;
	p->iPosition = 2;
	p->iId = WEAPON_CROSSBOW;
	p->iFlags = 0;
	p->iWeight = CROSSBOW_WEIGHT;
	return 1;
}

BOOL CCrossbow::Deploy()
{
	if ( m_iClip )
		return DefaultDeploy("models/v_crossbow.mdl", "models/p_crossbow.mdl", CROSSBOW_DRAW1, "bow");
	return DefaultDeploy("models/v_crossbow.mdl", "models/p_crossbow.mdl", CROSSBOW_DRAW2, "bow");
}

void CCrossbow::Holster(int)
{
	m_fInReload = FALSE;  // cancel any reload in progress.

	if ( m_fInZoom )
	{
		SecondaryAttack();
	}

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;

	if ( m_iClip )
		SendWeaponAnim(CROSSBOW_HOLSTER1);
	else
		SendWeaponAnim(CROSSBOW_HOLSTER2);
}

void CCrossbow::PrimaryAttack(void)
{
#ifdef CLIENT_DLL
	if ( m_fInZoom && bIsMultiplayer() )
#else
	if ( m_fInZoom && g_pGameRules->IsMultiplayer() )
#endif
	{
		FireSniperBolt();
		return;
	}

	FireBolt();
}

// this function only gets called in multiplayer
void CCrossbow::FireSniperBolt()
{
	m_flNextPrimaryAttack = GetNextAttackDelay(0.75);

	if ( m_iClip == 0 )
	{
		PlayEmptySound();
		return;
	}

	TraceResult tr;

	m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;
	m_iClip--;

	int flags;
#if defined(CLIENT_WEAPONS)
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	using namespace EventConstructor;
	CEventConstructor event;

	event << Flags(flags) << Invoker(m_pPlayer->edict()) << EventIndex(m_usCrossbow2) << IntParam1(m_iClip)
		  << IntParam2(m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]);

	event.Send();

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	Vector anglesAim(Vector(m_pPlayer->pev->v_angle) + Vector(m_pPlayer->pev->punchangle));
	UTIL_MakeVectors(anglesAim);
	Vector vecSrc = m_pPlayer->GetGunPosition() - Vector(gpGlobals->v_up) * 2;
	Vector vecDir = gpGlobals->v_forward;

	UTIL_TraceLine(vecSrc, vecSrc + vecDir * 8192, dont_ignore_monsters, m_pPlayer->edict(), &tr);

#ifndef CLIENT_DLL
	if ( tr.pHit->v.takedamage )
	{
		ClearMultiDamage();
		CBaseEntity::Instance(tr.pHit)->TraceAttack(m_pPlayer->pev, 120, vecDir, &tr, DMG_BULLET | DMG_NEVERGIB);
		ApplyMultiDamage(pev, m_pPlayer->pev);
	}
#endif
}

void CCrossbow::FireBolt()
{
	if ( m_iClip == 0 )
	{
		PlayEmptySound();
		return;
	}

	m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;

	m_iClip--;

	int flags;
#if defined(CLIENT_WEAPONS)
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	using namespace EventConstructor;
	CEventConstructor event;

	event << Flags(flags) << Invoker(m_pPlayer->edict()) << EventIndex(m_usCrossbow) << IntParam1(m_iClip)
		  << IntParam2(m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]);

	event.Send();

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	Vector anglesAim(Vector(m_pPlayer->pev->v_angle) + Vector(m_pPlayer->pev->punchangle));
	UTIL_MakeVectors(anglesAim);

	anglesAim.x = -anglesAim.x;

#ifndef CLIENT_DLL
	Vector vecSrc = m_pPlayer->GetGunPosition() - Vector(gpGlobals->v_up) * 2;
	Vector vecDir = gpGlobals->v_forward;

	CCrossbowBolt* pBolt = CCrossbowBolt::BoltCreate();
	VectorCopy(vecSrc, pBolt->pev->origin);
	VectorCopy(anglesAim, pBolt->pev->angles);
	pBolt->pev->owner = m_pPlayer->edict();

	if ( m_pPlayer->pev->waterlevel == 3 )
	{
		(vecDir * BOLT_WATER_VELOCITY).CopyToArray(pBolt->pev->velocity);
		pBolt->pev->speed = BOLT_WATER_VELOCITY;
	}
	else
	{
		(vecDir * BOLT_AIR_VELOCITY).CopyToArray(pBolt->pev->velocity);
		pBolt->pev->speed = BOLT_AIR_VELOCITY;
	}
	pBolt->pev->avelocity[ROLL] = 10;
#endif

	if ( !m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);

	m_flNextPrimaryAttack = GetNextAttackDelay(0.75);

	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.75f;

	if ( m_iClip != 0 )
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 5.0f;
	else
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.75f;
}

void CCrossbow::SecondaryAttack()
{
	if ( m_pPlayer->pev->fov != 0 )
	{
		m_pPlayer->pev->fov = 0.0f;  // 0 means reset to default fov
		m_pPlayer->m_iFOV = 0;

		m_fInZoom = 0;
	}
	else if ( m_pPlayer->pev->fov != 20 )
	{
		m_pPlayer->pev->fov = 20.0f;
		m_pPlayer->m_iFOV = 20;
		m_fInZoom = 1;
	}

	pev->nextthink = UTIL_WeaponTimeBase() + 0.1f;
	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 1.0f;
}

void CCrossbow::Reload(void)
{
	if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 || m_iClip == CROSSBOW_MAX_CLIP )
		return;

	if ( m_pPlayer->pev->fov != 0 )
	{
		SecondaryAttack();
	}

	if ( DefaultReload(CROSSBOW_MAX_CLIP, CROSSBOW_RELOAD, 4.5) )
	{
		EMIT_SOUND_DYN(
			ENT(m_pPlayer->pev),
			CHAN_ITEM,
			"weapons/xbow_reload1.wav",
			RANDOM_FLOAT(0.95f, 1.0f),
			ATTN_NORM,
			0,
			93 + RANDOM_LONG(0, 0xF));
	}
}

void CCrossbow::WeaponIdle(void)
{
	m_pPlayer->GetAutoaimVector(
		AUTOAIM_2DEGREES);  // get the autoaim vector but ignore it;  used for autoaim crosshair in DM

	ResetEmptySound();

	if ( m_flTimeWeaponIdle < UTIL_WeaponTimeBase() )
	{
		float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1);
		if ( flRand <= 0.75 )
		{
			if ( m_iClip )
			{
				SendWeaponAnim(CROSSBOW_IDLE1);
			}
			else
			{
				SendWeaponAnim(CROSSBOW_IDLE2);
			}
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
		}
		else
		{
			if ( m_iClip )
			{
				SendWeaponAnim(CROSSBOW_FIDGET1);
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 90.0f / 30.0f;
			}
			else
			{
				SendWeaponAnim(CROSSBOW_FIDGET2);
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 80.0f / 30.0f;
			}
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
		}
	}
}

class CCrossbowAmmo : public CBasePlayerAmmo
{
	void Spawn(void)
	{
		Precache();
		SET_MODEL(ENT(pev), "models/w_crossbow_clip.mdl");
		CBasePlayerAmmo::Spawn();
	}
	void Precache(void)
	{
		PRECACHE_MODEL("models/w_crossbow_clip.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo(CBaseEntity* pOther)
	{
		if ( pOther->GiveAmmo(AMMO_CROSSBOWCLIP_GIVE, AmmoDef_Bolt.AmmoName, AmmoDef_Bolt.MaxCarry) != -1 )
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
			return TRUE;
		}
		return FALSE;
	}
};

LINK_ENTITY_TO_CLASS(ammo_crossbow, CCrossbowAmmo)
#endif
