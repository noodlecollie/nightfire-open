#pragma once

#include "standard_includes.h"
#include "weapons/genericweapon.h"
#include "weaponmechanics/meleemechanic.h"

class CWeaponFists : public CGenericWeapon
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
	void AttackInvoked(WeaponAtts::AttackMode mode, const WeaponMechanics::InvocationResult& result) override;

private:
	WeaponMechanics::CMeleeMechanic* m_PunchAttack = nullptr;
	WeaponMechanics::CMeleeMechanic* m_PunchComboAttack = nullptr;
	WeaponMechanics::CMeleeMechanic* m_KarateChopAttack = nullptr;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFists>();
}
