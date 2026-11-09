#include "gameplay/inaccuracymodifiers.h"
#include "gameplay/inaccuracyCvars.h"
#include "standard_includes.h"
#include "miniutl.h"
#include "util/extramath.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"
#include "util/cvarFuncs.h"

namespace InaccuracyModifiers
{
	struct Cvars
	{
		cvar_t* CvarCheats;
		cvar_t* CvarEnableDebugging;
		cvar_t* CvarDebugRestValue;
		cvar_t* CvarDebugRestSpread;
		cvar_t* CvarDebugRunValue;
		cvar_t* CvarDebugRunSpread;
		cvar_t* CvarDebugCrouchShift;
		cvar_t* CvarDebugAirShift;
		cvar_t* CvarDebugFallShift;
		cvar_t* CvarDebugAttackCoefficient;
		cvar_t* CvarDebugDecayCoefficient;
		cvar_t* CvarDebugFireImpulse;
		cvar_t* CvarDebugFireImpulseCeiling;
		cvar_t* CvarDebugFireImpulseHoldTime;
		cvar_t* CvarDebugFireImpulseDecayWindow;
		cvar_t* CvarDebugFireImpulseDecayMod;
	};

	static Cvars gAllCvars;
	static bool gCvarInitWasRun = false;
	static bool gCvarsLoaded = false;

	static Vector2D ParseCvarAsVector2D(const cvar_t& cvar)
	{
		bool ok = false;
		Vector2D vec = CvarStringToVector2D(cvar.string, &ok);

		if ( ok )
		{
			return vec;
		}

		return Vector2D(cvar.value, cvar.value);
	}

	static void PopulateVector2DCvar(cvar_t& cvar, const Vector2D& vec)
	{
		if ( vec.x == vec.y )
		{
			cvar.value = vec.x;
			return;
		}

		char buffer[32];
		Vector2DToCvarString(vec, buffer, sizeof(buffer));

#ifdef CLIENT_DLL
		gEngfuncs.Cvar_Set(cvar.name, buffer);
#else
		g_engfuncs.pfnCvar_DirectSet(&cvar, buffer);
#endif
	}

	bool Init()
	{
		if ( gCvarInitWasRun )
		{
			return gCvarsLoaded;
		}

#define GET_CVAR(variable, cvarName) \
	do \
	{ \
		if ( gCvarsLoaded ) \
		{ \
			variable = GetCvarByName(cvarName); \
			gCvarsLoaded = variable != nullptr; \
		} \
	} \
	while ( false )

		// Start out assuming all will load
		gCvarsLoaded = true;
		memset(&gAllCvars, 0, sizeof(gAllCvars));

		GET_CVAR(gAllCvars.CvarCheats, "sv_cheats");
		GET_CVAR(gAllCvars.CvarEnableDebugging, CVARNAME_WEAPON_DEBUC_INAC_ENABLED);
		GET_CVAR(gAllCvars.CvarDebugRestValue, CVARNAME_WEAPON_DEBUG_INAC_RESTVALUE);
		GET_CVAR(gAllCvars.CvarDebugRestSpread, CVARNAME_WEAPON_DEBUG_INAC_RESTSPREAD);
		GET_CVAR(gAllCvars.CvarDebugRunValue, CVARNAME_WEAPON_DEBUG_INAC_RUNVALUE);
		GET_CVAR(gAllCvars.CvarDebugRunSpread, CVARNAME_WEAPON_DEBUG_INAC_RUNSPREAD);
		GET_CVAR(gAllCvars.CvarDebugCrouchShift, CVARNAME_WEAPON_DEBUG_INAC_CROUCHSHIFT);
		GET_CVAR(gAllCvars.CvarDebugAirShift, CVARNAME_WEAPON_DEBUG_INAC_AIRSHIFT);
		GET_CVAR(gAllCvars.CvarDebugFallShift, CVARNAME_WEAPON_DEBUG_INAC_FALLSHIFT);
		GET_CVAR(gAllCvars.CvarDebugAttackCoefficient, CVARNAME_WEAPON_DEBUG_INAC_ATTACKCOEFF);
		GET_CVAR(gAllCvars.CvarDebugDecayCoefficient, CVARNAME_WEAPON_DEBUG_INAC_DECAYCOEFF);
		GET_CVAR(gAllCvars.CvarDebugFireImpulse, CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSE);
		GET_CVAR(gAllCvars.CvarDebugFireImpulseCeiling, CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSECEIL);
		GET_CVAR(gAllCvars.CvarDebugFireImpulseHoldTime, CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSEHOLD);
		GET_CVAR(gAllCvars.CvarDebugFireImpulseDecayWindow, CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSEDECAYWIN);
		GET_CVAR(gAllCvars.CvarDebugFireImpulseDecayMod, CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSEDECAYMOD);

#undef GET_CVAR

		gCvarInitWasRun = true;
		return gCvarsLoaded;
	}

	void Deinit()
	{
		memset(&gAllCvars, 0, sizeof(gAllCvars));
		gCvarsLoaded = false;
		gCvarInitWasRun = false;
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
		ASSERT(gCvarInitWasRun);
		return gCvarsLoaded && gAllCvars.CvarCheats->value != 0.0f && gAllCvars.CvarEnableDebugging->value != 0.0f;
	}

	bool GetInaccuracyValuesFromDebugCvars(WeaponAtts::AccuracyParameters& params)
	{
		ASSERT(gCvarInitWasRun);

		if ( !gCvarsLoaded )
		{
			return false;
		}

		params.RestValue = gAllCvars.CvarDebugRestValue->value;
		params.RestSpread = ParseCvarAsVector2D(*gAllCvars.CvarDebugRestSpread);
		params.RunValue = gAllCvars.CvarDebugRunValue->value;
		params.RunSpread = ParseCvarAsVector2D(*gAllCvars.CvarDebugRunSpread);
		params.CrouchShift = gAllCvars.CvarDebugCrouchShift->value;
		params.AirShift = gAllCvars.CvarDebugAirShift->value;
		params.FallShift = gAllCvars.CvarDebugFallShift->value;
		params.AttackCoefficient = gAllCvars.CvarDebugAttackCoefficient->value;
		params.DecayCoefficient = gAllCvars.CvarDebugDecayCoefficient->value;
		params.FireImpulse = gAllCvars.CvarDebugFireImpulse->value;
		params.FireImpulseCeiling = gAllCvars.CvarDebugFireImpulseCeiling->value;
		params.FireImpulseHoldTime = gAllCvars.CvarDebugFireImpulseHoldTime->value;
		params.FireImpulseDecayWindow = gAllCvars.CvarDebugFireImpulseDecayWindow->value;
		params.FireImpulseDecayMod = gAllCvars.CvarDebugFireImpulseDecayMod->value;

		return true;
	}

	void SetInaccuracyDebugCvarsFromValues(const WeaponAtts::AccuracyParameters& params)
	{
		ASSERT(gCvarInitWasRun);

		if ( !gCvarsLoaded )
		{
			return;
		}

		gAllCvars.CvarDebugRestValue->value = params.RestValue;
		PopulateVector2DCvar(*gAllCvars.CvarDebugRestSpread, params.RestSpread);
		gAllCvars.CvarDebugRunValue->value = params.RunValue;
		PopulateVector2DCvar(*gAllCvars.CvarDebugRunSpread, params.RunSpread);
		gAllCvars.CvarDebugCrouchShift->value = params.CrouchShift;
		gAllCvars.CvarDebugAirShift->value = params.AirShift;
		gAllCvars.CvarDebugFallShift->value = params.FallShift;
		gAllCvars.CvarDebugAttackCoefficient->value = params.AttackCoefficient;
		gAllCvars.CvarDebugDecayCoefficient->value = params.AttackCoefficient;
		gAllCvars.CvarDebugFireImpulse->value = params.FireImpulse;
		gAllCvars.CvarDebugFireImpulseCeiling->value = params.FireImpulseCeiling;
		gAllCvars.CvarDebugFireImpulseHoldTime->value = params.FireImpulseHoldTime;
		gAllCvars.CvarDebugFireImpulseDecayWindow->value = params.FireImpulseDecayWindow;
		gAllCvars.CvarDebugFireImpulseDecayMod->value = params.FireImpulseDecayMod;
	}
}  // namespace InaccuracyModifiers
