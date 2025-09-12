#pragma once

#include "standard_includes.h"
#include "genericweapon.h"
#include "weaponmechanics/hitscanmechanic.h"

class CWeaponSilent : public CGenericWeapon
{
public:
	CWeaponSilent();
	const WeaponAtts::WACollection& WeaponAttributes() const override;

private:
	WeaponMechanics::CHitscanMechanic* m_PrimaryMechanic = nullptr;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponSilent>();
}
