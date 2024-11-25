#include "weaponmechanics/delegatedmechanic.h"
#include "weaponattributes/weaponatts_eventattack.h"
#include "genericweapon.h"

namespace WeaponMechanics
{
	CDelegatedMechanic::CDelegatedMechanic(CGenericWeapon* weapon, const WeaponAtts::WAEventAttack* attackMode) :
		CBaseMechanic(weapon, attackMode)
	{
	}

	CDelegatedMechanic::CDelegatedMechanic(CGenericWeapon* weapon, uint32_t attackIndex) :
		CBaseMechanic(weapon, weapon->GetAttackModeFromAttributes<WeaponAtts::WAEventAttack>(attackIndex))
	{
	}

	void CDelegatedMechanic::SetCallback(Callback callback)
	{
		m_Callback = callback;
	}

	InvocationResult CDelegatedMechanic::Invoke(uint32_t step)
	{
		if ( !m_Callback )
		{
			return InvocationResult::Rejected(*this);
		}

		return m_Callback(*this, step);
	}
}
