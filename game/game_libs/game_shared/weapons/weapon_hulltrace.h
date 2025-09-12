#pragma once

#include "standard_includes.h"
#include "genericweapon.h"
#include "weaponmechanics/delegatedmechanic.h"

class CWeaponHullTrace : public CGenericWeapon
{
public:
	CWeaponHullTrace();
	const WeaponAtts::WACollection& WeaponAttributes() const override;

private:
	WeaponMechanics::InvocationResult DoTrace(WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step);

#ifndef CLIENT_DLL
	void DoTrace_Server();
#endif

	WeaponMechanics::CDelegatedMechanic* m_PrimaryMechanic = nullptr;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponHullTrace>();
}
