#pragma once

#include "standard_includes.h"
#include "weapons/genericweapon.h"
#include "weapons.h"
#include "weaponmechanics/projectilemechanic.h"

class CWeaponGrenadeLauncher : public CGenericWeapon
{
public:
	CWeaponGrenadeLauncher();

	const WeaponAtts::WACollection& WeaponAttributes() const override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif

private:
#ifndef CLIENT_DLL
	void CreateProjectile(const WeaponMechanics::CProjectileMechanic& mechanic);
#endif

	WeaponMechanics::CProjectileMechanic* m_ContactGrenadeAttack = nullptr;
	WeaponMechanics::CProjectileMechanic* m_TimedGrenadeAttack = nullptr;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponGrenadeLauncher>();
}
