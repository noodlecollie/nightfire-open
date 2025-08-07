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

===== bmodels.cpp ========================================================

  spawn, think, and use functions for entities that use brush models

*/
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "saverestore.h"
#include "func_break.h"
#include "decals.h"
#include "explode.h"
#include "resources/SoundResources.h"
#include "PlatformLib/String.h"
#include "MathLib/angles.h"

extern DLL_GLOBAL Vector g_vecAttackDir;

// =================== FUNC_Breakable ==============================================

// Just add more items to the bottom of this array and they will automagically be supported
// This is done instead of just a classname in the FGD so we can control which entities can
// be spawned, and still remain fairly flexible
const char* CBreakable::pSpawnObjects[] = {
	NULL,  // 0
	"item_battery",  // 1
	"item_healthkit",  // 2
};

void CBreakable::KeyValue(KeyValueData* pkvd)
{
	// UNDONE_WC: explicitly ignoring these fields, but they shouldn't be in the map file!
	if ( FStrEq(pkvd->szKeyName, "explosion") )
	{
		if ( !PlatformLib_StrCaseCmp(pkvd->szValue, "directed") )
			m_Explosion = expDirected;
		else if ( !PlatformLib_StrCaseCmp(pkvd->szValue, "random") )
			m_Explosion = expRandom;
		else
			m_Explosion = expRandom;

		pkvd->fHandled = TRUE;
	}
	else if ( FStrEq(pkvd->szKeyName, "material") )
	{
		int i = atoi(pkvd->szValue);

		// 0:glass, 1:metal, 2:flesh, 3:wood

		if ( (i < 0) || (i >= matLastMaterial) )
			m_Material = matWood;
		else
			m_Material = (Materials)i;

		pkvd->fHandled = TRUE;
	}
	else if ( FStrEq(pkvd->szKeyName, "deadmodel") )
	{
		pkvd->fHandled = TRUE;
	}
	else if ( FStrEq(pkvd->szKeyName, "shards") )
	{
		// m_iShards = atof( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if ( FStrEq(pkvd->szKeyName, "gibmodel") )
	{
		m_iszGibModel = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if ( FStrEq(pkvd->szKeyName, "spawnobject") )
	{
		int object = atoi(pkvd->szValue);
		if ( object > 0 && object < (int)SIZE_OF_ARRAY(pSpawnObjects) )
			m_iszSpawnObject = MAKE_STRING(pSpawnObjects[object]);
		pkvd->fHandled = TRUE;
	}
	else if ( FStrEq(pkvd->szKeyName, "explodemagnitude") )
	{
		ExplosionSetMagnitude(atoi(pkvd->szValue));
		pkvd->fHandled = TRUE;
	}
	else if ( FStrEq(pkvd->szKeyName, "lip") )
		pkvd->fHandled = TRUE;
	else
		CBaseDelay::KeyValue(pkvd);
}

//
// func_breakable - bmodel that breaks into pieces after taking damage
//
LINK_ENTITY_TO_CLASS(func_breakable, CBreakable)

TYPEDESCRIPTION CBreakable::m_SaveData[] = {
	DEFINE_FIELD(CBreakable, m_Material, FIELD_INTEGER),
	DEFINE_FIELD(CBreakable, m_Explosion, FIELD_INTEGER),

	// Don't need to save/restore these because we precache after restore
	//	DEFINE_FIELD( CBreakable, m_idShard, FIELD_INTEGER ),

	DEFINE_FIELD(CBreakable, m_angle, FIELD_FLOAT),
	DEFINE_FIELD(CBreakable, m_iszGibModel, FIELD_STRING),
	DEFINE_FIELD(CBreakable, m_iszSpawnObject, FIELD_STRING),

	// Explosion magnitude is stored in pev->impulse
};

IMPLEMENT_SAVERESTORE(CBreakable, CBaseEntity)

void CBreakable::Spawn(void)
{
	Precache();

	if ( FBitSet(pev->spawnflags, SF_BREAK_TRIGGER_ONLY) )
	{
		pev->takedamage = DAMAGE_NO;
	}
	else
	{
		pev->takedamage = DAMAGE_YES;
	}

	pev->solid = SOLID_BSP;
	pev->movetype = MOVETYPE_PUSH;
	m_angle = pev->angles[YAW];
	pev->angles[YAW] = 0;

	// HACK:  matGlass can receive decals, we need the client to know about this
	//  so use class to store the material flag
	if ( m_Material == matGlass )
	{
		pev->playerclass = 1;
	}

	SET_MODEL(ENT(pev), STRING(pev->model));  // set size and link into world.

	SetTouch(&CBreakable::BreakTouch);

	if ( FBitSet(pev->spawnflags, SF_BREAK_TRIGGER_ONLY) )  // Only break on trigger
	{
		SetTouch(nullptr);
	}

	// Flag unbreakable glass as "worldbrush" so it will block ALL tracelines
	if ( !IsBreakable() && pev->rendermode != kRenderNormal )
	{
		pev->flags |= FL_WORLDBRUSH;
	}
}

void CBreakable::MaterialSoundRandom(edict_t* pEdict, Materials soundMaterial, float volume)
{
	const char* soundPath = SoundResources::SurfaceSounds.RandomResourcePath(SurfaceSoundIdForMaterial(soundMaterial));

	if ( soundPath )
	{
		EMIT_SOUND(pEdict, CHAN_BODY, soundPath, volume, 1.0);
	}
}

void CBreakable::Precache(void)
{
	const char* pGibName = NULL;

	switch ( m_Material )
	{
		case matWood:
			pGibName = "models/woodgibs.mdl";
			break;
		case matFlesh:
			pGibName = "models/fleshgibs.mdl";
			break;
		case matComputer:
			pGibName = "models/computergibs.mdl";
			break;
		case matUnbreakableGlass:
		case matGlass:
			pGibName = "models/glassgibs.mdl";
			break;
		case matMetal:
			pGibName = "models/metalplategibs.mdl";
			break;
		case matCinderBlock:
			pGibName = "models/cindergibs.mdl";
			break;
		case matRocks:
			pGibName = "models/rockgibs.mdl";
			break;
		case matCeilingTile:
			pGibName = "models/ceilinggibs.mdl";
			break;
		case matNone:
		case matLastMaterial:
			break;
		default:
			break;
	}

	if ( m_iszGibModel )
	{
		pGibName = STRING(m_iszGibModel);
	}

	m_idShard = PRECACHE_MODEL(pGibName);

	// Precache the spawn item's data
	if ( m_iszSpawnObject )
	{
		UTIL_PrecacheOther(STRING(m_iszSpawnObject));
	}
}

// play shard sound when func_breakable takes damage.
// the more damage, the louder the shard sound.
void CBreakable::DamageSound(void)
{
	int pitch = 100;
	float fvol = 1.0f;
	Materials material = m_Material;

	if ( RANDOM_LONG(0, 2) )
	{
		pitch = PITCH_NORM;
	}
	else
	{
		pitch = 95 + RANDOM_LONG(0, 34);
	}

	fvol = RANDOM_FLOAT(0.75, 1.0);

	if ( material == matComputer && RANDOM_LONG(0, 1) )
	{
		material = matMetal;
	}

	const char* soundPath = SoundResources::SurfaceSounds.RandomResourcePath(SurfaceSoundIdForMaterial(material));

	if ( soundPath )
	{
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, soundPath, fvol, ATTN_NORM, 0, pitch);
	}
}

void CBreakable::BreakTouch(CBaseEntity* pOther)
{
	float flDamage;
	entvars_t* pevToucher = pOther->pev;

	// only players can break these right now
	if ( !pOther->IsPlayer() || !IsBreakable() )
	{
		return;
	}

	if ( FBitSet(pev->spawnflags, SF_BREAK_TOUCH) )
	{
		// can be broken when run into
		flDamage = VectorLength(pevToucher->velocity) * 0.01f;

		if ( flDamage >= pev->health )
		{
			SetTouch(nullptr);
			TakeDamage(pevToucher, pevToucher, flDamage, DMG_CRUSH);

			// do a little damage to player if we broke glass or computer
			pOther->TakeDamage(pev, pev, flDamage / 4, DMG_SLASH);
		}
	}

	if ( FBitSet(pev->spawnflags, SF_BREAK_PRESSURE) && pevToucher->absmin[VEC3_Z] >= pev->maxs[VEC3_Z] - 2 )
	{
		// can be broken when stood upon
		// play creaking sound here.
		DamageSound();

		SetThink(&CBreakable::Die);
		SetTouch(NULL);

		if ( m_flDelay == 0 )
		{
			// !!!BUGBUG - why doesn't zero delay work?
			m_flDelay = 0.1f;
		}

		pev->nextthink = pev->ltime + m_flDelay;
	}
}

//
// Smash the our breakable object
//

// Break when triggered
void CBreakable::Use(CBaseEntity*, CBaseEntity*, USE_TYPE, float)
{
	if ( IsBreakable() )
	{
		pev->angles[YAW] = m_angle;
		UTIL_MakeVectors(pev->angles);
		g_vecAttackDir = gpGlobals->v_forward;

		Die();
	}
}

void CBreakable::TraceAttack(
	entvars_t* pevAttacker,
	float flDamage,
	Vector vecDir,
	const TraceResult* ptr,
	int bitsDamageType)
{
	// random spark if this is a 'computer' object
	if ( RANDOM_LONG(0, 1) )
	{
		switch ( m_Material )
		{
			case matComputer:
			{
				UTIL_Sparks(ptr->vecEndPos);

				EMIT_SOUND(
					ENT(pev),
					CHAN_VOICE,
					SoundResources::ItemSounds.RandomResourcePath(ItemSoundId::Spark),
					RANDOM_FLOAT(0.7f, 1.0f),
					ATTN_NORM);
			}
			break;
			case matUnbreakableGlass:
				UTIL_Ricochet(ptr->vecEndPos, RANDOM_FLOAT(0.5, 1.5));
				break;
			default:
				break;
		}
	}

	CBaseDelay::TraceAttack(pevAttacker, flDamage, vecDir, ptr, bitsDamageType);
}

//=========================================================
// Special takedamage for func_breakable. Allows us to make
// exceptions that are breakable-specific
// bitsDamageType indicates the type of damage sustained ie: DMG_CRUSH
//=========================================================
int CBreakable::TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	Vector vecTemp;

	// if Attacker == Inflictor, the attack was a melee or other instant-hit attack.
	// (that is, no actual entity projectile was involved in the attack so use the shooter's origin).
	if ( pevAttacker == pevInflictor )
	{
		vecTemp = Vector(pevInflictor->origin) - (Vector(pev->absmin) + (Vector(pev->size) * 0.5));

		// if a client hit the breakable with a crowbar, and breakable is crowbar-sensitive, break it now.
		if ( FBitSet(pevAttacker->flags, FL_CLIENT) && FBitSet(pev->spawnflags, SF_BREAK_CROWBAR) &&
			 (bitsDamageType & DMG_CLUB) )
		{
			flDamage = pev->health;
		}
	}
	else
	// an actual missile was involved.
	{
		vecTemp = Vector(pevInflictor->origin) - (Vector(pev->absmin) + (Vector(pev->size) * 0.5));
	}

	if ( !IsBreakable() )
	{
		return 0;
	}

	// Breakables take double damage from the crowbar
	if ( bitsDamageType & DMG_CLUB )
	{
		flDamage *= 2;
	}

	// Boxes / glass / etc. don't take much poison damage, just the impact of the dart - consider that 10%
	if ( bitsDamageType & DMG_POISON )
	{
		flDamage *= 0.1f;
	}

	// this global is still used for glass and other non-monster killables, along with decals.
	g_vecAttackDir = vecTemp.Normalize();

	// do the damage
	pev->health -= flDamage;
	if ( pev->health <= 0 )
	{
		Killed(pevAttacker, GIB_NORMAL);
		Die();
		return 0;
	}

	// Make a shard noise each time func breakable is hit.
	// Don't play shard noise if cbreakable actually died.
	DamageSound();

	return 1;
}

void CBreakable::Die(void)
{
	Vector vecSpot;  // shard origin
	Vector vecVelocity;  // shard velocity
	char cFlag = 0;
	int pitch;
	float fvol;

	pitch = 95 + RANDOM_LONG(0, 29);

	if ( pitch > 97 && pitch < 103 )
		pitch = 100;

	// The more negative pev->health, the louder
	// the sound should be.

	fvol = RANDOM_FLOAT(0.85f, 1.0f) + (fabsf(pev->health) / 100.0f);

	if ( fvol > 1.0 )
	{
		fvol = 1.0;
	}

	const char* breakSound = SoundResources::BreakSounds.RandomResourcePath(BreakSoundIdForMaterial(m_Material));

	if ( breakSound )
	{
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, breakSound, fvol, ATTN_NORM, 0, pitch);
	}

	if ( m_Explosion == expDirected )
	{
		vecVelocity = g_vecAttackDir * 200;
	}
	else
	{
		vecVelocity.x = 0;
		vecVelocity.y = 0;
		vecVelocity.z = 0;
	}

	vecSpot = Vector(pev->origin) + (Vector(pev->mins) + Vector(pev->maxs)) * 0.5;

	MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, vecSpot);
	WRITE_BYTE(TE_BREAKMODEL);

	// position
	WRITE_COORD(vecSpot.x);
	WRITE_COORD(vecSpot.y);
	WRITE_COORD(vecSpot.z);

	// size
	WRITE_COORD(pev->size[0]);
	WRITE_COORD(pev->size[1]);
	WRITE_COORD(pev->size[2]);

	// velocity
	WRITE_COORD(vecVelocity.x);
	WRITE_COORD(vecVelocity.y);
	WRITE_COORD(vecVelocity.z);

	// randomization
	WRITE_BYTE(10);

	// Model
	WRITE_SHORT(m_idShard);  // model id#

	// # of shards
	WRITE_BYTE(0);  // let client decide

	// duration
	WRITE_BYTE(25);  // 2.5 seconds

	// flags
	WRITE_BYTE(cFlag);
	MESSAGE_END();

	float size = pev->size[VEC3_X];

	if ( size < pev->size[VEC3_Y] )
	{
		size = pev->size[VEC3_Y];
	}

	if ( size < pev->size[VEC3_Z] )
	{
		size = pev->size[VEC3_Z];
	}

	// !!! HACK  This should work!
	// Build a box above the entity that looks like an 8 pixel high sheet
	Vector mins = pev->absmin;
	Vector maxs = pev->absmax;
	mins.z = pev->absmax[VEC3_Z];
	maxs.z += 8;

	// BUGBUG -- can only find 256 entities on a breakable -- should be enough
	CBaseEntity* pList[256];
	int count = UTIL_EntitiesInBox(pList, 256, mins, maxs, FL_ONGROUND);
	if ( count )
	{
		for ( int i = 0; i < count; i++ )
		{
			ClearBits(pList[i]->pev->flags, FL_ONGROUND);
			pList[i]->pev->groundentity = NULL;
		}
	}

	// Don't fire something that could fire myself
	pev->targetname = 0;

	pev->solid = SOLID_NOT;

	// Fire targets on break
	SUB_UseTargets(NULL, USE_TOGGLE, 0);

	SetThink(&CBaseEntity::SUB_Remove);
	pev->nextthink = pev->ltime + 0.1f;

	if ( m_iszSpawnObject )
	{
		CBaseEntity::Create(STRING(m_iszSpawnObject), VecBModelOrigin(pev), pev->angles, edict());
	}

	if ( Explodable() )
	{
		ExplosionCreate(Center(), pev->angles, edict(), ExplosionMagnitude(), TRUE);
	}
}

BOOL CBreakable::IsBreakable(void)
{
	return m_Material != matUnbreakableGlass;
}

int CBreakable::DamageDecal(int bitsDamageType)
{
	if ( m_Material == matGlass )
		return DECAL_GLASSBREAK1 + RANDOM_LONG(0, 2);

	if ( m_Material == matUnbreakableGlass )
		return DECAL_BPROOF1;

	return CBaseEntity::DamageDecal(bitsDamageType);
}

SurfaceSoundId CBreakable::SurfaceSoundIdForMaterial(Materials material)
{
	switch ( material )
	{
		case matGlass:
		case matComputer:
		{
			return SurfaceSoundId::HitGlassB;
		}

		case matWood:
		{
			return SurfaceSoundId::HitWood;
		}

		case matMetal:
		{
			return SurfaceSoundId::HitMetal;
		}

		case matFlesh:
		{
			return SurfaceSoundId::HitFlesh;
		}

		case matCinderBlock:
		case matRocks:
		{
			return SurfaceSoundId::HitConcrete;
		}

		case matCeilingTile:
		{
			return SurfaceSoundId::HitPlaster;
		}

		case matUnbreakableGlass:
		{
			return SurfaceSoundId::HitGlassA;
		}

		default:
		{
			return SurfaceSoundId::HitNone;
		}
	}
}

MaterialBreakSoundId CBreakable::BreakSoundIdForMaterial(Materials material)
{
	switch ( material )
	{
		case matUnbreakableGlass:
		case matGlass:
		{
			return MaterialBreakSoundId::BreakGlass;
		}

		case matWood:
		{
			return MaterialBreakSoundId::BreakWood;
		}

		case matComputer:
		case matMetal:
		{
			return MaterialBreakSoundId::BreakMetal;
		}

		case matFlesh:
		{
			return MaterialBreakSoundId::BreakFlesh;
		}

		case matCinderBlock:
		case matRocks:
		case matCeilingTile:
		{
			return MaterialBreakSoundId::BreakConcrete;
		}

		default:
		{
			return MaterialBreakSoundId::BreakNone;
		}
	}
}

class CPushable : public CBreakable
{
public:
	void Spawn(void);
	void Precache(void);
	void Touch(CBaseEntity* pOther);
	void Move(CBaseEntity* pMover, int push);
	void KeyValue(KeyValueData* pkvd);
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
	void EXPORT StopSound(void);
	// virtual void	SetActivator( CBaseEntity *pActivator ) { m_pPusher = pActivator; }

	virtual int ObjectCaps(void)
	{
		return (CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION) | FCAP_CONTINUOUS_USE;
	}
	virtual int Save(CSave& save);
	virtual int Restore(CRestore& restore);

	inline float MaxSpeed(void)
	{
		return m_maxSpeed;
	}

	// breakables use an overridden takedamage
	virtual int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);

	static TYPEDESCRIPTION m_SaveData[];

	static const char* m_soundNames[3];
	int m_lastSound;  // no need to save/restore, just keeps the same sound from playing twice in a row
	float m_maxSpeed;
	float m_soundTime;
};

TYPEDESCRIPTION CPushable::m_SaveData[] = {
	DEFINE_FIELD(CPushable, m_maxSpeed, FIELD_FLOAT),
	DEFINE_FIELD(CPushable, m_soundTime, FIELD_TIME),
};

IMPLEMENT_SAVERESTORE(CPushable, CBreakable)

LINK_ENTITY_TO_CLASS(func_pushable, CPushable)

const char* CPushable::m_soundNames[3] = {"debris/pushbox1.wav", "debris/pushbox2.wav", "debris/pushbox3.wav"};

void CPushable::Spawn(void)
{
	if ( pev->spawnflags & SF_PUSH_BREAKABLE )
		CBreakable::Spawn();
	else
		Precache();

	pev->movetype = MOVETYPE_PUSHSTEP;
	pev->solid = SOLID_BBOX;
	SET_MODEL(ENT(pev), STRING(pev->model));

	if ( pev->friction > 399 )
		pev->friction = 399;

	m_maxSpeed = 400 - pev->friction;
	SetBits(pev->flags, FL_FLOAT);
	pev->friction = 0;

	pev->origin[VEC3_Z] += 1;  // Pick up off of the floor
	UTIL_SetOrigin(pev, pev->origin);

	// Multiply by area of the box's cross-section (assume 1000 units^3 standard volume)
	pev->skin =
		(int)((pev->skin * (pev->maxs[VEC3_X] - pev->mins[VEC3_X]) * (pev->maxs[VEC3_Y] - pev->mins[VEC3_Y])) * 0.0005);
	m_soundTime = 0;
}

void CPushable::Precache(void)
{
	for ( int i = 0; i < 3; i++ )
		PRECACHE_SOUND(m_soundNames[i]);

	if ( pev->spawnflags & SF_PUSH_BREAKABLE )
		CBreakable::Precache();
}

void CPushable::KeyValue(KeyValueData* pkvd)
{
	if ( FStrEq(pkvd->szKeyName, "size") )
	{
		int bbox = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;

		switch ( bbox )
		{
			case 0:
				// Point
				UTIL_SetSize(pev, Vector(-8, -8, -8), Vector(8, 8, 8));
				break;
			case 2:
				// Big Hull!?!?	!!!BUGBUG Figure out what this hull really is
				UTIL_SetSize(pev, VEC_DUCK_HULL_MIN * 2, VEC_DUCK_HULL_MAX * 2);
				break;
			case 3:
				// Player duck
				UTIL_SetSize(pev, VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX);
				break;
			default:
			case 1:
				// Player
				UTIL_SetSize(pev, VEC_HULL_MIN, VEC_HULL_MAX);
				break;
		}
	}
	else if ( FStrEq(pkvd->szKeyName, "buoyancy") )
	{
		pev->skin = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBreakable::KeyValue(pkvd);
}

// Pull the func_pushable
void CPushable::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	if ( !pActivator || !pActivator->IsPlayer() )
	{
		if ( pev->spawnflags & SF_PUSH_BREAKABLE )
			this->CBreakable::Use(pActivator, pCaller, useType, value);
		return;
	}

	if ( pActivator->pev->velocity != g_vecZero )
		Move(pActivator, 0);
}

void CPushable::Touch(CBaseEntity* pOther)
{
	if ( FClassnameIs(pOther->pev, "worldspawn") )
		return;

	Move(pOther, 1);
}

void CPushable::Move(CBaseEntity* pOther, int push)
{
	entvars_t* pevToucher = pOther->pev;
	int playerTouch = 0;

	// Is entity standing on this pushable ?
	if ( FBitSet(pevToucher->flags, FL_ONGROUND) && pevToucher->groundentity && VARS(pevToucher->groundentity) == pev )
	{
		// Only push if floating
		if ( pev->waterlevel > 0 )
		{
			pev->velocity[VEC3_Z] += pevToucher->velocity[VEC3_Z] * 0.1f;
		}

		return;
	}

	// g-cont. fix pushable acceleration bug (reverted as it used in mods)
	if ( pOther->IsPlayer() )
	{
		// Don't push unless the player is pushing forward and NOT use (pull)
		if ( push && !(pevToucher->button & (IN_FORWARD | IN_USE)) )
			return;
		playerTouch = 1;
	}

	float factor;

	if ( playerTouch )
	{
		if ( !(pevToucher->flags & FL_ONGROUND) )  // Don't push away from jumping/falling players unless in water
		{
			if ( pev->waterlevel < 1 )
			{
				return;
			}
			else
			{
				factor = 0.1f;
			}
		}
		else
		{
			factor = 1;
		}
	}
	else
	{
		factor = 0.25;
	}

	pev->velocity[VEC3_X] += pevToucher->velocity[VEC3_X] * factor;
	pev->velocity[VEC3_Y] += pevToucher->velocity[VEC3_Y] * factor;

	float length = sqrtf(pev->velocity[VEC3_X] * pev->velocity[VEC3_X] + pev->velocity[VEC3_Y] * pev->velocity[VEC3_Y]);

	if ( push && (length > MaxSpeed()) )
	{
		pev->velocity[VEC3_X] = (pev->velocity[VEC3_X] * MaxSpeed() / length);
		pev->velocity[VEC3_Y] = (pev->velocity[VEC3_Y] * MaxSpeed() / length);
	}

	if ( playerTouch )
	{
		pevToucher->velocity[VEC3_X] = pev->velocity[VEC3_X];
		pevToucher->velocity[VEC3_Y] = pev->velocity[VEC3_Y];

		if ( (gpGlobals->time - m_soundTime) > 0.7 )
		{
			m_soundTime = gpGlobals->time;
			if ( length > 0 && FBitSet(pev->flags, FL_ONGROUND) )
			{
				m_lastSound = RANDOM_LONG(0, 2);
				EMIT_SOUND(ENT(pev), CHAN_WEAPON, m_soundNames[m_lastSound], 0.5, ATTN_NORM);
				// SetThink( &StopSound );
				// pev->nextthink = pev->ltime + 0.1;
			}
			else
			{
				STOP_SOUND(ENT(pev), CHAN_WEAPON, m_soundNames[m_lastSound]);
			}
		}
	}
}

#if 0
void CPushable::StopSound( void )
{
	Vector dist = pev->oldorigin - pev->origin;
	if( dist.Length() <= 0 )
		STOP_SOUND( ENT( pev ), CHAN_WEAPON, m_soundNames[m_lastSound] );
}
#endif

int CPushable::TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	if ( pev->spawnflags & SF_PUSH_BREAKABLE )
		return CBreakable::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);

	return 1;
}
