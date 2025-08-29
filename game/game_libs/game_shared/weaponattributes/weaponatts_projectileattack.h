#pragma once

#include "weaponatts_ammobasedattack.h"
#include "weaponatts_skillrecord.h"

namespace WeaponAtts
{
	struct WAProjectileAttack : public WAAmmoBasedAttack
	{
		using SkillDataEntryPtr = WASkillRecord::SkillDataEntryPtr;

		float ProjectileDelay = 0.0f;
		const char* ProjectileModelName = nullptr;
		float LaunchSpeed = 1000.0f;
		SkillDataEntryPtr BaseExplosionDamage = nullptr;

		virtual WABaseAttack::Classification Classify() const override
		{
			return WABaseAttack::Classification::Projectile;
		}

		void Validate() const override
		{
			WAAmmoBasedAttack::Validate();

			ASSERTSZ_Q(BaseExplosionDamage, "Skill entry for base explosion damage must be specified.");
		}
	};
}  // namespace WeaponAtts
