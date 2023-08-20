#include "gameplay/inaccuracymodifiers.h"
#include "gameplay/inaccuracyCvars.h"
#include "standard_includes.h"
#include "miniutl.h"
#include "util/extramath.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"
#include "util/cvarFuncs.h"

namespace InaccuracyModifiers
{
	cvar_t* CvarCheats = nullptr;
	cvar_t* CvarEnableDebugging = nullptr;
	cvar_t* CvarDebugRestValue = nullptr;
	cvar_t* CvarDebugRestSpread = nullptr;
	cvar_t* CvarDebugRunValue = nullptr;
	cvar_t* CvarDebugRunSpread = nullptr;
	cvar_t* CvarDebugCrouchShift = nullptr;
	cvar_t* CvarDebugAirShift = nullptr;
	cvar_t* CvarDebugFallShift = nullptr;
	cvar_t* CvarDebugAttackCoefficient = nullptr;
	cvar_t* CvarDebugDecayCoefficient = nullptr;
	cvar_t* CvarDebugFireImpulse = nullptr;
	cvar_t* CvarDebugFireImpulseCeiling = nullptr;
	cvar_t* CvarDebugFireImpulseHoldTime = nullptr;

	bool CvarInitWasRun = false;
	bool CvarsLoaded = false;

	void InitCvars()
	{
		if ( CvarInitWasRun )
		{
			return;
		}

		CvarCheats = GetCvarByName("sv_cheats");
		CvarEnableDebugging = GetCvarByName(CVARNAME_WEAPON_DEBUC_INAC_ENABLED);
		CvarDebugRestValue = GetCvarByName(CVARNAME_WEAPON_DEBUG_INAC_RESTVALUE);
		CvarDebugRestSpread = GetCvarByName(CVARNAME_WEAPON_DEBUG_INAC_RESTSPREAD);
		CvarDebugRunValue = GetCvarByName(CVARNAME_WEAPON_DEBUG_INAC_RUNVALUE);
		CvarDebugRunSpread = GetCvarByName(CVARNAME_WEAPON_DEBUG_INAC_RUNSPREAD);
		CvarDebugCrouchShift = GetCvarByName(CVARNAME_WEAPON_DEBUG_INAC_CROUCHSHIFT);
		CvarDebugAirShift = GetCvarByName(CVARNAME_WEAPON_DEBUG_INAC_AIRSHIFT);
		CvarDebugFallShift = GetCvarByName(CVARNAME_WEAPON_DEBUG_INAC_FALLSHIFT);
		CvarDebugAttackCoefficient = GetCvarByName(CVARNAME_WEAPON_DEBUG_INAC_ATTACKCOEFF);
		CvarDebugDecayCoefficient = GetCvarByName(CVARNAME_WEAPON_DEBUG_INAC_DECAYCOEFF);
		CvarDebugFireImpulse = GetCvarByName(CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSE);
		CvarDebugFireImpulseCeiling = GetCvarByName(CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSECEIL);
		CvarDebugFireImpulseHoldTime = GetCvarByName(CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSEHOLD);

		CvarsLoaded = CvarCheats && CvarEnableDebugging && CvarDebugRestValue && CvarDebugRestSpread &&
			CvarDebugRunValue && CvarDebugRunSpread && CvarDebugCrouchShift && CvarDebugAirShift &&
			CvarDebugFallShift && CvarDebugAttackCoefficient && CvarDebugDecayCoefficient && CvarDebugFireImpulse &&
			CvarDebugFireImpulseCeiling && CvarDebugFireImpulseHoldTime;

		ASSERTSZ(CvarsLoaded, "Unable to load inaccuracy debugging cvars.");

		CvarInitWasRun = true;
	}

	Vector2D GetInterpolatedSpread(const WeaponAtts::AccuracyParameters& params, float inaccuracy)
	{
		// The inaccuracy starts out in the overall range [0 1]. The rest and run values also live within this range.
		// Remap this to a new [0 1] value, where 0 represents rest and 1 represents running at full speed.
		// If the original inaccuracy was outside the range [RestValue RunValue], the remapped inaccuracy will be
		// outside the new range [0 1].
		const float remappedInaccuracy =
			ExtraMath::RemapLinear(inaccuracy, params.RestValue, params.RunValue, 0.0f, 1.0f, false);

		// Calculate a spread vector based on the remapped inaccuracy.
		const Vector2D spread = params.RestSpread + (remappedInaccuracy * (params.RunSpread - params.RestSpread));

		// Return this, but make sure components are not less than zero.
		return Vector2D(Max(spread.x, 0.0f), Max(spread.y, 0.0f));
	}

	bool IsInaccuracyDebuggingEnabled()
	{
		InitCvars();
		return CvarsLoaded && CvarCheats->value != 0.0f && CvarEnableDebugging->value != 0.0f;
	}

	bool GetInaccuracyValuesFromDebugCvars(WeaponAtts::AccuracyParameters& params)
	{
		InitCvars();

		if ( !CvarsLoaded )
		{
			return false;
		}

		params.RestValue = CvarDebugRestValue->value;
		params.RestSpread = Vector2D(CvarDebugRestSpread->value, CvarDebugRestSpread->value);
		params.RunValue = CvarDebugRunValue->value;
		params.RunSpread = Vector2D(CvarDebugRunSpread->value, CvarDebugRunSpread->value);
		params.CrouchShift = CvarDebugCrouchShift->value;
		params.AirShift = CvarDebugAirShift->value;
		params.FallShift = CvarDebugFallShift->value;
		params.AttackCoefficient = CvarDebugAttackCoefficient->value;
		params.AttackCoefficient = CvarDebugDecayCoefficient->value;
		params.FireImpulse = CvarDebugFireImpulse->value;
		params.FireImpulseCeiling = CvarDebugFireImpulseCeiling->value;
		params.FireImpulseHoldTime = CvarDebugFireImpulseHoldTime->value;

		return true;
	}

	void SetInaccuracyDebugCvarsFromValues(const WeaponAtts::AccuracyParameters& params)
	{
		InitCvars();

		if ( !CvarsLoaded )
		{
			return;
		}

		CvarDebugRestValue->value = params.RestValue;
		CvarDebugRestSpread->value = params.RestSpread.x;
		CvarDebugRunValue->value = params.RunValue;
		CvarDebugRunSpread->value = params.RunSpread.x;
		CvarDebugCrouchShift->value = params.CrouchShift;
		CvarDebugAirShift->value = params.AirShift;
		CvarDebugFallShift->value = params.FallShift;
		CvarDebugAttackCoefficient->value = params.AttackCoefficient;
		CvarDebugDecayCoefficient->value = params.AttackCoefficient;
		CvarDebugFireImpulse->value = params.FireImpulse;
		CvarDebugFireImpulseCeiling->value = params.FireImpulseCeiling;
		CvarDebugFireImpulseHoldTime->value = params.FireImpulseHoldTime;
	}
}  // namespace InaccuracyModifiers
