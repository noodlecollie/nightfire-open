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
#include "effects.h"
#include "gamerules.h"
#include "ammodefs.h"
#include "EnginePublicAPI/event_args.h"
#include "eventConstructor/eventConstructor.h"

#define TRIPMINE_PRIMARY_VOLUME 450

enum tripmine_e
{
	TRIPMINE_IDLE1 = 0,
	TRIPMINE_IDLE2,
	TRIPMINE_ARM1,
	TRIPMINE_ARM2,
	TRIPMINE_FIDGET,
	TRIPMINE_HOLSTER,
	TRIPMINE_DRAW,
	TRIPMINE_WORLD,
	TRIPMINE_GROUND
};

#ifndef CLIENT_DLL
class CTripmineGrenade : public CGrenade
{
	void Spawn(void);
	void Precache(void);
	void UpdateOnRemove();

	virtual int Save(CSave& save);
	virtual int Restore(CRestore& restore);

	static TYPEDESCRIPTION m_SaveData[];

	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);

	void EXPORT WarningThink(void);
	void EXPORT PowerupThink(void);
	void EXPORT BeamBreakThink(void);
	void EXPORT DelayDeathThink(void);
	void Killed(entvars_t* pevAttacker, int iGib);

	void MakeBeam(void);
	void KillBeam(void);

	float m_flPowerUp;
	Vector m_vecDir;
	Vector m_vecEnd;
	float m_flBeamLength;

	EHANDLE m_hOwner;
	CBeam* m_pBeam;
	Vector m_posOwner;
	Vector m_angleOwner;
	edict_t* m_pRealOwner;  // tracelines don't hit PEV->OWNER, which means a player couldn't detonate his own trip
							// mine, so we store the owner here.
};

LINK_ENTITY_TO_CLASS(monster_tripmine, CTripmineGrenade)

TYPEDESCRIPTION CTripmineGrenade::m_SaveData[] = {
	DEFINE_FIELD(CTripmineGrenade, m_flPowerUp, FIELD_TIME),
	DEFINE_FIELD(CTripmineGrenade, m_vecDir, FIELD_VECTOR),
	DEFINE_FIELD(CTripmineGrenade, m_vecEnd, FIELD_POSITION_VECTOR),
	DEFINE_FIELD(CTripmineGrenade, m_flBeamLength, FIELD_FLOAT),
	DEFINE_FIELD(CTripmineGrenade, m_hOwner, FIELD_EHANDLE),
	DEFINE_FIELD(CTripmineGrenade, m_pBeam, FIELD_CLASSPTR),
	DEFINE_FIELD(CTripmineGrenade, m_posOwner, FIELD_POSITION_VECTOR),
	DEFINE_FIELD(CTripmineGrenade, m_angleOwner, FIELD_VECTOR),
	DEFINE_FIELD(CTripmineGrenade, m_pRealOwner, FIELD_EDICT),
};

IMPLEMENT_SAVERESTORE(CTripmineGrenade, CGrenade)

void CTripmineGrenade::Spawn(void)
{
	Precache();

	// motor
	pev->movetype = MOVETYPE_FLY;
	pev->solid = SOLID_NOT;

	SET_MODEL(ENT(pev), "models/v_tripmine.mdl");
	pev->frame = 0;
	pev->body = 3;
	pev->sequence = TRIPMINE_WORLD;
	ResetSequenceInfo();
	pev->framerate = 0;

	UTIL_SetSize(pev, Vector(-8, -8, -8), Vector(8, 8, 8));
	UTIL_SetOrigin(pev, pev->origin);

	if ( pev->spawnflags & 1 )
	{
		// power up quickly
		m_flPowerUp = gpGlobals->time + 1.0f;
	}
	else
	{
		// power up in 2.5 seconds
		m_flPowerUp = gpGlobals->time + 2.5f;
	}

	SetThink(&CTripmineGrenade::PowerupThink);
	pev->nextthink = gpGlobals->time + 0.2f;

	pev->takedamage = DAMAGE_YES;
	pev->dmg = gSkillData.plrDmgTripmine;
	pev->health = 1;  // don't let die normally

	if ( pev->owner != NULL )
	{
		// play deploy sound
		EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/mine_deploy.wav", 1.0f, ATTN_NORM);
		EMIT_SOUND(ENT(pev), CHAN_BODY, "weapons/mine_charge.wav", 0.2f, ATTN_NORM);  // chargeup

		m_pRealOwner = pev->owner;  // see CTripmineGrenade for why.
	}

	UTIL_MakeAimVectors(pev->angles);

	m_vecDir = gpGlobals->v_forward;
	m_vecEnd = pev->origin + m_vecDir * 2048;
}

void CTripmineGrenade::Precache(void)
{
	PRECACHE_MODEL("models/v_tripmine.mdl");
	PRECACHE_SOUND("weapons/mine_deploy.wav");
	PRECACHE_SOUND("weapons/mine_activate.wav");
	PRECACHE_SOUND("weapons/mine_charge.wav");
}

void CTripmineGrenade::UpdateOnRemove()
{
	CBaseEntity::UpdateOnRemove();

	KillBeam();
}

void CTripmineGrenade::WarningThink(void)
{
	// play warning sound
	// EMIT_SOUND( ENT( pev ), CHAN_VOICE, "buttons/Blip2.wav", 1.0, ATTN_NORM );

	// set to power up
	SetThink(&CTripmineGrenade::PowerupThink);
	pev->nextthink = gpGlobals->time + 1.0f;
}

void CTripmineGrenade::PowerupThink(void)
{
	TraceResult tr;

	if ( m_hOwner == 0 )
	{
		// find an owner
		edict_t* oldowner = pev->owner;
		pev->owner = NULL;
		UTIL_TraceLine(pev->origin + m_vecDir * 8, pev->origin - m_vecDir * 32, dont_ignore_monsters, ENT(pev), &tr);
		if ( tr.fStartSolid || (oldowner && tr.pHit == oldowner) )
		{
			pev->owner = oldowner;
			m_flPowerUp += 0.1f;
			pev->nextthink = gpGlobals->time + 0.1f;
			return;
		}
		if ( tr.flFraction < 1.0 )
		{
			pev->owner = tr.pHit;
			m_hOwner = CBaseEntity::Instance(pev->owner);
			m_posOwner = m_hOwner->pev->origin;
			m_angleOwner = m_hOwner->pev->angles;
		}
		else
		{
			STOP_SOUND(ENT(pev), CHAN_VOICE, "weapons/mine_deploy.wav");
			STOP_SOUND(ENT(pev), CHAN_BODY, "weapons/mine_charge.wav");
			SetThink(&CBaseEntity::SUB_Remove);
			pev->nextthink = gpGlobals->time + 0.1f;
			ALERT(
				at_console,
				"WARNING:Tripmine at %.0f, %.0f, %.0f removed\n",
				pev->origin.x,
				pev->origin.y,
				pev->origin.z);
			KillBeam();
			return;
		}
	}
	else if ( m_posOwner != m_hOwner->pev->origin || m_angleOwner != m_hOwner->pev->angles )
	{
		// disable
		STOP_SOUND(ENT(pev), CHAN_VOICE, "weapons/mine_deploy.wav");
		STOP_SOUND(ENT(pev), CHAN_BODY, "weapons/mine_charge.wav");
		CBaseEntity* pMine = Create("weapon_tripmine", pev->origin + m_vecDir * 24, pev->angles);
		pMine->pev->spawnflags |= SF_NORESPAWN;

		SetThink(&CBaseEntity::SUB_Remove);
		KillBeam();
		pev->nextthink = gpGlobals->time + 0.1f;
		return;
	}
	// ALERT( at_console, "%d %.0f %.0f %0.f\n", pev->owner, m_pOwner->pev->origin.x, m_pOwner->pev->origin.y,
	// m_pOwner->pev->origin.z );

	if ( gpGlobals->time > m_flPowerUp )
	{
		// make solid
		pev->solid = SOLID_BBOX;
		UTIL_SetOrigin(pev, pev->origin);

		MakeBeam();

		// play enabled sound
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "weapons/mine_activate.wav", 0.5, ATTN_NORM, 1, 75);
	}
	pev->nextthink = gpGlobals->time + 0.1f;
}

void CTripmineGrenade::KillBeam(void)
{
	if ( m_pBeam )
	{
		UTIL_Remove(m_pBeam);
		m_pBeam = NULL;
	}
}

void CTripmineGrenade::MakeBeam(void)
{
	TraceResult tr;

	// ALERT( at_console, "serverflags %f\n", gpGlobals->serverflags );

	UTIL_TraceLine(pev->origin, m_vecEnd, dont_ignore_monsters, ENT(pev), &tr);

	m_flBeamLength = tr.flFraction;

	// set to follow laser spot
	SetThink(&CTripmineGrenade::BeamBreakThink);
	pev->nextthink = gpGlobals->time + 0.1f;

	Vector vecTmpEnd = pev->origin + m_vecDir * 2048 * m_flBeamLength;

	m_pBeam = CBeam::BeamCreate(g_pModelNameLaser, 10);
	m_pBeam->PointEntInit(vecTmpEnd, entindex());
	m_pBeam->SetColor(0, 214, 198);
	m_pBeam->SetScrollRate(255);
	m_pBeam->SetBrightness(64);
}

void CTripmineGrenade::BeamBreakThink(void)
{
	BOOL bBlowup = 0;

	TraceResult tr;

	// HACKHACK Set simple box using this really nice global!
	gpGlobals->trace_flags = FTRACE_SIMPLEBOX;
	UTIL_TraceLine(pev->origin, m_vecEnd, dont_ignore_monsters, ENT(pev), &tr);

	// ALERT( at_console, "%f : %f\n", tr.flFraction, m_flBeamLength );

	// respawn detect.
	if ( !m_pBeam )
	{
		MakeBeam();
		if ( tr.pHit )
			m_hOwner = CBaseEntity::Instance(tr.pHit);  // reset owner too
	}

	if ( fabs(m_flBeamLength - tr.flFraction) > 0.001 )
	{
		bBlowup = 1;
	}
	else
	{
		if ( m_hOwner == 0 )
			bBlowup = 1;
		else if ( m_posOwner != m_hOwner->pev->origin )
			bBlowup = 1;
		else if ( m_angleOwner != m_hOwner->pev->angles )
			bBlowup = 1;
	}

	if ( bBlowup )
	{
		// a bit of a hack, but all CGrenade code passes pev->owner along to make sure the proper player gets credit for
		// the kill so we have to restore pev->owner from pRealOwner, because an entity's tracelines don't strike it's
		// pev->owner which meant that a player couldn't trigger his own tripmine. Now that the mine is exploding, it's
		// safe the restore the owner so the CGrenade code knows who the explosive really belongs to.
		pev->owner = m_pRealOwner;
		pev->health = 0;
		Killed(VARS(pev->owner), GIB_NORMAL);
		return;
	}

	pev->nextthink = gpGlobals->time + 0.1f;
}

int CTripmineGrenade::TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	if ( gpGlobals->time < m_flPowerUp && flDamage < pev->health )
	{
		// disable
		// Create( "weapon_tripmine", pev->origin + m_vecDir * 24, pev->angles );
		SetThink(&CBaseEntity::SUB_Remove);
		pev->nextthink = gpGlobals->time + 0.1f;
		KillBeam();
		return FALSE;
	}
	return CGrenade::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);
}

void CTripmineGrenade::Killed(entvars_t* pevAttacker, int)
{
	pev->takedamage = DAMAGE_NO;

	if ( pevAttacker && (pevAttacker->flags & FL_CLIENT) )
	{
		// some client has destroyed this mine, he'll get credit for any kills
		pev->owner = ENT(pevAttacker);
	}

	SetThink(&CTripmineGrenade::DelayDeathThink);
	pev->nextthink = gpGlobals->time + RANDOM_FLOAT(0.1f, 0.3f);

	EMIT_SOUND(ENT(pev), CHAN_BODY, "common/null.wav", 0.5, ATTN_NORM);  // shut off chargeup
}

void CTripmineGrenade::DelayDeathThink(void)
{
	KillBeam();
	TraceResult tr;
	UTIL_TraceLine(pev->origin + m_vecDir * 8, pev->origin - m_vecDir * 64, dont_ignore_monsters, ENT(pev), &tr);

	Explode(&tr, DMG_BLAST);
}
#endif

LINK_ENTITY_TO_CLASS(weapon_tripmine, CTripmine)

void CTripmine::Spawn()
{
	Precache();
	m_iId = WEAPON_TRIPMINE;
	SET_MODEL(ENT(pev), "models/v_tripmine.mdl");
	pev->frame = 0;
	pev->body = 3;
	pev->sequence = TRIPMINE_GROUND;
	// ResetSequenceInfo();
	pev->framerate = 0;

	FallInit();  // get ready to fall down

	m_iDefaultAmmo = TRIPMINE_DEFAULT_GIVE;

#ifdef CLIENT_DLL
	if ( !bIsMultiplayer() )
#else
	if ( !g_pGameRules->IsDeathmatch() )
#endif
	{
		UTIL_SetSize(pev, Vector(-16, -16, 0), Vector(16, 16, 28));
	}
}

void CTripmine::Precache(void)
{
	PRECACHE_MODEL("models/v_tripmine.mdl");
	PRECACHE_MODEL("models/p_tripmine.mdl");
	UTIL_PrecacheOther("monster_tripmine");

	m_usTripFire = PRECACHE_EVENT(1, "events/tripfire.sc");
}

int CTripmine::GetItemInfo(ItemInfo* p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = AmmoDef_Tripmine.AmmoName;
	p->iMaxAmmo1 = AmmoDef_Tripmine.MaxCarry;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 4;
	p->iPosition = 2;
	p->iId = m_iId = WEAPON_TRIPMINE;
	p->iWeight = TRIPMINE_WEIGHT;
	p->iFlags = ITEM_FLAG_LIMITINWORLD | ITEM_FLAG_EXHAUSTIBLE;

	return 1;
}

BOOL CTripmine::Deploy()
{
	pev->body = 0;
	return DefaultDeploy("models/v_tripmine.mdl", "models/p_tripmine.mdl", TRIPMINE_DRAW, "trip");
}

void CTripmine::Holster(int)
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;

	if ( !m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] )
	{
		// out of mines
		m_pPlayer->pev->weapons &= ~(1 << WEAPON_TRIPMINE);
		DestroyItem();
	}

	SendWeaponAnim(TRIPMINE_HOLSTER);
	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "common/null.wav", 1.0, ATTN_NORM);
}

void CTripmine::PrimaryAttack(void)
{
	if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
		return;

	UTIL_MakeVectors(Vector(m_pPlayer->pev->v_angle) + Vector(m_pPlayer->pev->punchangle));
	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecAiming = gpGlobals->v_forward;

	TraceResult tr;

	UTIL_TraceLine(vecSrc, vecSrc + vecAiming * 128, dont_ignore_monsters, ENT(m_pPlayer->pev), &tr);

	int flags;
#ifdef CLIENT_WEAPONS
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	using namespace EventConstructor;
	CEventConstructor event;

	event << Flags(flags) << Invoker(m_pPlayer->edict()) << EventIndex(m_usTripFire);

	event.Send();

	if ( tr.flFraction < 1.0 )
	{
		CBaseEntity* pEntity = CBaseEntity::Instance(tr.pHit);
		if ( pEntity && !(pEntity->pev->flags & FL_CONVEYOR) )
		{
			Vector angles = UTIL_VecToAngles(tr.vecPlaneNormal);

			CBaseEntity::Create(
				"monster_tripmine",
				Vector(tr.vecEndPos) + Vector(tr.vecPlaneNormal) * 8,
				angles,
				m_pPlayer->edict());

			m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;

			// player "shoot" animation
			m_pPlayer->SetAnimation(PLAYER_ATTACK1);

			if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 )
			{
				// no more mines!
				RetireWeapon();
				return;
			}
		}
		/*else
		{
			// ALERT( at_console, "no deploy\n" );
		}*/
	}
	/*else
	{

	}*/

	m_flNextPrimaryAttack = GetNextAttackDelay(0.3f);
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
}

void CTripmine::WeaponIdle(void)
{
	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] > 0 )
	{
		SendWeaponAnim(TRIPMINE_DRAW);
	}
	else
	{
		RetireWeapon();
		return;
	}

	int iAnim;
	float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1);
	if ( flRand <= 0.25 )
	{
		iAnim = TRIPMINE_IDLE1;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 90.0f / 30.0f;
	}
	else if ( flRand <= 0.75 )
	{
		iAnim = TRIPMINE_IDLE2;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 60.0f / 30.0f;
	}
	else
	{
		iAnim = TRIPMINE_FIDGET;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 100.0f / 30.0f;
	}

	SendWeaponAnim(iAnim);
}
