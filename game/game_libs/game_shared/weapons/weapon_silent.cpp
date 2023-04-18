#include "weapon_silent.h"
#include "weaponregistry.h"
#include "weaponinfo.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_silent_atts.h"

#ifndef CLIENT_DLL
#include "bot.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_silent, CWeaponSilent)

CWeaponSilent::CWeaponSilent()
	: CGenericHitscanWeapon()
{
	SetPrimaryAttackModeFromAttributes(ATTACKMODE_NORMAL);
}

const WeaponAtts::WACollection& CWeaponSilent::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponSilent>();
}

#ifndef CLIENT_DLL
float CWeaponSilent::Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const
{
	return static_cast<float>(WeaponAttributes().Core.SwitchWeight) / static_cast<float>(WeaponPref_Max);
}

void CWeaponSilent::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponSilent>()
	{
		return ::StaticWeaponAttributes;
	}
}
