#pragma once

#include "weaponatts_baseattack.h"
#include "weaponatts_skillrecord.h"
#include "weaponatts_soundset.h"

namespace WeaponAtts
{
	struct WAMeleeAttack : public WABaseAttack
	{
		typedef WASkillRecord::SkillDataEntryPtr SkillDataEntryPtr;

		float Reach = 32.0f;  // Defaults to crowbar's reach
		SkillDataEntryPtr BaseDamagePerHit = nullptr;
		bool DecalOnImpact = true;

		// List of individual attack strikes, as offsets in seconds
		// from the original attack invocation.
		CUtlVector<float> Strikes;

		// If no body hitsounds provided, world hitsounds are used for bodies too.
		WASoundSet WorldHitSounds;
		WASoundSet BodyHitSounds;

		virtual WABaseAttack::Classification Classify() const override
		{
			return WABaseAttack::Classification::Melee;
		}

		void Validate() const override
		{
			WABaseAttack::Validate();

			ASSERTSZ_Q(BaseDamagePerHit, "Skill entry for base damage per hit must be specified.");

			FOR_EACH_VEC(Strikes, index)
			{
				ASSERTSZ_Q(Strikes[index] >= 0.0f, "Strike delay must be positive.");
			}
		}
	};
}  // namespace WeaponAtts
