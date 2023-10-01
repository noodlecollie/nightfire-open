#pragma once

#include "standard_includes.h"
#include "generichitscanweapon.h"

class CWeaponMP9 : public CGenericHitscanWeapon
{
public:
	CWeaponMP9();
	const WeaponAtts::WACollection& WeaponAttributes() const override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponMP9>();
}
