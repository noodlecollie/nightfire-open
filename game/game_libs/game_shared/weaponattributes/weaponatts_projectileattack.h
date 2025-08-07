#pragma once

#include "weaponatts_ammobasedattack.h"

namespace WeaponAtts
{
	struct WAProjectileAttack : public WAAmmoBasedAttack
	{
		float ProjectileDelay = 0.0f;
		const char* ProjectileModelName = nullptr;

		virtual WABaseAttack::Classification Classify() const override
		{
			return WABaseAttack::Classification::Projectile;
		}
	};
}  // namespace WeaponAtts
