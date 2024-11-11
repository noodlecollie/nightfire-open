#pragma once

#include "standard_includes.h"
#include "genericmeleeweapon.h"

class CWeaponFists : public CGenericMeleeWeapon
{
public:
	CWeaponFists();
	const WeaponAtts::WACollection& WeaponAttributes() const override;

	BOOL Deploy() override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif

protected:
	bool InvokeWithAttackMode(WeaponAtts::AttackMode mode, const WeaponAtts::WABaseAttack* attack) override;

private:
	const WeaponAtts::WAMeleeAttack* m_pPunchAttack;
	const WeaponAtts::WAMeleeAttack* m_pPunchComboAttack;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFists>();
}
