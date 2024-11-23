#pragma once

#include "weaponmechanics/basemechanic.h"

namespace WeaponAtts
{
	struct WAHitscanAttack;
};

namespace WeaponMechanics
{
	class CHitscanMechanic : public CBaseMechanic
	{
	public:
		CHitscanMechanic(CGenericWeapon* weapon, const WeaponAtts::WAHitscanAttack* attackMode);
		CHitscanMechanic(CGenericWeapon* weapon, uint32_t attackIndex);

		void Precache() override;
		InvocationResult Invoke(uint32_t step) override;
		void WeaponIdle() override;

	private:
		const WeaponAtts::WAHitscanAttack* HitscanAttackMode() const;
	};
}
