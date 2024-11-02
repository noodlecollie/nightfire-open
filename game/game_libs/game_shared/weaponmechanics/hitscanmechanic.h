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
		explicit CHitscanMechanic(CGenericWeapon* weapon, const WeaponAtts::WAHitscanAttack* attackMode);

		void Precache() override;
		InvocationResult Invoke() override;

	private:
		const WeaponAtts::WAHitscanAttack* HitscanAttackMode() const
		{
			return GetAttackMode<WeaponAtts::WAHitscanAttack>();
		}
	};
}
