#pragma once

#include "vector_classes.h"

class CBasePlayer;

namespace WeaponAtts
{
	struct AccuracyParameters;
}

// Names of debugging cvars, for convenience.
#define CVARNAME_WEAPON_INACCURACY_MAXSPEED "sv_weapon_inaccuracy_maxspeed"
#define CVARNAME_WEAPON_INACCURACY_MAXFALLSPEED "sv_weapon_inaccuracy_maxfallspeed"
#define CVARNAME_WEAPON_DEBUC_INAC_ENABLED "sv_weapon_debug_inac_enabled"
#define CVARNAME_WEAPON_DEBUG_INAC_RESTVALUE "sv_weapon_debug_inac_restvalue"
#define CVARNAME_WEAPON_DEBUG_INAC_RESTSPREAD "sv_weapon_debug_inac_restspread"
#define CVARNAME_WEAPON_DEBUG_INAC_RUNVALUE "sv_weapon_debug_inac_runvalue"
#define CVARNAME_WEAPON_DEBUG_INAC_RUNSPREAD "sv_weapon_debug_inac_runspread"
#define CVARNAME_WEAPON_DEBUG_INAC_CROUCHSHIFT "sv_weapon_debug_inac_crouchshift"
#define CVARNAME_WEAPON_DEBUG_INAC_AIRSHIFT "sv_weapon_debug_inac_airshift"
#define CVARNAME_WEAPON_DEBUG_INAC_FALLSHIFT "sv_weapon_debug_inac_fallshift"
#define CVARNAME_WEAPON_DEBUG_INAC_ATTACKCOEFF "sv_weapon_debug_inac_attackcoeff"
#define CVARNAME_WEAPON_DEBUG_INAC_DECAYCOEFF "sv_weapon_debug_inac_decaycoeff"
#define CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSE "sv_weapon_debug_inac_fireimpulse"
#define CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSECEIL "sv_weapon_debug_inac_fireimpulseceil"
#define CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSEHOLD "sv_weapon_debug_inac_fireimpulsehold"

namespace InaccuracyModifiers
{
	Vector2D GetInterpolatedSpread(const WeaponAtts::AccuracyParameters& params, float inaccuracy);

	bool IsInaccuracyDebuggingEnabled();
	bool GetInaccuracyValuesFromDebugCvars(WeaponAtts::AccuracyParameters& params);
	void SetInaccuracyDebugCvarsFromValues(const WeaponAtts::AccuracyParameters& params);
}  // namespace InaccuracyModifiers
