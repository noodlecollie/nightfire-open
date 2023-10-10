#include "weapons/basegrenadelauncher.h"
#include "MathLib/utils.h"

#ifndef CLIENT_DLL
CBaseGrenadeLauncher_Grenade*
CBaseGrenadeLauncher::CreateGrenade(entvars_t* pevOwner, const Vector& location, const Vector& launchDir)
{
	CBaseGrenadeLauncher_Grenade* pGrenade = GetClassPtr<CBaseGrenadeLauncher_Grenade>(NULL);
	pGrenade->Spawn();

	UTIL_SetOrigin(pGrenade->pev, location);
	launchDir.CopyToArray(pGrenade->pev->velocity);
	UTIL_VecToAngles(pGrenade->pev->velocity).CopyToArray(pGrenade->pev->angles);
	pGrenade->SetOwner(pevOwner);
	pGrenade->SetGravity(1.4f);
	pGrenade->SetFuseTime(-1.0f);
	pGrenade->SetExplodeSpriteScale(50.0f);

	return pGrenade;
}

CBaseGrenadeLauncher_Grenade* CBaseGrenadeLauncher::CreateGrenade(float extraPitch, float distFromView)
{
	if ( !m_pPlayer )
	{
		return nullptr;
	}

	Vector forward;
	AngleVectors(GetGrenadeLaunchAngles(extraPitch), forward, nullptr, nullptr);

	const Vector location =
		Vector(m_pPlayer->pev->origin) + Vector(m_pPlayer->pev->view_ofs) + (forward * distFromView);

	return CreateGrenade(m_pPlayer->pev, location, forward);
}

Vector CBaseGrenadeLauncher::GetGrenadeLaunchAngles(float extraPitch) const
{
	if ( !m_pPlayer )
	{
		return Vector();
	}

	Vector viewAngles = Vector(m_pPlayer->pev->v_angle) + Vector(m_pPlayer->pev->punchangle);

	if ( extraPitch != 0.0f )
	{
		// Add some more pitch depending on the cosine of the original pitch.
		// If the player is looking horizontally, we want the grenade to be
		// launched a little more upward; if they are looking straight up or
		// down, we don't want any modification at all.
		viewAngles[0] -= cosf(UTIL_DegreesToRadians(viewAngles[0])) * extraPitch;
	}

	if ( viewAngles[0] < -89.0f )
	{
		viewAngles[0] = -89.0f;
	}
	else if ( viewAngles[0] > 89.0f )
	{
		viewAngles[0] = 89.0f;
	}

	return viewAngles;
}
#endif

void CBaseGrenadeLauncher_Grenade::SetModelName(const char* modelName)
{
	SET_MODEL(ENT(pev), modelName);
}

void CBaseGrenadeLauncher_Grenade::SetSize(const Vector& min, const Vector& max)
{
	UTIL_SetSize(pev, min, max);
}

void CBaseGrenadeLauncher_Grenade::SetSize(const Vector& halfSize)
{
	SetSize(-halfSize, halfSize);
}

void CBaseGrenadeLauncher_Grenade::SetFriction(float friction)
{
	pev->friction = friction;
}

void CBaseGrenadeLauncher_Grenade::SetGravity(float gravity)
{
	pev->gravity = gravity;
}

#ifndef CLIENT_DLL
void CBaseGrenadeLauncher_Grenade::SetOwner(entvars_t* owner)
{
	pev->owner = ENT(owner);
}

void CBaseGrenadeLauncher_Grenade::SetExplodeOnContact(bool explodeOnContact)
{
	SetTouch(explodeOnContact ? &CGrenade::ExplodeTouch : &CGrenade::BounceTouch);
}

void CBaseGrenadeLauncher_Grenade::SetTumbleAngVel(float vel)
{
	pev->avelocity[VEC3_X] = vel;
}

void CBaseGrenadeLauncher_Grenade::SetRandomTumbleAngVel(float min, float max)
{
	SetTumbleAngVel(RANDOM_FLOAT(min, max));
}

void CBaseGrenadeLauncher_Grenade::SetDamageOnExplode(float damage)
{
	pev->dmg = damage;
}

void CBaseGrenadeLauncher_Grenade::SetSpeed(float speed)
{
	Vector vel(pev->velocity);

	if ( vel.Length() == 0.0f )
	{
		return;
	}

	(vel.Normalize() * speed).CopyToArray(pev->velocity);
}

void CBaseGrenadeLauncher_Grenade::SetFuseTime(float fuseTime)
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
