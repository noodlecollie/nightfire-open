#include "weaponslots.h"
#include "standard_includes.h"
#include "weapons.h"
#include "weaponids.h"

const int WEAPON_HUD_SLOTS[MAX_WEAPON_SLOTS][MAX_WEAPON_POSITIONS] = {
	// Slot 1
	{
		static_cast<int>(WeaponId_e::WeaponFists),

		// Debug weapons
		static_cast<int>(WeaponId_e::WeaponSilent),
	},

	// Slot 2
	{
		static_cast<int>(WeaponId_e::WeaponP99),
		static_cast<int>(WeaponId_e::WeaponRaptor),
	},

	// Slot 3
	{
		static_cast<int>(WeaponId_e::WeaponFrinesi),
		static_cast<int>(WeaponId_e::WeaponMP9),
		static_cast<int>(WeaponId_e::WeaponL96A1),
	},

	// Slot 4
	{
		static_cast<int>(WeaponId_e::WeaponGrenadeLauncher),
	},

	// Slot 5
	{
		static_cast<int>(WeaponId_e::WeaponFragGrenade),
	},
};
