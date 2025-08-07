#ifndef WEAPON_PREF_WEIGHTS_H
#define WEAPON_PREF_WEIGHTS_H

// All in one place, so it's easy to see how weapons compare to one another.
// Weapons with higher preference values are more desirable than those with
// lower values.
enum WeaponPreferenceWeight
{
	// For debug weapons - should never take priority.
	WeaponPref_Debug = 0,

	// Least preferable
	WeaponPref_Fists = 1,
	WeaponPref_FragGrenade,
	WeaponPref_P99,
	WeaponPref_MP9,
	WeaponPref_Raptor,
	WeaponPref_Frinesi,
	WeaponPref_Ronin,
	WeaponPref_GrenadeLauncher,
	WeaponPref_L96A1,
	WeaponPref_Max = WeaponPref_L96A1
	// Most preferable
};

#endif  // WEAPON_PREF_WEIGHTS_H
