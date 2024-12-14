#pragma once

#include <functional>
#include "weaponmechanics/basemechanic.h"

namespace WeaponAtts
{
	struct WAProjectileAttack;
};

namespace WeaponMechanics
{
	class CProjectileMechanic : public CBaseMechanic
	{
	public:
		using CreateProjectileCallback = std::function<bool(CProjectileMechanic&)>;

		CProjectileMechanic(CGenericWeapon* weapon, const WeaponAtts::WAProjectileAttack* attackMode);
		CProjectileMechanic(CGenericWeapon* weapon, uint32_t attackIndex);

		const WeaponAtts::WAProjectileAttack* ProjectileAttackMode() const;
		void SetCreateProjectileCallback(CreateProjectileCallback callback);

		Vector GetProjectileLaunchAngles(float extraPitch) const;

		void Precache() override;
		InvocationResult Invoke(uint32_t step) override;

	private:
		void InitialWeaponFire();
		void CreateProjectileAndDecrementAmmo();
		void FireEvent();
		bool AmmoDecrementWillEmptyWeaponClip(int decrement) const;

		CreateProjectileCallback m_CreateProjectileCallback;
	};
}  // namespace WeaponMechanics
