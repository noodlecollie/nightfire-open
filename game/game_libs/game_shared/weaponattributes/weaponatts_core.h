#pragma once

#include "weaponatts_base.h"
#include "weaponids.h"

namespace WeaponAtts
{
	struct WACore : public WABase
	{
		WeaponId_e Id = WeaponId_e::WeaponNone;
		const char* Classname = nullptr;
		int Flags = 0;
		int SwitchWeight = 0;
		const char* PickupSoundOverride = nullptr;

		virtual void Validate() const override
		{
			ASSERTSZ_Q(Classname, "Weapon must have a classname.");
			ASSERTSZ_Q(Id != WeaponId_e::WeaponNone, "Weapon must have a valid ID.");
		}
	};
}  // namespace WeaponAtts
