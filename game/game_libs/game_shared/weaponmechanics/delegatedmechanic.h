#pragma once

#include <functional>
#include "weaponmechanics/basemechanic.h"

namespace WeaponAtts
{
	struct WAEventAttack;
};

namespace WeaponMechanics
{
	class CDelegatedMechanic : public CBaseMechanic
	{
	public:
		using Callback = std::function<InvocationResult(CDelegatedMechanic&, uint32_t)>;

		CDelegatedMechanic(CGenericWeapon* weapon, const WeaponAtts::WAEventAttack* attackMode);
		CDelegatedMechanic(CGenericWeapon* weapon, uint32_t attackIndex);

		void SetCallback(Callback callback);
		InvocationResult Invoke(uint32_t step) override;

	private:
		Callback m_Callback;
	};
}
