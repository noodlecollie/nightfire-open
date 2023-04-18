#include "genericprojectileweapon.h"
#include "soundent.h"
#include "weaponatts_projectileattack.h"
#include "eventConstructor/eventConstructor.h"

void CGenericProjectileWeapon::Precache()
{
	CGenericWeapon::Precache();
}

bool CGenericProjectileWeapon::InvokeWithAttackMode(WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode)
{
	if ( !attackMode || attackMode->Classify() != WeaponAtts::WABaseAttack::Classification::Projectile )
	{
		return false;
	}

	const WeaponAtts::WAProjectileAttack* projectileAttack = static_cast<const WeaponAtts::WAProjectileAttack*>(attackMode);

	if ( projectileAttack->AttackRate <= 0.0f )
	{
		return false;
	}

	if ( !CGenericWeapon::InvokeWithAttackMode(type, projectileAttack) )
	{
		return false;
	}

	m_pPlayer->m_iWeaponVolume = projectileAttack->Volume;
	m_pPlayer->m_iWeaponFlash = projectileAttack->MuzzleFlashBrightness;

	m_pPlayer->m_iExtraSoundTypes = bits_SOUND_DANGER;
	m_pPlayer->m_flStopExtraSoundTime = UTIL_WeaponTimeBase() + 0.2;

	DecrementAmmo(projectileAttack, 1);

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

#ifndef CLIENT_DLL
	// Only create the actual projectile on the server.
	CreateProjectile(*projectileAttack);
#endif

	const int eventID = GetEventIDForAttackMode(projectileAttack);

	if ( eventID >= 0 )
	{
		using namespace EventConstructor;

		CEventConstructor event;
		event.InitSimple(DefaultEventFlags(), m_pPlayer->edict(), eventID);

		event.Send();
	}

	DelayFiring(1.0f / projectileAttack->AttackRate);
	SetNextIdleTime(5, true);

	if ( !HasAmmo(projectileAttack, 1, true) && !HasAmmo(projectileAttack, 1, false) )
	{
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}

	return true;
}
