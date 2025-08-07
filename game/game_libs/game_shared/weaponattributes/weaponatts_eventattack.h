#pragma once

#include "weaponatts_baseattack.h"

namespace WeaponAtts
{
	struct WAEventAttack : public WABaseAttack
	{
		virtual WABaseAttack::Classification Classify() const override
		{
			return WABaseAttack::Classification::Event;
		}

		virtual void Validate() const override
		{
			WABaseAttack::Validate();
		}
	};
}  // namespace WeaponAtts
