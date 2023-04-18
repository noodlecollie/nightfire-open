#pragma once

#include "standard_includes.h"
#include "genericmeleeweapon.h"

class CWeaponFists : public CGenericMeleeWeapon
{
public:
	CWeaponFists();
	virtual const WeaponAtts::WACollection& WeaponAttributes() const override;

#ifndef CLIENT_DLL
	virtual float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	virtual void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif

protected:
	virtual bool InvokeWithAttackMode(WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode) override;

private:
	const WeaponAtts::WAMeleeAttack* m_pPunchAttack;
	const WeaponAtts::WAMeleeAttack* m_pPunchComboAttack;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFists>();
}
