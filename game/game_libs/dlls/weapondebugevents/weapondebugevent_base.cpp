#include "weapondebugevent_base.h"
#include "genericweapon.h"

CWeaponDebugEvent_Base::CWeaponDebugEvent_Base(CGenericWeapon& weapon) :
	m_pWeapon(&weapon)
{
}

CGenericWeapon* CWeaponDebugEvent_Base::Weapon() const
{
	return m_pWeapon;
}

CBasePlayer* CWeaponDebugEvent_Base::WeaponOwner() const
{
	return m_pWeapon->m_pPlayer;
}
