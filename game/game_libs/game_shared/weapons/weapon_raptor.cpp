#include "weapon_raptor.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_raptor_atts.h"

#ifndef CLIENT_DLL
#include "bot.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_raptor, CWeaponRaptor)

#ifdef NFOPEN_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known weapons to this one.
LINK_ENTITY_TO_CLASS(weapon_pdw90, CWeaponRaptor)
#endif

CWeaponRaptor::CWeaponRaptor() :
	CGenericWeapon()
{
	AddMechanicByAttributeIndex<WeaponAtts::WAHitscanAttack>(ATTACKMODE_NORMAL, m_PrimaryMechanic);
	SetPrimaryAttackMechanic(m_PrimaryMechanic);
}

const WeaponAtts::WACollection& CWeaponRaptor::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponRaptor>();
}

#ifndef CLIENT_DLL
float CWeaponRaptor::Bot_CalcDesireToUse(CBaseBot&, CBaseEntity&, float) const
{
	return static_cast<float>(WeaponAttributes().Core.SwitchWeight) / static_cast<float>(WeaponPref_Max);
}

void CWeaponRaptor::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
	fightStyle.SetSecondaryFire(false);
	fightStyle.RandomizeAimAtHead(80);
	fightStyle.SetNextShootTime(m_flNextPrimaryAttack, 0.0f, 0.5f);
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponRaptor>()
	{
		return ::StaticWeaponAttributes;
	}
}  // namespace WeaponAtts

class CAmmoRaptor : public CGenericAmmo
{
public:
	CAmmoRaptor() :
		CGenericAmmo("models/weapon_raptor/w_ammo_raptor.mdl", Ammo_Raptor)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_raptor, CAmmoRaptor)

#ifdef NFOPEN_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known ammo to this one.
LINK_ENTITY_TO_CLASS(ammo_pdw90, CAmmoRaptor)
#endif
