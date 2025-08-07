#pragma once

#include "weaponatts_ammobasedattack.h"
#include "weaponatts_skillrecord.h"

namespace WeaponAtts
{
	struct WAHitscanAttack : public WAAmmoBasedAttack
	{
		typedef WASkillRecord::SkillDataEntryPtr SkillDataEntryPtr;

		uint8_t BulletsPerShot = 1;
		SkillDataEntryPtr BaseDamagePerShot = nullptr;
		float AutoAim = 0.0f;

		virtual WABaseAttack::Classification Classify() const override
		{
			return WABaseAttack::Classification::Hitscan;
		}

		virtual void Validate() const override
		{
			WAAmmoBasedAttack::Validate();

			ASSERTSZ_Q(BaseDamagePerShot, "Skill entry for base damage per shot must be specified.");
		}
	};
}  // namespace WeaponAtts
