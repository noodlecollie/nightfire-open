#include "weapon_fists.h"
#include "weapon_pref_weights.h"
#include "weapon_fists_atts.h"

#ifndef CLIENT_DLL
#include "bot.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_fists, CWeaponFists);
LINK_ENTITY_TO_CLASS(weapon_dukes, CWeaponFists);	// For NF compatibility

CWeaponFists::CWeaponFists() : CGenericMeleeWeapon()
{
	m_pPunchAttack = GetAttackModeFromAttributes<WeaponAtts::WAMeleeAttack>(ATTACKMODE_PUNCH);
	m_pPunchComboAttack = GetAttackModeFromAttributes<WeaponAtts::WAMeleeAttack>(ATTACKMODE_PUNCH_COMBO);

	SetPrimaryAttackMode(m_pPunchAttack);
	SetSecondaryAttackModeFromAttributes(ATTACKMODE_KARATE_CHOP);
}

const WeaponAtts::WACollection& CWeaponFists::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponFists>();
}

bool CWeaponFists::InvokeWithAttackMode(WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode)
{
	if ( !CGenericMeleeWeapon::InvokeWithAttackMode(type, attackMode) )
	{
		return false;
	}

	if ( type == WeaponAttackType::Primary )
	{
		// Alternate between modes.
		const WeaponAtts::WAMeleeAttack* currentMode = GetPrimaryAttackMode<WeaponAtts::WAMeleeAttack>();
		const WeaponAtts::WAMeleeAttack* newMode = (currentMode == m_pPunchAttack ? m_pPunchComboAttack : m_pPunchAttack);
		SetPrimaryAttackMode(newMode);
	}

	return true;
}

#ifndef CLIENT_DLL
float CWeaponFists::Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const
{
	return static_cast<float>(WeaponAttributes().Core.SwitchWeight) / static_cast<float>(WeaponPref_Max);
}

void CWeaponFists::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
	fightStyle.SetSecondaryFire(false);
	fightStyle.SetHoldDownAttack(true);
	fightStyle.RandomizeAimAtHead(40);
	fightStyle.SetNextShootTime(m_flNextPrimaryAttack, 0.2f, 0.7f);
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFists>()
	{
		return ::StaticWeaponAttributes;
	}
}
