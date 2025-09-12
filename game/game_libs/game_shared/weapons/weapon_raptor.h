#pragma once

#include "standard_includes.h"
#include "genericweapon.h"
#include "weaponmechanics/hitscanmechanic.h"

class CWeaponRaptor : public CGenericWeapon
{
public:
	CWeaponRaptor();

	const WeaponAtts::WACollection& WeaponAttributes() const override;

private:
	WeaponMechanics::CHitscanMechanic* m_PrimaryMechanic = nullptr;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponRaptor>();
}
