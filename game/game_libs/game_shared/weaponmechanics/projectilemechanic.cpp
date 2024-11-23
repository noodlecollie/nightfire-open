#include "weaponmechanics/projectilemechanic.h"
#include "weaponattributes/weaponatts_projectileattack.h"
#include "weapons.h"
#include "weapons/genericweapon.h"

namespace WeaponMechanics
{
	CProjectileMechanic::CProjectileMechanic(CGenericWeapon* weapon, const WeaponAtts::WAProjectileAttack* attackMode) :
		CBaseMechanic(weapon, attackMode)
	{
	}

	CProjectileMechanic::CProjectileMechanic(CGenericWeapon* weapon, uint32_t attackIndex) :
		CBaseMechanic(weapon, weapon->GetAttackModeFromAttributes<WeaponAtts::WAProjectileAttack>(attackIndex))
	{
	}

	void CProjectileMechanic::Precache()
	{
		CBaseMechanic::Precache();

		const WeaponAtts::WAProjectileAttack* attackMode = ProjectileAttackMode();

		if ( attackMode->ProjectileModelName )
		{
			PRECACHE_MODEL(attackMode->ProjectileModelName);
		}
	}

	InvocationResult CProjectileMechanic::Invoke(uint32_t)
	{
		// TODO
		return InvocationResult::Rejected(*this);
	}

	const WeaponAtts::WAProjectileAttack* CProjectileMechanic::ProjectileAttackMode() const
	{
		return GetAttackMode<WeaponAtts::WAProjectileAttack>();
	}
}  // namespace WeaponMechanics
