#pragma once

#include "weaponatts_ammobasedattack.h"

namespace WeaponAtts
{
	struct WAProjectileAttack : public WAAmmoBasedAttack
	{
		float projectileDelay = 0.0f;

		virtual WABaseAttack::Classification Classify() const override
		{
			return WABaseAttack::Classification::Projectile;
		}
	};
}  // namespace WeaponAtts
