#pragma once

#include "weaponmechanics/basemechanic.h"

namespace WeaponAtts
{
	struct WAMeleeAttack;
};

namespace WeaponMechanics
{
	class CMeleeMechanic : public CBaseMechanic
	{
	public:
		explicit CMeleeMechanic(CGenericWeapon* weapon, const WeaponAtts::WAMeleeAttack* attackMode);

		void Precache() override;

	private:
		const WeaponAtts::WAMeleeAttack* MeleeAttackMode() const
		{
			return GetAttackMode<WeaponAtts::WAMeleeAttack>();
		}
	};
}
