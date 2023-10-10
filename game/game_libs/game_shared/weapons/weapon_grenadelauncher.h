#pragma once

#include "standard_includes.h"
#include "weapons/basegrenadelauncher.h"
#include "weapons.h"

class CWeaponGrenadeLauncher : public CBaseGrenadeLauncher
{
public:
	CWeaponGrenadeLauncher();

	const WeaponAtts::WACollection& WeaponAttributes() const override;
	void Precache() override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;

protected:
	void CreateProjectile(const WeaponAtts::WAProjectileAttack& projectileAttack) override;
#endif
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponGrenadeLauncher>();
}
