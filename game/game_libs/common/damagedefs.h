#pragma once

// instant damage
#define DMG_GENERIC 0  // generic damage was done
#define DMG_CRUSH (1 << 0)  // crushed by falling or moving object
#define DMG_BULLET (1 << 1)  // shot
#define DMG_SLASH (1 << 2)  // cut, clawed, stabbed
#define DMG_BURN (1 << 3)  // heat burned
#define DMG_FREEZE (1 << 4)  // frozen
#define DMG_FALL (1 << 5)  // fell too far
#define DMG_BLAST (1 << 6)  // explosive blast damage
#define DMG_CLUB (1 << 7)  // crowbar, punch, headbutt
#define DMG_SHOCK (1 << 8)  // electric shock
#define DMG_SONIC (1 << 9)  // sound pulse shockwave
#define DMG_ENERGYBEAM (1 << 10)  // laser or other high energy beam
#define DMG_NEVERGIB (1 << 12)  // with this bit OR'd in, no damage type will be able to gib victims upon death
#define DMG_ALWAYSGIB (1 << 13)  // with this bit OR'd in, any damage type can be made to gib victims upon death.

// time-based damage
#define DMG_DROWN (1 << 14)  // Drowning
#define DMG_PARALYZE (1 << 15)  // slows affected creature down
#define DMG_NERVEGAS (1 << 16)  // nerve toxins, very bad
#define DMG_POISON (1 << 17)  // blood poisioning
#define DMG_RADIATION (1 << 18)  // radiation exposure
#define DMG_DROWNRECOVER (1 << 19)  // drowning recovery
#define DMG_ACID (1 << 20)  // toxic chemicals or acid burns
#define DMG_SLOWBURN (1 << 21)  // in an oven
#define DMG_SLOWFREEZE (1 << 22)  // in a subzero freezer
#define DMG_MORTAR (1 << 23)  // Hit by air raid (done to distinguish grenade from mortar)

// mask off TF-specific stuff too
#define DMG_TIMEBASED (~(0xff003fff))  // mask for time-based damage
#define DMG_FIRSTTIMEBASED DMG_DROWN

// TF ADDITIONS
#define DMG_IGNITE (1 << 24)  // Players hit by this begin to burn
#define DMG_RADIUS_MAX (1 << 25)  // Radius damage with this flag doesn't decrease over distance
#define DMG_RADIUS_QUAKE (1 << 26)  // Radius damage is done like Quake. 1/2 damage at 1/2 radius.
#define DMG_IGNOREARMOR (1 << 27)  // Damage ignores target's armor
#define DMG_AIMED (1 << 28)  // Does Hit location damage
#define DMG_WALLPIERCING (1 << 29)  // Blast Damages ents through walls

#define DMG_CALTROP (1 << 30)
#define DMG_HALLUC (1 << 31)

// TF Healing Additions for TakeHealth
#define DMG_IGNORE_MAXHEALTH DMG_IGNITE

// TF Redefines since we never use the originals
#define DMG_NAIL DMG_SLASH
#define DMG_NOT_SELF DMG_FREEZE

#define DMG_TRANQ DMG_MORTAR
#define DMG_CONCUSS DMG_SONIC

// these are the damage types that are allowed to gib corpses
#define DMG_GIB_CORPSE (DMG_CRUSH | DMG_FALL | DMG_BLAST | DMG_SONIC | DMG_CLUB)

// these are the damage types that have client hud art
#define DMG_SHOWNHUD \
	(DMG_POISON | DMG_ACID | DMG_FREEZE | DMG_SLOWFREEZE | DMG_DROWN | DMG_BURN | DMG_SLOWBURN | DMG_NERVEGAS | \
	 DMG_RADIATION | DMG_SHOCK)
