#include "gameplay/crosshairCvars.h"
#include "gameplay/inaccuracyCvars.h"
#include "EnginePublicAPI/cvardef.h"
#include "cl_dll.h"
#include "debug_assert.h"
#include "hud.h"
#include "weapons/weaponregistry.h"
#include "weaponattributes/weaponatts_collection.h"
#include "weaponatts_ammobasedattack.h"

namespace CrosshairCvars
{
	static cvar_t* CvarCheats = nullptr;
	static cvar_t* CvarDebugSpread = nullptr;
	static cvar_t* CvarOverrideCrosshairAtts = nullptr;
	static cvar_t* CvarOverrideCrosshairRadiusMin = nullptr;
	static cvar_t* CvarOverrideCrosshairRadiusMax = nullptr;
	static cvar_t* CvarOverrideCrosshairBarLengthMin = nullptr;
	static cvar_t* CvarOverrideCrosshairBarLengthMax = nullptr;
	static bool CvarsLoaded = false;

	static inline cvar_t* GetOrCreateClientCvar(const char* name, const char* defaultValue = "0")
	{
		cvar_t* cvar = gEngfuncs.pfnGetCvarPointer(name);

		if ( !cvar )
		{
			cvar = gEngfuncs.pfnRegisterVariable(name, defaultValue, FCVAR_CLIENTDLL);
		}

		return cvar;
	}

	static void PopulateCvars(const WeaponAtts::CrosshairParameters& params)
	{
		if ( !CvarsLoaded )
		{
			return;
		}

		gEngfuncs.Cvar_SetValue(CvarOverrideCrosshairRadiusMin->name, params.RadiusMin);
		gEngfuncs.Cvar_SetValue(CvarOverrideCrosshairRadiusMax->name, params.RadiusMax);
		gEngfuncs.Cvar_SetValue(CvarOverrideCrosshairBarLengthMin->name, params.BarScaleMin);
		gEngfuncs.Cvar_SetValue(CvarOverrideCrosshairBarLengthMax->name, params.BarScaleMax);
	}

	static void PopulateCvarsFromCurrentWeapon(void)
	{
		if ( !CvarsLoaded )
		{
			gEngfuncs.Con_Printf("Convars unable to be loaded.\n");
			return;
		}

		if ( !CvarCheats->value )
		{
			gEngfuncs.Con_Printf("Cheats must be enabled to execute this command.\n");
			return;
		}

		WEAPON* currentWeapon = gHUD.m_Ammo.GetCurrentWeapon();

		if ( !currentWeapon )
		{
			gEngfuncs.Con_Printf("Could not get current client weapon.\n");
			return;
		}

		CWeaponRegistry& registry = CWeaponRegistry::StaticInstance();
		const WeaponAtts::WACollection* atts = registry.Get(currentWeapon->iId);

		if ( !atts )
		{
			gEngfuncs.Con_Printf(
				"Could not get attributes for current client weapon %s (%d).\n",
				currentWeapon->szName,
				currentWeapon->iId);
			return;
		}

		if ( currentWeapon->iPriAttackMode >= atts->AttackModes.Count() )
		{
			gEngfuncs.Con_Printf(
				"Current client weapon %s (%d) had invalid attack mode index %d.\n",
				currentWeapon->szName,
				currentWeapon->iId,
				currentWeapon->iPriAttackMode);
			return;
		}

		const WeaponAtts::WABaseAttack* attackMode = atts->AttackModes[currentWeapon->iPriAttackMode].get();
		const WeaponAtts::WAAmmoBasedAttack* ammoAttack =
			dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(attackMode);

		if ( !ammoAttack )
		{
			gEngfuncs.Con_Printf(
				"Current client weapon %s (%d) uses an unsupported attack mode.\n",
				currentWeapon->szName,
				currentWeapon->iId);
			return;
		}

		PopulateCvars(ammoAttack->Crosshair);

		gEngfuncs.Con_Printf(
			"Crosshair convars populated from current client weapon %s (%d).\n",
			currentWeapon->szName,
			currentWeapon->iId);
	}

	static inline void AddValue(CUtlString& str, const cvar_t* cvar)
	{
		if ( cvar )
		{
			str.AppendFormat("  %s = %f\n", cvar->name, cvar->value);
		}
	}

	static void DumpCvars(void)
	{
		if ( !CvarsLoaded )
		{
			gEngfuncs.Con_Printf("Convars unable to be loaded.\n");
			return;
		}

		if ( !CvarCheats->value )
		{
			gEngfuncs.Con_Printf("Cheats must be enabled to execute this command.\n");
		}

		CUtlString output;

		output.AppendFormat("Crosshair values:\n");
		output.AppendFormat("  cl_crosshair_ovr_enabled = %s\n", CvarOverrideCrosshairAtts->value ? "true" : "false");

		AddValue(output, CvarOverrideCrosshairRadiusMin);
		AddValue(output, CvarOverrideCrosshairRadiusMax);
		AddValue(output, CvarOverrideCrosshairBarLengthMin);
		AddValue(output, CvarOverrideCrosshairBarLengthMax);

		gEngfuncs.Con_Printf("%s", output.Get());
	}

	void Init()
	{
		CvarCheats = gEngfuncs.pfnGetCvarPointer("sv_cheats");
		CvarDebugSpread = GetOrCreateClientCvar(CVARNAME_DEBUG_WEAPON_SPREAD);
		CvarOverrideCrosshairAtts = GetOrCreateClientCvar(CVARNAME_CROSSHAIR_OVR_ENABLED);
		CvarOverrideCrosshairRadiusMin = GetOrCreateClientCvar(CVARNAME_CROSSHAIR_OVR_RAD_MIN, "0");
		CvarOverrideCrosshairRadiusMax = GetOrCreateClientCvar(CVARNAME_CROSSHAIR_OVR_RAD_MAX, "0.5");
		CvarOverrideCrosshairBarLengthMin = GetOrCreateClientCvar(CVARNAME_CROSSHAIR_OVR_BAR_MIN, "0.04");
		CvarOverrideCrosshairBarLengthMax = GetOrCreateClientCvar(CVARNAME_CROSSHAIR_OVR_BAR_MAX, "0.03");

		CvarsLoaded = CvarCheats && CvarDebugSpread && CvarOverrideCrosshairAtts && CvarOverrideCrosshairRadiusMin &&
			CvarOverrideCrosshairRadiusMax && CvarOverrideCrosshairBarLengthMin && CvarOverrideCrosshairBarLengthMax;

		ASSERTSZ(CvarsLoaded, "Unable to load crosshair debugging cvars.");

		gEngfuncs.pfnAddCommand(CVARNAME_CROSSHAIR_OVR_POPULATE, &PopulateCvarsFromCurrentWeapon);
		gEngfuncs.pfnAddCommand(CVARNAME_CROSSHAIR_OVR_DUMP, &DumpCvars);
	}

	bool SpreadVisualisationEnabled()
	{
		return CvarsLoaded && CvarCheats->value != 0.0f && static_cast<int>(CvarDebugSpread->value) != 0;
	}

	bool CrosshairOverrideEnabled()
	{
		return CvarsLoaded && CvarCheats->value != 0.0f && CvarOverrideCrosshairAtts->value != 0.0f;
	}

	void PopulateCrosshairParametersFromDebugCvars(WeaponAtts::CrosshairParameters& params)
	{
		params.RadiusMin = RadiusMin();
		params.RadiusMax = RadiusMax();
		params.BarScaleMin = BarLengthMin();
		params.BarScaleMax = BarLengthMax();
	}

	int AttackModeForSpreadVisualisation()
	{
		return CvarsLoaded ? static_cast<int>(CvarDebugSpread->value) : 0;
	}

	float RadiusMin()
	{
		return CvarsLoaded ? CvarOverrideCrosshairRadiusMin->value : 0.0f;
	}

	float RadiusMax()
	{
		return CvarsLoaded ? CvarOverrideCrosshairRadiusMax->value : 0.0f;
	}

	float BarLengthMin()
	{
		return CvarsLoaded ? CvarOverrideCrosshairBarLengthMin->value : 0.0f;
	}

	float BarLengthMax()
	{
		return CvarsLoaded ? CvarOverrideCrosshairBarLengthMax->value : 0.0f;
	}
}  // namespace CrosshairCvars
