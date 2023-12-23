#include "genericprojectileweapon.h"
#include "soundent.h"
#include "weaponatts_projectileattack.h"
#include "eventConstructor/eventConstructor.h"
#include "EnginePublicAPI/weaponinfo.h"

void CGenericProjectileWeapon::Precache()
{
	CGenericWeapon::Precache();
}

void CGenericProjectileWeapon::WeaponTick()
{
	if ( m_EnqueuedProjectileAttack && pev->fuser1 <= UTIL_WeaponTimeBase() )
	{
		CreateProjectileAndDecrementAmmo(*m_EnqueuedProjectileAttack);
		m_EnqueuedProjectileAttack = nullptr;
	}
}

bool CGenericProjectileWeapon::InvokeWithAttackMode(WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode)
{
	if ( !attackMode || attackMode->Classify() != WeaponAtts::WABaseAttack::Classification::Projectile )
	{
		return false;
	}

	const WeaponAtts::WAProjectileAttack* projectileAttack =
		static_cast<const WeaponAtts::WAProjectileAttack*>(attackMode);

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
	m_pPlayer->m_flStopExtraSoundTime = UTIL_WeaponTimeBase() + 0.2f;

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	const bool willBeEmpty = AmmoDecrementWillEmptyWeaponClip(attackMode, 1);

	if ( projectileAttack->projectileDelay <= 0.0f )
	{
		CreateProjectileAndDecrementAmmo(*projectileAttack);
	}
	else
	{
		m_EnqueuedProjectileAttack = projectileAttack;
		pev->fuser1 = UTIL_WeaponTimeBase() + projectileAttack->projectileDelay;
	}

	const int eventID = GetEventIDForAttackMode(projectileAttack);

	if ( eventID >= 0 )
	{
		using namespace EventConstructor;

		CEventConstructor event;
		event << Flags(DefaultEventFlags()) << Invoker(m_pPlayer->edict())
			  << EventIndex(static_cast<unsigned short>(eventID)) << IntParam1(m_pPlayer->random_seed)
			  << BoolParam1(willBeEmpty) << FloatParam1(GetInaccuracy());

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

bool CGenericProjectileWeapon::AmmoDecrementWillEmptyWeaponClip(
	const WeaponAtts::WABaseAttack* attackMode,
	int decrement) const
{
	const int ammoLeft = AmmoLeft(attackMode);

	if ( ammoLeft < 0 )
	{
		return false;
	}

	return decrement >= ammoLeft;
}

void CGenericProjectileWeapon::CreateProjectileAndDecrementAmmo(const WeaponAtts::WAProjectileAttack& projectileAttack)
{
	DecrementAmmo(&projectileAttack, 1);

#ifndef CLIENT_DLL
	// Only create the actual projectile on the server.
	CreateProjectile(projectileAttack);
#endif
}

void CGenericProjectileWeapon::Holster(int skiplocal)
{
	CGenericWeapon::Holster(skiplocal);

	m_EnqueuedProjectileAttack = nullptr;
}

bool CGenericProjectileWeapon::ReadPredictionData(const weapon_data_t* from)
{
	if ( !CGenericWeapon::ReadPredictionData(from) )
	{
		return false;
	}

	pev->fuser1 = from->fuser1;
	return true;
}

bool CGenericProjectileWeapon::WritePredictionData(weapon_data_t* to)
{
	if ( !CGenericWeapon::WritePredictionData(to) )
	{
		return false;
	}

	to->fuser1 = pev->fuser1;
	return true;
}
