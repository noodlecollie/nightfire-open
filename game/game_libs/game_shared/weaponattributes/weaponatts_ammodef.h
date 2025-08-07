#pragma once

#include "weaponatts_base.h"
#include "ammodefs.h"
#include "weapons.h"

namespace WeaponAtts
{
	struct WAAmmoDef : public WABase
	{
		const CAmmoDef* PrimaryAmmo = nullptr;
		const CAmmoDef* SecondaryAmmo = nullptr;
		int PrimaryAmmoOnFirstPickup = 0;
		int MaxClip = WEAPON_NOCLIP;
		const char* PickupClassnames[8];

		WAAmmoDef()
		{
			memset(PickupClassnames, 0, sizeof(PickupClassnames));
		}

		virtual void Validate() const override
		{
			ASSERTSZ_Q(PrimaryAmmoOnFirstPickup >= 0, "Amount of primary ammo on pickup must be positive.");
		}
	};
}  // namespace WeaponAtts
