/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   This source code contains proprietary and confidential information of
 *   Valve LLC and its suppliers.  Access to this code is restricted to
 *   persons who have executed a written SDK license with Valve.  Any access,
 *   use or distribution of this code by or to any unlicensed person is illegal.
 *
 ****/
//=========================================================
//=========================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "squadmonster.h"
#include "MathLib/angles.h"

#define AFLOCK_MAX_RECRUIT_RADIUS 1024
#define AFLOCK_FLY_SPEED 125
#define AFLOCK_TURN_RATE 75
#define AFLOCK_ACCELERATE 10
#define AFLOCK_CHECK_DIST 192
#define AFLOCK_TOO_CLOSE 100
#define AFLOCK_TOO_FAR 256

//=========================================================
//=========================================================
class CFlockingFlyerFlock : public CBaseMonster
{
public:
	void Spawn(void);
	void Precache(void);
	void KeyValue(KeyValueData* pkvd);
	void SpawnFlock(void);

	virtual int Save(CSave& save);
	virtual int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];

	// Sounds are shared by the flock
	static void PrecacheFlockSounds(void);

	int m_cFlockSize;
	float m_flFlockRadius;
};

TYPEDESCRIPTION CFlockingFlyerFlock::m_SaveData[] = {
	DEFINE_FIELD(CFlockingFlyerFlock, m_cFlockSize, FIELD_INTEGER),
	DEFINE_FIELD(CFlockingFlyerFlock, m_flFlockRadius, FIELD_FLOAT),
};

IMPLEMENT_SAVERESTORE(CFlockingFlyerFlock, CBaseMonster)

//=========================================================
//=========================================================
class CFlockingFlyer : public CBaseMonster
{
public:
	void Spawn(void);
	void Precache(void);
	void SpawnCommonCode(void);
	void EXPORT IdleThink(void);
	void BoidAdvanceFrame(void);
	void EXPORT FormFlock(void);
	void EXPORT Start(void);
	void EXPORT FlockLeaderThink(void);
	void EXPORT FlockFollowerThink(void);
	void EXPORT FallHack(void);
	void MakeSound(void);
	void AlertFlock(void);
	void SpreadFlock(void);
	void SpreadFlock2(void);
	void Killed(entvars_t* pevAttacker, int iGib);
	void Poop(void);
	BOOL FPathBlocked(void);
	// void KeyValue( KeyValueData *pkvd );

	virtual int Save(CSave& save);
	virtual int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];

	int IsLeader(void)
	{
		return m_pSquadLeader == this;
	}
	int InSquad(void)
	{
		return m_pSquadLeader != NULL;
	}
	int SquadCount(void);
	void SquadRemove(CFlockingFlyer* pRemove);
	void SquadUnlink(void);
	void SquadAdd(CFlockingFlyer* pAdd);
	void SquadDisband(void);

	CFlockingFlyer* m_pSquadLeader;
	CFlockingFlyer* m_pSquadNext;
	BOOL m_fTurning;  // is this boid turning?
	BOOL m_fCourseAdjust;  // followers set this flag TRUE to override flocking while they avoid something
	BOOL m_fPathBlocked;  // TRUE if there is an obstacle ahead
	Vector m_vecReferencePoint;  // last place we saw leader
	Vector m_vecAdjustedVelocity;  // adjusted velocity (used when fCourseAdjust is TRUE)
	float m_flGoalSpeed;
	float m_flLastBlockedTime;
	float m_flFakeBlockedTime;
	float m_flAlertTime;
	float m_flFlockNextSoundTime;
};

LINK_ENTITY_TO_CLASS(monster_flyer, CFlockingFlyer)
LINK_ENTITY_TO_CLASS(monster_flyer_flock, CFlockingFlyerFlock)

TYPEDESCRIPTION CFlockingFlyer::m_SaveData[] = {
	DEFINE_FIELD(CFlockingFlyer, m_pSquadLeader, FIELD_CLASSPTR),
	DEFINE_FIELD(CFlockingFlyer, m_pSquadNext, FIELD_CLASSPTR),
	DEFINE_FIELD(CFlockingFlyer, m_fTurning, FIELD_BOOLEAN),
	DEFINE_FIELD(CFlockingFlyer, m_fCourseAdjust, FIELD_BOOLEAN),
	DEFINE_FIELD(CFlockingFlyer, m_fPathBlocked, FIELD_BOOLEAN),
	DEFINE_FIELD(CFlockingFlyer, m_vecReferencePoint, FIELD_POSITION_VECTOR),
	DEFINE_FIELD(CFlockingFlyer, m_vecAdjustedVelocity, FIELD_VECTOR),
	DEFINE_FIELD(CFlockingFlyer, m_flGoalSpeed, FIELD_FLOAT),
	DEFINE_FIELD(CFlockingFlyer, m_flLastBlockedTime, FIELD_TIME),
	DEFINE_FIELD(CFlockingFlyer, m_flFakeBlockedTime, FIELD_TIME),
	DEFINE_FIELD(CFlockingFlyer, m_flAlertTime, FIELD_TIME),
	// DEFINE_FIELD( CFlockingFlyer, m_flFlockNextSoundTime, FIELD_TIME ),	// don't need to save
};

IMPLEMENT_SAVERESTORE(CFlockingFlyer, CBaseMonster)

//=========================================================
//=========================================================
void CFlockingFlyerFlock::KeyValue(KeyValueData* pkvd)
{
	if ( FStrEq(pkvd->szKeyName, "iFlockSize") )
	{
		m_cFlockSize = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if ( FStrEq(pkvd->szKeyName, "flFlockRadius") )
	{
		m_flFlockRadius = static_cast<float>(atof(pkvd->szValue));
		pkvd->fHandled = TRUE;
	}
}

//=========================================================
//=========================================================
void CFlockingFlyerFlock::Spawn()
{
	Precache();
	SpawnFlock();

	REMOVE_ENTITY(ENT(pev));  // dump the spawn ent
}

//=========================================================
//=========================================================
void CFlockingFlyerFlock::Precache()
{
	// PRECACHE_MODEL( "models/aflock.mdl" );
	PRECACHE_MODEL("models/boid.mdl");

	PrecacheFlockSounds();
}

void CFlockingFlyerFlock::PrecacheFlockSounds(void)
{
	PRECACHE_SOUND("boid/boid_alert1.wav");
	PRECACHE_SOUND("boid/boid_alert2.wav");

	PRECACHE_SOUND("boid/boid_idle1.wav");
	PRECACHE_SOUND("boid/boid_idle2.wav");
}

//=========================================================
//=========================================================
void CFlockingFlyerFlock::SpawnFlock(void)
{
	float R = m_flFlockRadius;
	int iCount;
	Vector vecSpot;
	CFlockingFlyer *pBoid, *pLeader;

	pLeader = pBoid = NULL;

	for ( iCount = 0; iCount < m_cFlockSize; iCount++ )
	{
		pBoid = GetClassPtr<CFlockingFlyer>();

		if ( !pLeader )
		{
			// make this guy the leader.
			pLeader = pBoid;

			pLeader->m_pSquadLeader = pLeader;
			pLeader->m_pSquadNext = NULL;
		}

		vecSpot.x = RANDOM_FLOAT(-R, R);
		vecSpot.y = RANDOM_FLOAT(-R, R);
		vecSpot.z = RANDOM_FLOAT(0, 16);
		vecSpot = Vector(pev->origin) + vecSpot;

		UTIL_SetOrigin(pBoid->pev, vecSpot);
		pBoid->pev->movetype = MOVETYPE_FLY;
		pBoid->SpawnCommonCode();
		pBoid->pev->flags &= ~FL_ONGROUND;
		VectorClear(pBoid->pev->velocity);
		VectorCopy(pev->angles, pBoid->pev->angles);

		pBoid->pev->frame = 0;
		pBoid->pev->nextthink = gpGlobals->time + 0.2f;
		pBoid->SetThink(&CFlockingFlyer::IdleThink);

		if ( pBoid != pLeader )
		{
			pLeader->SquadAdd(pBoid);
		}
	}
}

//=========================================================
//=========================================================
void CFlockingFlyer::Spawn()
{
	Precache();
	SpawnCommonCode();

	pev->frame = 0;
	pev->nextthink = gpGlobals->time + 0.1f;
	SetThink(&CFlockingFlyer::IdleThink);
}

//=========================================================
//=========================================================
void CFlockingFlyer::Precache()
{
	// PRECACHE_MODEL( "models/aflock.mdl" );
	PRECACHE_MODEL("models/boid.mdl");
	CFlockingFlyerFlock::PrecacheFlockSounds();
}

//=========================================================
//=========================================================
void CFlockingFlyer::MakeSound(void)
{
	if ( m_flAlertTime > gpGlobals->time )
	{
		// make agitated sounds
		switch ( RANDOM_LONG(0, 1) )
		{
			case 0:
				EMIT_SOUND(ENT(pev), CHAN_WEAPON, "boid/boid_alert1.wav", 1, ATTN_NORM);
				break;
			case 1:
				EMIT_SOUND(ENT(pev), CHAN_WEAPON, "boid/boid_alert2.wav", 1, ATTN_NORM);
				break;
		}

		return;
	}

	// make normal sound
	switch ( RANDOM_LONG(0, 1) )
	{
		case 0:
			EMIT_SOUND(ENT(pev), CHAN_WEAPON, "boid/boid_idle1.wav", 1, ATTN_NORM);
			break;
		case 1:
			EMIT_SOUND(ENT(pev), CHAN_WEAPON, "boid/boid_idle2.wav", 1, ATTN_NORM);
			break;
	}
}

//=========================================================
//=========================================================
void CFlockingFlyer::Killed(entvars_t*, int)
{
	CFlockingFlyer* pSquad;

	pSquad = (CFlockingFlyer*)m_pSquadLeader;

	while ( pSquad )
	{
		pSquad->m_flAlertTime = gpGlobals->time + 15;
		pSquad = (CFlockingFlyer*)pSquad->m_pSquadNext;
	}

	if ( m_pSquadLeader )
	{
		m_pSquadLeader->SquadRemove(this);
	}

	pev->deadflag = DEAD_DEAD;

	pev->framerate = 0;
	pev->effects = EF_NOINTERP;

	UTIL_SetSize(pev, Vector(0, 0, 0), Vector(0, 0, 0));
	pev->movetype = MOVETYPE_TOSS;

	SetThink(&CFlockingFlyer::FallHack);
	pev->nextthink = gpGlobals->time + 0.1f;
}

void CFlockingFlyer::FallHack(void)
{
	if ( pev->flags & FL_ONGROUND )
	{
		if ( !FClassnameIs(pev->groundentity, "worldspawn") )
		{
			pev->flags &= ~FL_ONGROUND;
			pev->nextthink = gpGlobals->time + 0.1f;
		}
		else
		{
			VectorClear(pev->velocity);
			SetThink(NULL);
		}
	}
}

//=========================================================
//=========================================================
void CFlockingFlyer::SpawnCommonCode()
{
	pev->deadflag = DEAD_NO;
	pev->classname = MAKE_STRING("monster_flyer");
	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_FLY;
	pev->takedamage = DAMAGE_NO;
	pev->health = 1;

	m_fPathBlocked = FALSE;  // obstacles will be detected
	m_flFieldOfView = 0.2f;

	// SET_MODEL( ENT( pev ), "models/aflock.mdl" );
	SET_MODEL(ENT(pev), "models/boid.mdl");

	// UTIL_SetSize( pev, Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
	UTIL_SetSize(pev, Vector(-5, -5, 0), Vector(5, 5, 2));
}

//=========================================================
//=========================================================
void CFlockingFlyer::BoidAdvanceFrame()
{
	float flapspeed = (pev->speed - pev->armorvalue) / AFLOCK_ACCELERATE;
	pev->armorvalue = pev->armorvalue * 0.8f + pev->speed * 0.2f;

	if ( flapspeed < 0 )
		flapspeed = -flapspeed;
	if ( flapspeed < 0.25f )
		flapspeed = 0.25f;
	if ( flapspeed > 1.9f )
		flapspeed = 1.9f;

	pev->framerate = flapspeed;

	// lean
	pev->avelocity[PITCH] = -(pev->angles[PITCH] + flapspeed * 5);

	// bank
	pev->avelocity[ROLL] = -(pev->angles[ROLL] + pev->avelocity[YAW]);

	// pev->framerate = flapspeed;
	StudioFrameAdvance(0.1f);
}

//=========================================================
//=========================================================
void CFlockingFlyer::IdleThink(void)
{
	pev->nextthink = gpGlobals->time + 0.2f;

	// see if there's a client in the same pvs as the monster
	if ( !FNullEnt(FIND_CLIENT_IN_PVS(edict())) )
	{
		SetThink(&CFlockingFlyer::Start);
		pev->nextthink = gpGlobals->time + 0.1f;
	}
}

//=========================================================
// Start - player enters the pvs, so get things going.
//=========================================================
void CFlockingFlyer::Start(void)
{
	pev->nextthink = gpGlobals->time + 0.1f;

	if ( IsLeader() )
	{
		SetThink(&CFlockingFlyer::FlockLeaderThink);
	}
	else
	{
		SetThink(&CFlockingFlyer::FlockFollowerThink);
	}
	/*
		Vector vecTakeOff;
		vecTakeOff = Vector( 0, 0, 0 );

		vecTakeOff.z = 50 + RANDOM_FLOAT( 0, 100 );
		vecTakeOff.x = 20 - RANDOM_FLOAT( 0, 40 );
		vecTakeOff.y = 20 - RANDOM_FLOAT( 0, 40 );

		pev->velocity = vecTakeOff;

		pev->speed = pev->velocity.Length();
		pev->sequence = 0;
	*/
	SetActivity(ACT_FLY);
	ResetSequenceInfo();
	BoidAdvanceFrame();

	pev->speed = AFLOCK_FLY_SPEED;  // no delay!
}

//=========================================================
// Leader boid calls this to form a flock from surrounding boids
//=========================================================
void CFlockingFlyer::FormFlock(void)
{
	if ( !InSquad() )
	{
		// I am my own leader
		m_pSquadLeader = this;
		m_pSquadNext = NULL;
		int squadCount = 1;

		CBaseEntity* pEntity = NULL;

		while ( (pEntity = UTIL_FindEntityInSphere(pEntity, pev->origin, AFLOCK_MAX_RECRUIT_RADIUS)) != NULL )
		{
			CBaseMonster* pRecruit = pEntity->MyMonsterPointer();

			if ( pRecruit && pRecruit != this && pRecruit->IsAlive() && !pRecruit->m_pCine )
			{
				// Can we recruit this guy?
				if ( FClassnameIs(pRecruit->pev, "monster_flyer") )
				{
					squadCount++;
					SquadAdd((CFlockingFlyer*)pRecruit);
				}
			}
		}
	}

	SetThink(&CFlockingFlyer::IdleThink);  // now that flock is formed, go to idle and wait for a player to come along.
	pev->nextthink = gpGlobals->time;
}

//=========================================================
// Searches for boids that are too close and pushes them away
//=========================================================
void CFlockingFlyer::SpreadFlock()
{
	Vector vecDir;
	float flSpeed;  // holds vector magnitude while we fiddle with the direction

	CFlockingFlyer* pList = m_pSquadLeader;
	while ( pList )
	{
		if ( pList != this && (Vector(pev->origin) - Vector(pList->pev->origin)).Length() <= AFLOCK_TOO_CLOSE )
		{
			// push the other away
			vecDir = Vector(pList->pev->origin) - Vector(pev->origin);
			vecDir = vecDir.Normalize();

			// store the magnitude of the other boid's velocity, and normalize it so we
			// can average in a course that points away from the leader.
			flSpeed = VectorLength(pList->pev->velocity);
			VectorNormalize(pList->pev->velocity);
			((Vector(pList->pev->velocity) + vecDir) * 0.5f).CopyToArray(pList->pev->velocity);
			VectorScale(pList->pev->velocity, flSpeed, pList->pev->velocity);
		}

		pList = pList->m_pSquadNext;
	}
}

//=========================================================
// Alters the caller's course if he's too close to others
//
// This function should **ONLY** be called when Caller's velocity is normalized!!
//=========================================================
void CFlockingFlyer::SpreadFlock2()
{
	Vector vecDir;

	CFlockingFlyer* pList = m_pSquadLeader;
	while ( pList )
	{
		if ( pList != this && (Vector(pev->origin) - Vector(pList->pev->origin)).Length() <= AFLOCK_TOO_CLOSE )
		{
			vecDir = Vector(pev->origin) - Vector(pList->pev->origin);
			vecDir = vecDir.Normalize();

			VectorAdd(pev->velocity, vecDir, pev->velocity);
		}

		pList = pList->m_pSquadNext;
	}
}

//=========================================================
// FBoidPathBlocked - returns TRUE if there is an obstacle ahead
//=========================================================
BOOL CFlockingFlyer::FPathBlocked()
{
	TraceResult tr;
	Vector vecDist;  // used for general measurements
	Vector vecDir;  // used for general measurements
	BOOL fBlocked;

	if ( m_flFakeBlockedTime > gpGlobals->time )
	{
		m_flLastBlockedTime = gpGlobals->time;
		return TRUE;
	}

	// use VELOCITY, not angles, not all boids point the direction they are flying
	// vecDir = UTIL_VecToAngles( pevBoid->velocity );
	UTIL_MakeVectors(pev->angles);

	fBlocked = FALSE;  // assume the way ahead is clear

	// check for obstacle ahead
	UTIL_TraceLine(
		pev->origin,
		Vector(pev->origin) + Vector(gpGlobals->v_forward) * AFLOCK_CHECK_DIST,
		ignore_monsters,
		ENT(pev),
		&tr);
	if ( tr.flFraction != 1.0 )
	{
		m_flLastBlockedTime = gpGlobals->time;
		fBlocked = TRUE;
	}

	// extra wide checks
	UTIL_TraceLine(
		Vector(pev->origin) + Vector(gpGlobals->v_right) * 12,
		Vector(pev->origin) + Vector(gpGlobals->v_right) * 12 + Vector(gpGlobals->v_forward) * AFLOCK_CHECK_DIST,
		ignore_monsters,
		ENT(pev),
		&tr);
	if ( tr.flFraction != 1.0 )
	{
		m_flLastBlockedTime = gpGlobals->time;
		fBlocked = TRUE;
	}

	UTIL_TraceLine(
		Vector(pev->origin) - Vector(gpGlobals->v_right) * 12,
		Vector(pev->origin) - Vector(gpGlobals->v_right) * 12 + Vector(gpGlobals->v_forward) * AFLOCK_CHECK_DIST,
		ignore_monsters,
		ENT(pev),
		&tr);
	if ( tr.flFraction != 1.0 )
	{
		m_flLastBlockedTime = gpGlobals->time;
		fBlocked = TRUE;
	}

	if ( !fBlocked && gpGlobals->time - m_flLastBlockedTime > 6 )
	{
		// not blocked, and it's been a few seconds since we've actually been blocked.
		m_flFakeBlockedTime = gpGlobals->time + RANDOM_LONG(1, 3);
	}

	return fBlocked;
}

//=========================================================
// Leader boids use this think every tenth
//=========================================================
void CFlockingFlyer::FlockLeaderThink(void)
{
	TraceResult tr;
	Vector vecDist;  // used for general measurements
	Vector vecDir;  // used for general measurements
	float flLeftSide;
	float flRightSide;

	pev->nextthink = gpGlobals->time + 0.1f;

	UTIL_MakeVectors(pev->angles);

	// is the way ahead clear?
	if ( !FPathBlocked() )
	{
		// if the boid is turning, stop the trend.
		if ( m_fTurning )
		{
			m_fTurning = FALSE;
			pev->avelocity[YAW] = 0;
		}

		m_fPathBlocked = FALSE;

		if ( pev->speed <= AFLOCK_FLY_SPEED )
		{
			pev->speed += 5;
		}

		VectorScale(gpGlobals->v_forward, pev->speed, pev->velocity);

		BoidAdvanceFrame();

		return;
	}

	// IF we get this far in the function, the leader's path is blocked!
	m_fPathBlocked = TRUE;

	if ( !m_fTurning )  // something in the way and boid is not already turning to avoid
	{
		// measure clearance on left and right to pick the best dir to turn
		UTIL_TraceLine(
			pev->origin,
			Vector(pev->origin) + Vector(gpGlobals->v_right) * AFLOCK_CHECK_DIST,
			ignore_monsters,
			ENT(pev),
			&tr);
		VectorSubtract(tr.vecEndPos, pev->origin, vecDist);
		flRightSide = vecDist.Length();

		UTIL_TraceLine(
			pev->origin,
			Vector(pev->origin) - Vector(gpGlobals->v_right) * AFLOCK_CHECK_DIST,
			ignore_monsters,
			ENT(pev),
			&tr);
		VectorSubtract(tr.vecEndPos, pev->origin, vecDist);
		flLeftSide = vecDist.Length();

		// turn right if more clearance on right side
		if ( flRightSide > flLeftSide )
		{
			pev->avelocity[YAW] = -AFLOCK_TURN_RATE;
			m_fTurning = TRUE;
		}
		// default to left turn :)
		else if ( flLeftSide > flRightSide )
		{
			pev->avelocity[YAW] = AFLOCK_TURN_RATE;
			m_fTurning = TRUE;
		}
		else
		{
			// equidistant. Pick randomly between left and right.
			m_fTurning = TRUE;

			if ( RANDOM_LONG(0, 1) == 0 )
			{
				pev->avelocity[YAW] = AFLOCK_TURN_RATE;
			}
			else
			{
				pev->avelocity[YAW] = -AFLOCK_TURN_RATE;
			}
		}
	}
	SpreadFlock();

	VectorScale(gpGlobals->v_forward, pev->speed, pev->velocity);

	// check and make sure we aren't about to plow into the ground, don't let it happen
	UTIL_TraceLine(pev->origin, Vector(pev->origin) - Vector(gpGlobals->v_up) * 16, ignore_monsters, ENT(pev), &tr);

	if ( tr.flFraction != 1.0 && pev->velocity[VEC3_Z] < 0 )
	{
		pev->velocity[VEC3_Z] = 0;
	}

	// maybe it did, though.
	if ( FBitSet(pev->flags, FL_ONGROUND) )
	{
		UTIL_SetOrigin(pev, Vector(pev->origin) + Vector(0, 0, 1));
		pev->velocity[VEC3_Z] = 0;
	}

	if ( m_flFlockNextSoundTime < gpGlobals->time )
	{
		MakeSound();
		m_flFlockNextSoundTime = gpGlobals->time + RANDOM_FLOAT(1, 3);
	}

	BoidAdvanceFrame();

	return;
}

//=========================================================
// follower boids execute this code when flocking
//=========================================================
void CFlockingFlyer::FlockFollowerThink(void)
{
	Vector vecDist;
	Vector vecDir;
	Vector vecDirToLeader;
	float flDistToLeader;

	pev->nextthink = gpGlobals->time + 0.1f;

	if ( IsLeader() || !InSquad() )
	{
		// the leader has been killed and this flyer suddenly finds himself the leader.
		SetThink(&CFlockingFlyer::FlockLeaderThink);
		return;
	}

	vecDirToLeader = (Vector(m_pSquadLeader->pev->origin) - Vector(pev->origin));
	flDistToLeader = vecDirToLeader.Length();

	// match heading with leader
	VectorCopy(m_pSquadLeader->pev->angles, pev->angles);

	//
	// We can see the leader, so try to catch up to it
	//
	if ( FInViewCone(m_pSquadLeader) )
	{
		// if we're too far away, speed up
		if ( flDistToLeader > AFLOCK_TOO_FAR )
		{
			m_flGoalSpeed = VectorLength(m_pSquadLeader->pev->velocity) * 1.5f;
		}

		// if we're too close, slow down
		else if ( flDistToLeader < AFLOCK_TOO_CLOSE )
		{
			m_flGoalSpeed = VectorLength(m_pSquadLeader->pev->velocity) * 0.5f;
		}
	}
	else
	{
		// wait up! the leader isn't out in front, so we slow down to let him pass
		m_flGoalSpeed = VectorLength(m_pSquadLeader->pev->velocity) * 0.5f;
	}

	SpreadFlock2();

	pev->speed = VectorLength(pev->velocity);
	VectorNormalize(pev->velocity);

	// if we are too far from leader, average a vector towards it into our current velocity
	if ( flDistToLeader > AFLOCK_TOO_FAR )
	{
		vecDirToLeader = vecDirToLeader.Normalize();
		((Vector(pev->velocity) + vecDirToLeader) * 0.5f).CopyToArray(pev->velocity);
	}

	// clamp speeds and handle acceleration
	if ( m_flGoalSpeed > AFLOCK_FLY_SPEED * 2 )
	{
		m_flGoalSpeed = AFLOCK_FLY_SPEED * 2;
	}

	if ( pev->speed < m_flGoalSpeed )
	{
		pev->speed += AFLOCK_ACCELERATE;
	}
	else if ( pev->speed > m_flGoalSpeed )
	{
		pev->speed -= AFLOCK_ACCELERATE;
	}

	VectorScale(pev->velocity, pev->speed, pev->velocity);

	BoidAdvanceFrame();
}

/*
	TraceResult tr;

	// Is this boid's course blocked?
	if( FBoidPathBlocked( pev ) )
	{
		// course is still blocked from last time. Just keep flying along adjusted
		// velocity
		if( m_fCourseAdjust )
		{
			pev->velocity = m_vecAdjustedVelocity * pev->speed;
			return;
		}
		else // set course adjust flag and calculate adjusted velocity
		{
			m_fCourseAdjust = TRUE;

			// use VELOCITY, not angles, not all boids point the direction they are flying
			//vecDir = UTIL_VecToAngles( pev->velocity );
			//UTIL_MakeVectors( vecDir );

			UTIL_MakeVectors( pev->angles );

			// measure clearance on left and right to pick the best dir to turn
			UTIL_TraceLine( pev->origin, pev->origin + gpGlobals->v_right * AFLOCK_CHECK_DIST, ignore_monsters, ENT( pev
   ), &tr ); vecDist = tr.vecEndPos - pev->origin; flRightSide = vecDist.Length();

			UTIL_TraceLine( pev->origin, pev->origin - gpGlobals->v_right * AFLOCK_CHECK_DIST, ignore_monsters, ENT( pev
   ), &tr ); vecDist = tr.vecEndPos - pev->origin; flLeftSide = vecDist.Length();

			// slide right if more clearance on right side
			if( flRightSide > flLeftSide )
			{
				m_vecAdjustedVelocity = gpGlobals->v_right;
			}
			// else slide left
			else
			{
				m_vecAdjustedVelocity = gpGlobals->v_right * -1;
			}
		}
		return;
	}

	// if we make it this far, boids path is CLEAR!
	m_fCourseAdjust = FALSE;
*/

//=========================================================
//
// SquadUnlink(), Unlink the squad pointers.
//
//=========================================================
void CFlockingFlyer::SquadUnlink(void)
{
	m_pSquadLeader = NULL;
	m_pSquadNext = NULL;
}

//=========================================================
//
// SquadAdd(), add pAdd to my squad
//
//=========================================================
void CFlockingFlyer::SquadAdd(CFlockingFlyer* pAdd)
{
	ASSERT(pAdd != NULL);
	ASSERT(!pAdd->InSquad());
	ASSERT(this->IsLeader());

	pAdd->m_pSquadNext = m_pSquadNext;
	m_pSquadNext = pAdd;
	pAdd->m_pSquadLeader = this;
}

//=========================================================
//
// SquadRemove(), remove pRemove from my squad.
// If I am pRemove, promote m_pSquadNext to leader
//
//=========================================================
void CFlockingFlyer::SquadRemove(CFlockingFlyer* pRemove)
{
	ASSERT(pRemove != NULL);
	ASSERT(this->IsLeader());
	ASSERT(pRemove->m_pSquadLeader == this);

	if ( SquadCount() > 2 )
	{
		// Removing the leader, promote m_pSquadNext to leader
		if ( pRemove == this )
		{
			CFlockingFlyer* pLeader = m_pSquadNext;

			if ( pLeader )
			{
				// copy the enemy LKP to the new leader
				pLeader->m_vecEnemyLKP = m_vecEnemyLKP;

				CFlockingFlyer* pList = pLeader;

				while ( pList )
				{
					pList->m_pSquadLeader = pLeader;
					pList = pList->m_pSquadNext;
				}
			}
			SquadUnlink();
		}
		else  // removing a node
		{
			CFlockingFlyer* pList = this;

			// Find the node before pRemove
			while ( pList->m_pSquadNext != pRemove )
			{
				// assert to test valid list construction
				ASSERT(pList->m_pSquadNext != NULL);
				pList = pList->m_pSquadNext;
			}
			// List validity
			ASSERT(pList->m_pSquadNext == pRemove);

			// Relink without pRemove
			pList->m_pSquadNext = pRemove->m_pSquadNext;

			// Unlink pRemove
			pRemove->SquadUnlink();
		}
	}
	else
		SquadDisband();
}

//=========================================================
//
// SquadCount(), return the number of members of this squad
// callable from leaders & followers
//
//=========================================================
int CFlockingFlyer::SquadCount(void)
{
	CFlockingFlyer* pList = m_pSquadLeader;
	int squadCount = 0;
	while ( pList )
	{
		squadCount++;
		pList = pList->m_pSquadNext;
	}

	return squadCount;
}

//=========================================================
//
// SquadDisband(), Unlink all squad members
//
//=========================================================
void CFlockingFlyer::SquadDisband(void)
{
	CFlockingFlyer* pList = m_pSquadLeader;
	CFlockingFlyer* pNext;

	while ( pList )
	{
		pNext = pList->m_pSquadNext;
		pList->SquadUnlink();
		pList = pNext;
	}
}
