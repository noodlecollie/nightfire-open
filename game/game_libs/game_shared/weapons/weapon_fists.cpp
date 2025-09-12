#include "weapon_fists.h"
#include "weapon_pref_weights.h"
#include "weapon_fists_atts.h"

#ifndef CLIENT_DLL
#include "bot.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_fists, CWeaponFists);
LINK_ENTITY_TO_CLASS(weapon_dukes, CWeaponFists);  // For NF compatibility

CWeaponFists::CWeaponFists() :
	CGenericWeapon()
{
	AddMechanicByAttributeIndex<WeaponAtts::WAMeleeAttack>(ATTACKMODE_PUNCH, m_PunchAttack);
	AddMechanicByAttributeIndex<WeaponAtts::WAMeleeAttack>(ATTACKMODE_PUNCH_COMBO, m_PunchComboAttack);
	AddMechanicByAttributeIndex<WeaponAtts::WAMeleeAttack>(ATTACKMODE_KARATE_CHOP, m_KarateChopAttack);

	SetPrimaryAttackMechanic(m_PunchAttack);
	SetSecondaryAttackMechanic(m_KarateChopAttack);
}

const WeaponAtts::WACollection& CWeaponFists::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponFists>();
}

BOOL CWeaponFists::Deploy()
{
	if ( !CGenericWeapon::Deploy() )
	{
		return FALSE;
	}

	// Ensure we always start on the first attack.
	SetPrimaryAttackMechanic(m_PunchAttack);

	return TRUE;
}

void CWeaponFists::AttackInvoked(WeaponAtts::AttackMode, const WeaponMechanics::InvocationResult& result)
{
	if ( result.WasComplete() && result.mechanic == GetPrimaryAttackMechanic() )
	{
		WeaponMechanics::CMeleeMechanic* nextAttack =
			result.mechanic == m_PunchAttack ? m_PunchComboAttack : m_PunchAttack;

		SetPrimaryAttackMechanic(nextAttack);
	}
}

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFists>()
	{
		return ::StaticWeaponAttributes;
	}
}  // namespace WeaponAtts
