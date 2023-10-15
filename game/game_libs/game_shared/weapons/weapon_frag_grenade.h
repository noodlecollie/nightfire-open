#pragma once

#include "standard_includes.h"
#include "weapons/basegrenadelauncher.h"
#include "weapons.h"

class CWeaponFragGrenade : public CBaseGrenadeLauncher
{
public:
	CWeaponFragGrenade();

	const WeaponAtts::WACollection& WeaponAttributes() const override;
	void Precache() override;
	void WeaponTick() override;
	void PrimaryAttack() override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;

protected:
	void CreateProjectile(const WeaponAtts::WAProjectileAttack& projectileAttack) override;
#endif

private:
	bool m_bGrenadePrimed = false;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFragGrenade>();
}
