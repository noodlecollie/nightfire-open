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
/*

===== explode.cpp ========================================================

  Explosion-related code

*/
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "decals.h"
#include "explode.h"
#include "PlatformLib/String.h"

// Spark Shower
class CShower : public CBaseEntity
{
	void Spawn(void);
	void Think(void);
	void Touch(CBaseEntity* pOther);
	int ObjectCaps(void)
	{
		return FCAP_DONT_SAVE;
	}
};

LINK_ENTITY_TO_CLASS(spark_shower, CShower)

void CShower::Spawn(void)
{
	VectorScale(pev->angles, RANDOM_FLOAT(200, 300), pev->velocity);
	pev->velocity[VEC3_X] += RANDOM_FLOAT(-100.f, 100.f);
	pev->velocity[VEC3_Y] += RANDOM_FLOAT(-100.f, 100.f);

	if ( pev->velocity[VEC3_Z] >= 0 )
	{
		pev->velocity[VEC3_Z] += 200;
	}
	else
	{
		pev->velocity[VEC3_Z] -= 200;
	}

	pev->movetype = MOVETYPE_BOUNCE;
	pev->gravity = 0.5;
	pev->nextthink = gpGlobals->time + 0.1f;
	pev->solid = SOLID_NOT;
	SET_MODEL(edict(), "models/grenade.mdl");  // Need a model, just use the grenade, we don't draw it anyway
	UTIL_SetSize(pev, g_vecZero, g_vecZero);
	pev->effects |= EF_NODRAW;
	pev->speed = RANDOM_FLOAT(0.5, 1.5);
	VectorClear(pev->angles);
}

void CShower::Think(void)
{
	UTIL_Sparks(pev->origin);

	pev->speed -= 0.1f;
	if ( pev->speed > 0 )
		pev->nextthink = gpGlobals->time + 0.1f;
	else
		UTIL_Remove(this);
	pev->flags &= ~FL_ONGROUND;
}

void CShower::Touch(CBaseEntity*)
{
	if ( pev->flags & FL_ONGROUND )
	{
		VectorScale(pev->velocity, 0.1f, pev->velocity);
	}
	else
	{
		VectorScale(pev->velocity, 0.6f, pev->velocity);
	}

	if ( (pev->velocity[VEC3_X] * pev->velocity[VEC3_X] + pev->velocity[VEC3_Y] * pev->velocity[VEC3_Y]) < 10.0 )
	{
		pev->speed = 0;
	}
}

class CEnvExplosion : public CBaseMonster
{
public:
	void Spawn();
	void EXPORT Smoke(void);
	void KeyValue(KeyValueData* pkvd);
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

	virtual int Save(CSave& save);
	virtual int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];

	int m_iMagnitude;  // how large is the fireball? how much damage?
	int m_spriteScale;  // what's the exact fireball sprite scale?
};

TYPEDESCRIPTION CEnvExplosion::m_SaveData[] = {
	DEFINE_FIELD(CEnvExplosion, m_iMagnitude, FIELD_INTEGER),
	DEFINE_FIELD(CEnvExplosion, m_spriteScale, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CEnvExplosion, CBaseMonster)
LINK_ENTITY_TO_CLASS(env_explosion, CEnvExplosion)

void CEnvExplosion::KeyValue(KeyValueData* pkvd)
{
	if ( FStrEq(pkvd->szKeyName, "iMagnitude") )
	{
		m_iMagnitude = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseEntity::KeyValue(pkvd);
}

void CEnvExplosion::Spawn(void)
{
	pev->solid = SOLID_NOT;
	pev->effects = EF_NODRAW;

	pev->movetype = MOVETYPE_NONE;
	/*
	if( m_iMagnitude > 250 )
	{
		m_iMagnitude = 250;
	}
	*/

	float flSpriteScale;
	flSpriteScale = (m_iMagnitude - 50) * 0.6f;

	/*
	if( flSpriteScale > 50 )
	{
		flSpriteScale = 50;
	}
	*/
	if ( flSpriteScale < 10 )
	{
		flSpriteScale = 10;
	}

	m_spriteScale = (int)flSpriteScale;
}

void CEnvExplosion::Use(CBaseEntity*, CBaseEntity*, USE_TYPE, float)
{
	TraceResult tr;

	pev->model = iStringNull;  // invisible
	pev->solid = SOLID_NOT;  // intangible

	Vector vecSpot;  // trace starts here!

	vecSpot = Vector(pev->origin) + Vector(0, 0, 8);

	UTIL_TraceLine(vecSpot, vecSpot + Vector(0, 0, -40), ignore_monsters, ENT(pev), &tr);

	// Pull out of the wall a bit
	if ( tr.flFraction != 1.0 )
	{
		(Vector(tr.vecEndPos) + (Vector(tr.vecPlaneNormal) * (static_cast<float>(m_iMagnitude - 24)) * 0.6f))
			.CopyToArray(pev->origin);
	}

	// draw decal
	if ( !(pev->spawnflags & SF_ENVEXPLOSION_NODECAL) )
	{
		if ( RANDOM_FLOAT(0, 1) < 0.5 )
		{
			UTIL_DecalTrace(&tr, DECAL_SCORCH1);
		}
		else
		{
			UTIL_DecalTrace(&tr, DECAL_SCORCH2);
		}
	}

	// draw fireball
	if ( !(pev->spawnflags & SF_ENVEXPLOSION_NOFIREBALL) )
	{
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pev->origin);
		WRITE_BYTE(TE_EXPLOSION);
		WRITE_COORD(pev->origin[0]);
		WRITE_COORD(pev->origin[1]);
		WRITE_COORD(pev->origin[2]);
		WRITE_SHORT(g_sModelIndexFireball);
		WRITE_BYTE((BYTE)m_spriteScale);  // scale * 10
		WRITE_BYTE(15);  // framerate
		WRITE_BYTE(TE_EXPLFLAG_NONE);
		MESSAGE_END();
	}
	else
	{
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pev->origin);
		WRITE_BYTE(TE_EXPLOSION);
		WRITE_COORD(pev->origin[0]);
		WRITE_COORD(pev->origin[1]);
		WRITE_COORD(pev->origin[2]);
		WRITE_SHORT(g_sModelIndexFireball);
		WRITE_BYTE(0);  // no sprite
		WRITE_BYTE(15);  // framerate
		WRITE_BYTE(TE_EXPLFLAG_NONE);
		MESSAGE_END();
	}

	// do damage
	if ( !(pev->spawnflags & SF_ENVEXPLOSION_NODAMAGE) )
	{
		RadiusDamage(pev, pev, static_cast<float>(m_iMagnitude), CLASS_NONE, DMG_BLAST);
	}

	SetThink(&CEnvExplosion::Smoke);
	pev->nextthink = gpGlobals->time + 0.3f;

	// draw sparks
	if ( !(pev->spawnflags & SF_ENVEXPLOSION_NOSPARKS) )
	{
		int sparkCount = RANDOM_LONG(0, 3);

		for ( int i = 0; i < sparkCount; i++ )
		{
			Create("spark_shower", pev->origin, tr.vecPlaneNormal, NULL);
		}
	}
}

void CEnvExplosion::Smoke(void)
{
	if ( !(pev->spawnflags & SF_ENVEXPLOSION_NOSMOKE) )
	{
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pev->origin);
		WRITE_BYTE(TE_SMOKE);
		WRITE_COORD(pev->origin[0]);
		WRITE_COORD(pev->origin[1]);
		WRITE_COORD(pev->origin[2]);
		WRITE_SHORT(g_sModelIndexSmoke);
		WRITE_BYTE((BYTE)m_spriteScale);  // scale * 10
		WRITE_BYTE(12);  // framerate
		MESSAGE_END();
	}

	if ( !(pev->spawnflags & SF_ENVEXPLOSION_REPEATABLE) )
	{
		UTIL_Remove(this);
	}
}

// HACKHACK -- create one of these and fake a keyvalue to get the right explosion setup
void ExplosionCreate(const Vector& center, const Vector& angles, edict_t* pOwner, int magnitude, BOOL doDamage)
{
	KeyValueData kvd;
	char buf[128];

	CBaseEntity* pExplosion = CBaseEntity::Create("env_explosion", center, angles, pOwner);
	PlatformLib_SNPrintF(buf, sizeof(buf), "%3d", magnitude);
	kvd.szKeyName = (char*)"iMagnitude";
	kvd.szValue = buf;
	pExplosion->KeyValue(&kvd);

	if ( !doDamage )
	{
		pExplosion->pev->spawnflags |= SF_ENVEXPLOSION_NODAMAGE;
	}

	pExplosion->Spawn();
	pExplosion->Use(NULL, NULL, USE_TOGGLE, 0);
}
