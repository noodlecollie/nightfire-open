#pragma once

#include "standard_includes.h"
#include "genericweapon.h"
#include "weaponmechanics/delegatedmechanic.h"

class CWeaponProjectileTrace : public CGenericWeapon
{
public:
	CWeaponProjectileTrace();
	const WeaponAtts::WACollection& WeaponAttributes() const override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif

private:
	WeaponMechanics::InvocationResult DoTrace(WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step);

#ifndef CLIENT_DLL
	void DoTrace_Server();
#endif

	WeaponMechanics::CDelegatedMechanic* m_PrimaryMechanic = nullptr;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponProjectileTrace>();
}
