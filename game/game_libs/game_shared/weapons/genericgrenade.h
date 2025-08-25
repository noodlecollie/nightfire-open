#pragma once

#include "standard_includes.h"
#include "weapons.h"
#include "weaponattributes/weaponatts_skillrecord.h"

class CGenericGrenade : public CGrenade
{
public:
	void SetModelName(const char* modelName);
	void SetSize(const Vector& min, const Vector& max);
	void SetSize(const Vector& halfSize);
	void SetFriction(float friction);
	void SetGravity(float gravity);

#ifndef CLIENT_DLL
	static CGenericGrenade* CreateGrenade(CBasePlayer* player, const Vector& launchAngles, float distFromEyes = 16.0f);
	static CGenericGrenade* CreateGrenade(entvars_t* pevOwner, const Vector& location, const Vector& launchDir);

	void SetOwner(entvars_t* owner);
	void SetExplodeOnContact(bool explodeOnContact);
	void SetTumbleAngVel(float vel);
	void SetRandomTumbleAngVel(float min, float max);
	void SetDamageOnExplode(float damage);
	void SetDamageOnExplode(WeaponAtts::WASkillRecord::SkillDataEntryPtr ptr);
	void SetSpeed(float speed);
	void SetFuseTime(float fuseTime);
#endif
};
