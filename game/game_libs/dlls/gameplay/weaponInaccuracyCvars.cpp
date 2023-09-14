#include "gameplay/weaponInaccuracyCvars.h"
#include "gameplay/inaccuracymodifiers.h"
#include "gameplay/inaccuracyCvars.h"
#include "standard_includes.h"
#include "EnginePublicAPI/cvardef.h"
#include "utlstring.h"
#include "mp_utils.h"
#include "weapons/weaponregistry.h"
#include "weapons/genericweapon.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"
#include "gameplay/gameplaySystems.h"
#include "util/cvarFuncs.h"

namespace WeaponInaccuracyCvars
{
#define DEFINE_CVAR(name, value) {(char*)name, (char*)#value, 0, (float)value, nullptr};

	// By default this is slightly lower than the max movement speed of the player,
	// to avoid jitter at the top end of the velocity.
	static cvar_t sv_weapon_inaccuracy_maxspeed = DEFINE_CVAR(CVARNAME_WEAPON_INACCURACY_MAXSPEED, 275);
	static cvar_t sv_weapon_inaccuracy_maxfallspeed = DEFINE_CVAR(CVARNAME_WEAPON_INACCURACY_MAXFALLSPEED, 400);

	static cvar_t sv_weapon_debug_inac_enabled = DEFINE_CVAR(CVARNAME_WEAPON_DEBUC_INAC_ENABLED, 0);
	static cvar_t sv_weapon_debug_inac_restvalue = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_RESTVALUE, 0.1);
	static cvar_t sv_weapon_debug_inac_restspread = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_RESTSPREAD, 0);
	static cvar_t sv_weapon_debug_inac_runvalue = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_RUNVALUE, 0.5);
	static cvar_t sv_weapon_debug_inac_runspread = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_RUNSPREAD, 0);
	static cvar_t sv_weapon_debug_inac_crouchshift = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_CROUCHSHIFT, -0.08);
	static cvar_t sv_weapon_debug_inac_airshift = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_AIRSHIFT, 0.2);
	static cvar_t sv_weapon_debug_inac_fallshift = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_FALLSHIFT, 0.05);
	static cvar_t sv_weapon_debug_inac_attackcoeff = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_ATTACKCOEFF, 0.5);
	static cvar_t sv_weapon_debug_inac_decaycoeff = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_DECAYCOEFF, 0.5);
	static cvar_t sv_weapon_debug_inac_fireimpulse = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSE, 0.1);
	static cvar_t sv_weapon_debug_inac_fireimpulseceil = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSECEIL, 0.3);
	static cvar_t sv_weapon_debug_inac_fireimpulsehold = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSEHOLD, 0.05);
	static cvar_t sv_weapon_debug_inac_fireimpulsedecaywin = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSEDECAYWIN, 0.0);
	static cvar_t sv_weapon_debug_inac_fireimpulsedecaymod = DEFINE_CVAR(CVARNAME_WEAPON_DEBUG_INAC_FIREIMPULSEDECAYMOD, 1.0);

#undef DEFINE_CVAR

	static inline void AddValue(CUtlString& str, const cvar_t& cvar)
	{
		str.AppendFormat("  %s = %s\n", cvar.name, cvar.string);
	}

	static void DumpValues(void)
	{
		if ( CVAR_GET_FLOAT("sv_cheats") == 0.0f )
		{
			ALERT(at_console, "This command requires sv_cheats to be 1.\n");
			return;
		}

		CUtlString output;

		output.AppendFormat("Weapon inaccuracy values:\n");
		output.AppendFormat(
			"  %s = %s\n",
			sv_weapon_debug_inac_enabled.name,
			sv_weapon_debug_inac_enabled.value ? "true" : "false");

		AddValue(output, sv_weapon_debug_inac_restvalue);
		AddValue(output, sv_weapon_debug_inac_restspread);
		AddValue(output, sv_weapon_debug_inac_runvalue);
		AddValue(output, sv_weapon_debug_inac_runspread);
		AddValue(output, sv_weapon_debug_inac_crouchshift);
		AddValue(output, sv_weapon_debug_inac_airshift);
		AddValue(output, sv_weapon_debug_inac_fallshift);
		AddValue(output, sv_weapon_debug_inac_attackcoeff);
		AddValue(output, sv_weapon_debug_inac_decaycoeff);
		AddValue(output, sv_weapon_debug_inac_fireimpulse);
		AddValue(output, sv_weapon_debug_inac_fireimpulseceil);
		AddValue(output, sv_weapon_debug_inac_fireimpulsehold);
		AddValue(output, sv_weapon_debug_inac_fireimpulsedecaywin);
		AddValue(output, sv_weapon_debug_inac_fireimpulsedecaymod);

		ALERT(at_console, "%s", output.Get());
	}

	static void PopulateVector2DCvar(const char* name, const Vector2D& vec)
	{
		if ( vec.x == vec.y )
		{
			g_engfuncs.pfnCVarSetFloat(name, vec.x);
			return;
		}

		char buffer[32];
		Vector2DToCvarString(vec, buffer, sizeof(buffer));

		g_engfuncs.pfnCVarSetString(name, buffer);
	}

	static void PopulateCvars(const WeaponAtts::AccuracyParameters& params)
	{
		g_engfuncs.pfnCVarSetFloat(sv_weapon_debug_inac_restvalue.name, params.RestValue);
		PopulateVector2DCvar(sv_weapon_debug_inac_restspread.name, params.RestSpread);
		g_engfuncs.pfnCVarSetFloat(sv_weapon_debug_inac_runvalue.name, params.RunValue);
		PopulateVector2DCvar(sv_weapon_debug_inac_runspread.name, params.RunSpread);
		g_engfuncs.pfnCVarSetFloat(sv_weapon_debug_inac_crouchshift.name, params.CrouchShift);
		g_engfuncs.pfnCVarSetFloat(sv_weapon_debug_inac_airshift.name, params.AirShift);
		g_engfuncs.pfnCVarSetFloat(sv_weapon_debug_inac_fallshift.name, params.FallShift);
		g_engfuncs.pfnCVarSetFloat(sv_weapon_debug_inac_attackcoeff.name, params.AttackCoefficient);
		g_engfuncs.pfnCVarSetFloat(sv_weapon_debug_inac_decaycoeff.name, params.DecayCoefficient);
		g_engfuncs.pfnCVarSetFloat(sv_weapon_debug_inac_fireimpulse.name, params.FireImpulse);
		g_engfuncs.pfnCVarSetFloat(sv_weapon_debug_inac_fireimpulseceil.name, params.FireImpulseCeiling);
		g_engfuncs.pfnCVarSetFloat(sv_weapon_debug_inac_fireimpulsehold.name, params.FireImpulseHoldTime);
		g_engfuncs.pfnCVarSetFloat(sv_weapon_debug_inac_fireimpulsedecaywin.name, params.FireImpulseDecayWindow);
		g_engfuncs.pfnCVarSetFloat(sv_weapon_debug_inac_fireimpulsedecaymod.name, params.FireImpulseDecayMod);
	}

	static void PopulateCvarsFromPlayerWeapon(void)
	{
		if ( CVAR_GET_FLOAT("sv_cheats") == 0.0f )
		{
			ALERT(at_console, "This command requires sv_cheats to be 1.\n");
			return;
		}

		const bool isSingleplayer = !GameplaySystems::IsMultiplayer();
		const int argc = g_engfuncs.pfnCmd_Argc();

		if ( (isSingleplayer && argc != 1 && argc != 2) || (!isSingleplayer && argc != 2) )
		{
			ALERT(at_console, "Usage: sv_weapon_debug_inac_populate <#playerId|playerName>\n");
			ALERT(at_console, "If in single player, the player name can be omitted.\n");
			return;
		}

		CBasePlayer* player = nullptr;

		if ( isSingleplayer && argc < 2 )
		{
			// Single player and no player name specified, so infer local player to use.
			player = MPUtils::CBasePlayerFromIndex(1);
		}
		else
		{
			player = MPUtils::CBasePlayerFromStringLookup(g_engfuncs.pfnCmd_Argv(1));
		}

		if ( !player )
		{
			ALERT(at_console, "Could not find player specified player.\n");
			return;
		}

		CGenericWeapon* weapon = dynamic_cast<CGenericWeapon*>(player->m_pActiveItem);

		if ( !weapon )
		{
			ALERT(at_console, "Player does not have an active weapon.\n");
			return;
		}

		const WeaponAtts::WACollection& atts = weapon->WeaponAttributes();
		byte attackMode = weapon->GetPrimaryAttackModeIndex();

		if ( attackMode >= atts.AttackModes.Count() )
		{
			ALERT(
				at_console,
				"Player's active weapon %s (%d) has invalid attack mode index.\n",
				atts.Core.Classname,
				atts.Core.Id);
			return;
		}

		const WeaponAtts::WAAmmoBasedAttack* attack =
			dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(atts.AttackModes[attackMode].get());

		if ( !attack )
		{
			ALERT(
				at_console,
				"Player's active weapon %s (%d) has unsupported attack mode.\n",
				atts.Core.Classname,
				atts.Core.Id);
			return;
		}

		PopulateCvars(attack->Accuracy);

		ALERT(
			at_console,
			"Weapon inaccuravy convars populated from player's active weapon %s (%d).\n",
			atts.Core.Classname,
			atts.Core.Id);
	}

	void Init()
	{
		CVAR_REGISTER(&sv_weapon_inaccuracy_maxspeed);
		CVAR_REGISTER(&sv_weapon_inaccuracy_maxfallspeed);

		CVAR_REGISTER(&sv_weapon_debug_inac_enabled);
		CVAR_REGISTER(&sv_weapon_debug_inac_restvalue);
		CVAR_REGISTER(&sv_weapon_debug_inac_restspread);
		CVAR_REGISTER(&sv_weapon_debug_inac_runvalue);
		CVAR_REGISTER(&sv_weapon_debug_inac_runspread);
		CVAR_REGISTER(&sv_weapon_debug_inac_crouchshift);
		CVAR_REGISTER(&sv_weapon_debug_inac_airshift);
		CVAR_REGISTER(&sv_weapon_debug_inac_fallshift);
		CVAR_REGISTER(&sv_weapon_debug_inac_attackcoeff);
		CVAR_REGISTER(&sv_weapon_debug_inac_decaycoeff);
		CVAR_REGISTER(&sv_weapon_debug_inac_fireimpulse);
		CVAR_REGISTER(&sv_weapon_debug_inac_fireimpulseceil);
		CVAR_REGISTER(&sv_weapon_debug_inac_fireimpulsehold);
		CVAR_REGISTER(&sv_weapon_debug_inac_fireimpulsedecaywin);
		CVAR_REGISTER(&sv_weapon_debug_inac_fireimpulsedecaymod);

		g_engfuncs.pfnAddServerCommand(CVARNAME_WEAPON_DEBUG_INAC_DUMP, &DumpValues);
		g_engfuncs.pfnAddServerCommand(CVARNAME_WEAPON_DEBUG_INAC_POPULATE, &PopulateCvarsFromPlayerWeapon);
	}
}  // namespace GameplayCvars
