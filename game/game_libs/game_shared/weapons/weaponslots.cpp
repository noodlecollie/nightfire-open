#include "weaponslots.h"
#include "standard_includes.h"
#include "weapons.h"
#include "weaponids.h"

const WeaponId_e WEAPON_HUD_SLOTS[MAX_WEAPON_SLOTS][MAX_WEAPON_POSITIONS] = {
	// Slot 1
	{
		WeaponId_e::WeaponFists,

		// Debug weapons
		WeaponId_e::WeaponSilent,
		WeaponId_e::WeaponHullTrace,
	},

	// Slot 2
	{
		WeaponId_e::WeaponP99,
		WeaponId_e::WeaponRaptor,
	},

	// Slot 3
	{
		WeaponId_e::WeaponFrinesi,
		WeaponId_e::WeaponMP9,
		WeaponId_e::WeaponL96A1,
	},

	// Slot 4
	{
		WeaponId_e::WeaponGrenadeLauncher,
	},

	// Slot 5
	{
		WeaponId_e::WeaponFragGrenade,
		WeaponId_e::WeaponRonin,
	},
};
