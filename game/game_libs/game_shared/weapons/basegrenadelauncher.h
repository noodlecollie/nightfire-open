#pragma once

#include "standard_includes.h"
#include "genericprojectileweapon.h"
#include "weapons.h"

class CBaseGrenadeLauncher_Grenade;

class CBaseGrenadeLauncher : public CGenericProjectileWeapon
{
#ifndef CLIENT_DLL
protected:
	static CBaseGrenadeLauncher_Grenade*
	CreateGrenade(entvars_t* pevOwner, const Vector& location, const Vector& launchDir);

	CBaseGrenadeLauncher_Grenade* CreateGrenade(float extraPitch = 0.0f, float distFromView = 16.0f);
	Vector GetGrenadeLaunchAngles(float extraPitch = 0.0f) const;
#endif
};

class CBaseGrenadeLauncher_Grenade : public CGrenade
{
public:
	void SetModelName(const char* modelName);
	void SetSize(const Vector& min, const Vector& max);
	void SetSize(const Vector& halfSize);
	void SetFriction(float friction);
	void SetGravity(float gravity);

#ifndef CLIENT_DLL
	void SetOwner(entvars_t* owner);
	void SetExplodeOnContact(bool explodeOnContact);
	void SetTumbleAngVel(float vel);
	void SetRandomTumbleAngVel(float min, float max);
	void SetDamageOnExplode(float damage);
	void SetSpeed(float speed);
	void SetFuseTime(float fuseTime);
#endif
};
