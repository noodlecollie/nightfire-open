#pragma once

#include "standard_includes.h"
#include "generichitscanweapon.h"

class CWeaponSilent : public CGenericHitscanWeapon
{
public:
	CWeaponSilent();
	const WeaponAtts::WACollection& WeaponAttributes() const override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponSilent>();
}
