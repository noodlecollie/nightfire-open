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
#pragma once
#ifndef FUNC_BREAK_H
#define FUNC_BREAK_H

#include "resources/SurfaceSoundResources.h"
#include "resources/MaterialBreakSoundResources.h"

typedef enum
{
	expRandom,
	expDirected
} Explosions;
typedef enum
{
	matGlass = 0,
	matWood,
	matMetal,
	matFlesh,
	matCinderBlock,
	matCeilingTile,
	matComputer,
	matUnbreakableGlass,
	matRocks,
	matNone,
	matLastMaterial
} Materials;

#define NUM_SHARDS 6  // this many shards spawned when breakable objects break;

class CBreakable : public CBaseDelay
{
public:
	// basic functions
	void Spawn(void);
	void Precache(void);
	void KeyValue(KeyValueData* pkvd);
	void EXPORT BreakTouch(CBaseEntity* pOther);
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
	void DamageSound(void);

	// breakables use an overridden takedamage
	virtual int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);
	// To spark when hit
	void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, const TraceResult* ptr, int bitsDamageType);

	BOOL IsBreakable(void);

	int DamageDecal(int bitsDamageType);

	void EXPORT Die(void);
	virtual int ObjectCaps(void)
	{
		return (CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION);
	}
	virtual int Save(CSave& save);
	virtual int Restore(CRestore& restore);

	inline BOOL Explodable(void)
	{
		return ExplosionMagnitude() > 0;
	}
	inline int ExplosionMagnitude(void)
	{
		return pev->impulse;
	}
	inline void ExplosionSetMagnitude(int magnitude)
	{
		pev->impulse = magnitude;
	}

	static void MaterialSoundRandom(edict_t* pEdict, Materials soundMaterial, float volume);

	static const char* pSoundsWood[];
	static const char* pSoundsFlesh[];
	static const char* pSoundsGlass[];
	static const char* pSoundsMetal[];
	static const char* pSoundsConcrete[];
	static const char* pSpawnObjects[];

	static TYPEDESCRIPTION m_SaveData[];

	Materials m_Material;
	Explosions m_Explosion;
	int m_idShard;
	float m_angle;
	string_t m_iszGibModel;
	string_t m_iszSpawnObject;

private:
	static SurfaceSoundId SurfaceSoundIdForMaterial(Materials material);
	static MaterialBreakSoundId BreakSoundIdForMaterial(Materials material);
};
#endif  // FUNC_BREAK_H
