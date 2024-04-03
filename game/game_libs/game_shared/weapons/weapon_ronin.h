#pragma once

#include "standard_includes.h"
#include "basegrenadelauncher.h"

class CWeaponRonin : public CBaseGrenadeLauncher
{
public:
	CWeaponRonin();
	const WeaponAtts::WACollection& WeaponAttributes() const override;

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
	const WACollection& StaticWeaponAttributes<CWeaponRonin>();
}
