#pragma once

#include "weaponmechanics/basemechanic.h"

namespace WeaponAtts
{
	struct WAProjectileAttack;
};

namespace WeaponMechanics
{
	class CProjectileMechanic : public CBaseMechanic
	{
	public:
		CProjectileMechanic(CGenericWeapon* weapon, const WeaponAtts::WAProjectileAttack* attackMode);
		CProjectileMechanic(CGenericWeapon* weapon, uint32_t attackIndex);

		void Precache() override;
		InvocationResult Invoke() override;

	private:
		const WeaponAtts::WAProjectileAttack* ProjectileAttackMode() const;
	};
}  // namespace WeaponMechanics
