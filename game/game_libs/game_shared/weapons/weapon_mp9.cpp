#include "weapon_mp9.h"
#include "weaponregistry.h"
#include "EnginePublicAPI/weaponinfo.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_mp9_atts.h"

#ifndef CLIENT_DLL
#include "bot.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_mp9, CWeaponMP9)

CWeaponMP9::CWeaponMP9() :
	CGenericWeapon()
{
	AddMechanicByAttributeIndex<WeaponAtts::WAHitscanAttack>(MP9_ATTACKMODE_NORMAL, m_PrimaryMechanic);
	SetPrimaryAttackMechanic(m_PrimaryMechanic);
}

const WeaponAtts::WACollection& CWeaponMP9::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponMP9>();
}

#ifndef CLIENT_DLL
float CWeaponMP9::Bot_CalcDesireToUse(CBaseBot&, CBaseEntity&, float) const
{
	return static_cast<float>(WeaponAttributes().Core.SwitchWeight) / static_cast<float>(WeaponPref_Max);
}

void CWeaponMP9::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
	// NFTODO: Tune this properly
	fightStyle.SetSecondaryFire(false);
	fightStyle.RandomizeAimAtHead(50);
	fightStyle.SetHoldDownAttack(true);
	fightStyle.SetNextShootTime(m_flNextPrimaryAttack, 0.1f, 0.2f);
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponMP9>()
	{
		return ::StaticWeaponAttributes;
	}
}  // namespace WeaponAtts

class CAmmoMP9 : public CGenericAmmo
{
public:
	CAmmoMP9() :
		CGenericAmmo("models/weapon_mp9/w_ammo_mp9.mdl", Ammo_MP9)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_mp9, CAmmoMP9)
