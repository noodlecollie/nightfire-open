#pragma once

#include "weaponatts_ammobasedattack.h"

namespace WeaponAtts
{
	struct WAProjectileAttack : public WAAmmoBasedAttack
	{
		virtual WABaseAttack::Classification Classify() const override
		{
			return WABaseAttack::Classification::Projectile;
		}
	};
}
