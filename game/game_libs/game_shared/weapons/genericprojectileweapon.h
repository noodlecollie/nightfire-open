#pragma once

#include "genericweapon.h"
#include "weaponatts_projectileattack.h"

class CGenericProjectileWeapon : public CGenericWeapon
{
public:
	// Required so that derived classes can call through.
	virtual void Precache() override;

protected:
	virtual bool InvokeWithAttackMode(WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode) override;

#ifndef CLIENT_DLL
	virtual void CreateProjectile(const WeaponAtts::WAProjectileAttack& projectileAttack) = 0;
#endif
};
