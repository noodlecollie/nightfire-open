#include "weapon_ronin.h"
#include "ammodefs.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_ronin_atts.h"

#ifndef CLIENT_DLL
#include <limits>
#include <algorithm>
#include "bot.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_ronin, CWeaponRonin);

CWeaponRonin::CWeaponRonin() :
	CBaseGrenadeLauncher()
{
	// SetPrimaryAttackModeFromAttributes(ATTACKMODE_CONTACT);
	// SetSecondaryAttackModeFromAttributes(ATTACKMODE_TIMED);
}

const WeaponAtts::WACollection& CWeaponRonin::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponRonin>();
}

#ifndef CLIENT_DLL
float CWeaponRonin::Bot_CalcDesireToUse(CBaseBot&, CBaseEntity&, float) const
{
	// TODO
	return 0.0f;
}

void CWeaponRonin::Bot_SetFightStyle(CBaseBotFightStyle&) const
{
	// TODO
}

void CWeaponRonin::CreateProjectile(const WeaponAtts::WAProjectileAttack& projectileAttack)
{
	// TODO
	(void)projectileAttack;
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponRonin>()
	{
		return ::StaticWeaponAttributes;
	}
}  // namespace WeaponAtts

class CAmmoRonin : public CGenericAmmo
{
public:
	CAmmoRonin() :
		CGenericAmmo("models/weapon_ronin/w_ammo_ronin.mdl", Ammo_Ronin)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_ronin, CAmmoRonin)
