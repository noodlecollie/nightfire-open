#include "weaponslots.h"
#include "standard_includes.h"
#include "weapons.h"
#include "weaponids.h"

const int WEAPON_HUD_SLOTS[MAX_WEAPON_SLOTS][MAX_WEAPON_POSITIONS] =
{
	// Slot 1
	{
		WEAPON_CROWBAR,
		static_cast<int>(WeaponId_e::WeaponFists),

		// Debug weapons
		static_cast<int>(WeaponId_e::WeaponSilent)
	},

	// Slot 2
	{
		WEAPON_GLOCK,
		WEAPON_PYTHON,
		static_cast<int>(WeaponId_e::WeaponP99),
		static_cast<int>(WeaponId_e::WeaponRaptor),
	},

	// Slot 3
	{
		WEAPON_MP5,
		WEAPON_SHOTGUN,
		WEAPON_CROSSBOW,
		static_cast<int>(WeaponId_e::WeaponFrinesi),
		static_cast<int>(WeaponId_e::WeaponL96A1)
	},

	// Slot 4
	{
		WEAPON_RPG,
		WEAPON_GAUSS,
		WEAPON_EGON,
		WEAPON_HORNETGUN,
		static_cast<int>(WeaponId_e::WeaponGrenadeLauncher)
	},

	// Slot 5
	{
		WEAPON_HANDGRENADE,
		WEAPON_SATCHEL,
		WEAPON_TRIPMINE,
		WEAPON_SNARK
	}
};
