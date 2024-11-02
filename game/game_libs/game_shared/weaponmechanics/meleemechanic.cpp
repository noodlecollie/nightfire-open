#include "standard_includes.h"
#include "weaponmechanics/meleemechanic.h"
#include "weaponattributes/weaponatts_meleeattack.h"

namespace WeaponMechanics
{
	CMeleeMechanic::CMeleeMechanic(CGenericWeapon* weapon, const WeaponAtts::WAMeleeAttack* attackMode) :
		CBaseMechanic(weapon, attackMode)
	{
	}

	void CMeleeMechanic::Precache()
	{
		CBaseMechanic::Precache();

		const WeaponAtts::WAMeleeAttack* attackMode = MeleeAttackMode();

		PrecacheSoundSet(attackMode->BodyHitSounds);
		PrecacheSoundSet(attackMode->WorldHitSounds);
	}
}
