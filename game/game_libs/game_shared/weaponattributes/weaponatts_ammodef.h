#pragma once

#include "BuildPlatform/Utils.h"
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

		bool GivenByPickup(const char* classname) const
		{
			if ( !classname || !(*classname) )
			{
				return false;
			}

			for ( size_t index = 0; index < SIZE_OF_ARRAY(PickupClassnames); ++index )
			{
				const char* candidate = PickupClassnames[index];

				if ( candidate && *candidate && strcmp(candidate, classname) == 0 )
				{
					return true;
				}
			}

			return false;
		}

		void Validate() const override
		{
			ASSERTSZ_Q(PrimaryAmmoOnFirstPickup >= 0, "Amount of primary ammo on pickup must be positive.");
		}
	};
}  // namespace WeaponAtts
