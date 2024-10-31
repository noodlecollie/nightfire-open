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
#include "npc/npc_ronin_turret.h"
#include "MathLib/utils.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_ronin, CWeaponRonin);

CWeaponRonin::CWeaponRonin() :
	CBaseGrenadeLauncher()
{
	// TODO: Enum for index
	SetPrimaryAttackModeFromAttributes(0);
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
	(void)projectileAttack;

	CNPCRoninTurret* turret = GetClassPtr<CNPCRoninTurret>(nullptr);
	turret->pev->owner = m_pPlayer->edict();
	turret->Spawn();

	Vector forward;
	AngleVectors(GetGrenadeLaunchAngles(0.0f), forward, nullptr, nullptr);

	// TODO: This is very rough and it's easy to get the Ronin stuck in-game.
	// Should be replaced with more robust checks once we actually allow
	// projectile motion.
	const Vector traceBegin = Vector(m_pPlayer->pev->origin) + Vector(m_pPlayer->pev->view_ofs);
	const Vector traceEnd = traceBegin + (forward * 64.0f);

	TraceResult tr {};
	UTIL_TraceLine(traceBegin, traceEnd, dont_ignore_monsters, m_pPlayer->edict(), &tr);

	Vector location = tr.flFraction < 1.0f ? Vector(tr.vecEndPos) : traceEnd;
	location -= forward * 32.0f;

	Vector velocity = (200.0f * forward) + (150.0f * Vector(0, 0, 1));

	const float rotDir = RANDOM_LONG(0, 1) ? 1.0f : -1.0f;
	Vector avelocity(0, RANDOM_FLOAT(360.0f * rotDir, 540.0f * rotDir), 0);

	turret->StartToss(location, velocity, avelocity);
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
