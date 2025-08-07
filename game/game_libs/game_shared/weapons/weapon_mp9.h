#pragma once

#include "standard_includes.h"
#include "genericweapon.h"
#include "weaponmechanics/hitscanmechanic.h"

class CWeaponMP9 : public CGenericWeapon
{
public:
	CWeaponMP9();
	const WeaponAtts::WACollection& WeaponAttributes() const override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif

private:
	WeaponMechanics::CHitscanMechanic* m_PrimaryMechanic = nullptr;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponMP9>();
}
