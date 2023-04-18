#include "gameplay/crosshairCvars.h"
#include "cvardef.h"
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
			gEngfuncs.Con_Printf("Could not get attributes for current client weapon %s (%d).\n",
								 currentWeapon->szName,
								 currentWeapon->iId);
			return;
		}

		if ( currentWeapon->iPriAttackMode >= atts->AttackModes.Count() )
		{
			gEngfuncs.Con_Printf("Current client weapon %s (%d) had invalid attack mode index %d.\n",
								 currentWeapon->szName,
								 currentWeapon->iId,
								 currentWeapon->iPriAttackMode);
			return;
		}

		const WeaponAtts::WABaseAttack* attackMode = atts->AttackModes[currentWeapon->iPriAttackMode].get();
		const WeaponAtts::WAAmmoBasedAttack* ammoAttack = dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(attackMode);

		if ( !ammoAttack )
		{
			gEngfuncs.Con_Printf("Current client weapon %s (%d) uses an unsupported attack mode.\n",
								 currentWeapon->szName,
								 currentWeapon->iId);
			return;
		}

		PopulateCvars(ammoAttack->Crosshair);

		gEngfuncs.Con_Printf("Convars populated from current client weapon %s (%d).\n",
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

		gEngfuncs.Con_Printf(output.Get());
	}

	void Init()
	{
		CvarCheats = gEngfuncs.pfnGetCvarPointer("sv_cheats");
		CvarDebugSpread = GetOrCreateClientCvar("cl_debug_weapon_spread");
		CvarOverrideCrosshairAtts = GetOrCreateClientCvar("cl_crosshair_ovr_enabled");
		CvarOverrideCrosshairRadiusMin = GetOrCreateClientCvar("cl_crosshair_ovr_rad_min", "0");
		CvarOverrideCrosshairRadiusMax = GetOrCreateClientCvar("cl_crosshair_ovr_rad_max", "0.5");
		CvarOverrideCrosshairBarLengthMin = GetOrCreateClientCvar("cl_crosshair_ovr_bar_min", "0.04");
		CvarOverrideCrosshairBarLengthMax = GetOrCreateClientCvar("cl_crosshair_ovr_bar_max", "0.03");

		CvarsLoaded =
			CvarCheats &&
			CvarDebugSpread &&
			CvarOverrideCrosshairAtts &&
			CvarOverrideCrosshairRadiusMin &&
			CvarOverrideCrosshairRadiusMax &&
			CvarOverrideCrosshairBarLengthMin &&
			CvarOverrideCrosshairBarLengthMax;

		ASSERTSZ(CvarsLoaded, "Unable to load crosshair debugging cvars.");

		gEngfuncs.pfnAddCommand("cl_crosshair_ovr_populate", &PopulateCvarsFromCurrentWeapon);
		gEngfuncs.pfnAddCommand("cl_crosshair_ovr_dump", &DumpCvars);
	}

	bool SpreadVisualisationEnabled()
	{
		return
			CvarsLoaded &&
			CvarCheats->value != 0.0f &&
			static_cast<int>(CvarDebugSpread->value) != 0;
	}

	bool CrosshairOverrideEnabled()
	{
		return
			CvarsLoaded &&
			CvarCheats->value != 0.0f &&
			CvarOverrideCrosshairAtts->value != 0.0f;
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
}
