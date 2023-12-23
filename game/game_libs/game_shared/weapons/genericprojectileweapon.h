#pragma once

#include "genericweapon.h"
#include "weaponatts_projectileattack.h"

class CGenericProjectileWeapon : public CGenericWeapon
{
public:
	// Required so that derived classes can call through.
	void Precache() override;
	void WeaponTick() override;
	void Holster(int skiplocal = 0) override;
	bool ReadPredictionData(const weapon_data_t* from) override;
	bool WritePredictionData(weapon_data_t* to) override;

protected:
	virtual bool InvokeWithAttackMode(WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode) override;

#ifndef CLIENT_DLL
	virtual void CreateProjectile(const WeaponAtts::WAProjectileAttack& projectileAttack) = 0;
#endif

private:
	bool AmmoDecrementWillEmptyWeaponClip(const WeaponAtts::WABaseAttack* attackMode, int decrement) const;
	void CreateProjectileAndDecrementAmmo(const WeaponAtts::WAProjectileAttack& projectileAttack);

	const WeaponAtts::WAProjectileAttack* m_EnqueuedProjectileAttack = nullptr;
};
