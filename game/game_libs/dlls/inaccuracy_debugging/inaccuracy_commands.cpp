#include "inaccuracy_debugging/inaccuracy_commands.h"
#include "client/clientCommandRegister.h"
#include "standard_includes.h"
#include "weapons/genericweapon.h"
#include "weapons/weaponregistry.h"
#include "gameplay/inaccuracymodifiers.h"

namespace InaccuracyDebugging
{
	static void HandleSetInaccuracyDebugVarsFromCurrentWeapon(CBasePlayer* player)
	{
		if ( !static_cast<int>(CVAR_GET_FLOAT("sv_cheats")) )
		{
			ClientPrintMsg(
				&player->edict()->v,
				HUD_PRINTCONSOLE,
				"Cannot use this command when cheats are not enabled.\n");

			return;
		}

		CGenericWeapon* weapon = dynamic_cast<CGenericWeapon*>(player->m_pActiveItem);

		if ( !weapon )
		{
			ClientPrintMsg(&player->edict()->v, HUD_PRINTCONSOLE, "No active compatible weapon.\n");
			return;
		}

		CWeaponRegistry& registry = CWeaponRegistry::StaticInstance();
		const WeaponAtts::WACollection* atts = registry.Get(weapon->m_iId);

		if ( !atts )
		{
			ClientPrintMsg(
				&player->edict()->v,
				HUD_PRINTCONSOLE,
				"Could not get attributes for current client weapon %s (%d).\n",
				weapon->pszName(),
				weapon->m_iId);

			return;
		}

		int priAttackModeIndex = weapon->GetPrimaryAttackModeIndex();

		if ( priAttackModeIndex >= atts->AttackModes.Count() )
		{
			ClientPrintMsg(
				&player->edict()->v,
				HUD_PRINTCONSOLE,
				"Current client weapon %s (%d) had invalid attack mode index %d.\n",
				weapon->pszName(),
				weapon->m_iId,
				priAttackModeIndex);

			return;
		}

		const WeaponAtts::WABaseAttack* attackMode = atts->AttackModes[priAttackModeIndex].get();
		const WeaponAtts::WAAmmoBasedAttack* ammoAttack =
			dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(attackMode);

		if ( !ammoAttack )
		{
			ClientPrintMsg(
				&player->edict()->v,
				HUD_PRINTCONSOLE,
				"Current client weapon %s (%d) uses an unsupported attack mode.\n",
				weapon->pszName(),
				weapon->m_iId);

			return;
		}

		InaccuracyModifiers::SetInaccuracyDebugCvarsFromValues(ammoAttack->Accuracy);

		ClientPrintMsg(
			&player->edict()->v,
			HUD_PRINTCONSOLE,
			"Convars populated from current client weapon %s (%d).\n",
			weapon->pszName(),
			weapon->m_iId);
	}

	void AddCommands(CClientCommandRegister& cmdRegister)
	{
		cmdRegister.AddCommand("sv_weapon_debug_inac_populate", &HandleSetInaccuracyDebugVarsFromCurrentWeapon);
	}
}  // namespace InaccuracyDebugging
