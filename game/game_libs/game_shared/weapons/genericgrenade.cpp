#include "weapons/genericgrenade.h"
#include "MathLib/utils.h"

void CGenericGrenade::SetModelName(const char* modelName)
{
	SET_MODEL(ENT(pev), modelName);
}

void CGenericGrenade::SetSize(const Vector& min, const Vector& max)
{
	UTIL_SetSize(pev, min, max);
}

void CGenericGrenade::SetSize(const Vector& halfSize)
{
	SetSize(-halfSize, halfSize);
}

void CGenericGrenade::SetFriction(float friction)
{
	pev->friction = friction;
}

void CGenericGrenade::SetGravity(float gravity)
{
	pev->gravity = gravity;
}

#ifndef CLIENT_DLL
CGenericGrenade* CGenericGrenade::CreateGrenade(CBasePlayer* player, const Vector& launchAngles, float distFromEyes)
{
	if ( !player )
	{
		return nullptr;
	}

	Vector forward;
	AngleVectors(launchAngles, forward, nullptr, nullptr);

	const Vector location = Vector(player->pev->origin) + Vector(player->pev->view_ofs) + (forward * distFromEyes);

	return CreateGrenade(player->pev, location, forward);
}

CGenericGrenade* CGenericGrenade::CreateGrenade(entvars_t* pevOwner, const Vector& location, const Vector& launchDir)
{
	CGenericGrenade* pGrenade = GetClassPtr<CGenericGrenade>(nullptr);
	pGrenade->Spawn();

	UTIL_SetOrigin(pGrenade->pev, location);
	launchDir.CopyToArray(pGrenade->pev->velocity);
	UTIL_VecToAngles(launchDir).CopyToArray(pGrenade->pev->angles);
	pGrenade->SetOwner(pevOwner);

	// Some reasonable defaults:
	pGrenade->SetGravity(1.4f);
	pGrenade->SetFuseTime(-1.0f);
	pGrenade->SetExplodeSpriteScale(50);

	return pGrenade;
}

void CGenericGrenade::SetOwner(entvars_t* owner)
{
	pev->owner = ENT(owner);
}

void CGenericGrenade::SetExplodeOnContact(bool explodeOnContact)
{
	SetTouch(explodeOnContact ? &CGrenade::ExplodeTouch : &CGrenade::BounceTouch);
}

void CGenericGrenade::SetTumbleAngVel(float vel)
{
	pev->avelocity[VEC3_X] = vel;
}

void CGenericGrenade::SetRandomTumbleAngVel(float min, float max)
{
	SetTumbleAngVel(RANDOM_FLOAT(min, max));
}

void CGenericGrenade::SetDamageOnExplode(float damage)
{
	pev->dmg = damage;
}

void CGenericGrenade::SetSpeed(float speed)
{
	Vector vel(pev->velocity);

	if ( vel.Length() == 0.0f )
	{
		return;
	}

	(vel.Normalize() * speed).CopyToArray(pev->velocity);
}

void CGenericGrenade::SetFuseTime(float fuseTime)
{
	if ( fuseTime < 0.0f )
	{
		SetThink(&CGrenade::DangerSoundThink);
		pev->nextthink = gpGlobals->time;
		return;
	}

	SetThink(&CGrenade::TumbleThink);

	pev->dmgtime = gpGlobals->time + fuseTime;
	pev->nextthink = gpGlobals->time + 0.1f;

	if ( fuseTime < 0.1 )
	{
		pev->nextthink = gpGlobals->time;
		VectorClear(pev->velocity);
	}
}
#endif
