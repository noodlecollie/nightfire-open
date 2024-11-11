#include <memory>
#include "generichitscanweapon.h"
#include "weaponatts_hitscanattack.h"
#include "skill.h"
#include "eventConstructor/eventConstructor.h"
#include "gameplay/inaccuracymodifiers.h"
#include "gameplay/spreadPatterns.h"
#include "gameplay/hitscancomponent.h"

void CGenericHitscanWeapon::WeaponIdle()
{
	const WeaponAtts::WAHitscanAttack* primaryAttack = GetPrimaryAttackMode<const WeaponAtts::WAHitscanAttack>();

	if ( primaryAttack )
	{
		m_pPlayer->GetAutoaimVector(primaryAttack->AutoAim);
	}

	CGenericWeapon::WeaponIdle();
}

void CGenericHitscanWeapon::PrecacheAttackMode(const WeaponAtts::WABaseAttack& attack)
{
	CGenericWeapon::PrecacheAttackMode(attack);

	if ( attack.Classify() != WeaponAtts::WABaseAttack::Classification::Hitscan )
	{
		ASSERT(false);
		return;
	}

	const WeaponAtts::WAHitscanAttack& hitscanAttack = static_cast<const WeaponAtts::WAHitscanAttack&>(attack);

	if ( hitscanAttack.ShellModelName )
	{
		PRECACHE_MODEL(hitscanAttack.ShellModelName);
	}
}

bool CGenericHitscanWeapon::InvokeWithAttackMode(WeaponAtts::AttackMode mode, const WeaponAtts::WABaseAttack* attack)
{
	if ( !attack || attack->Classify() != WeaponAtts::WABaseAttack::Classification::Hitscan )
	{
		return false;
	}

	const WeaponAtts::WAHitscanAttack* hitscanAttack = static_cast<const WeaponAtts::WAHitscanAttack*>(attack);

	if ( hitscanAttack->AttackRate <= 0.0f || hitscanAttack->BulletsPerShot < 1 )
	{
		return false;
	}

	// Check base class allows the attack:
	if ( !CGenericWeapon::InvokeWithAttackMode(mode, hitscanAttack) )
	{
		return false;
	}

	DecrementAmmo(hitscanAttack);

	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_pPlayer->m_iWeaponVolume = hitscanAttack->Volume;
	m_pPlayer->m_iWeaponFlash = hitscanAttack->MuzzleFlashBrightness;

	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecAiming;

	if ( hitscanAttack->AutoAim > 0.0f )
	{
		vecAiming = m_pPlayer->GetAutoaimVector(hitscanAttack->AutoAim);
	}
	else
	{
		vecAiming = gpGlobals->v_forward;
	}

	CHitscanComponent hitscanComponent;

	hitscanComponent.SetGunPos(vecSrc);
	hitscanComponent.SetShootDir(vecAiming);
	hitscanComponent.SetRightDir(gpGlobals->v_right);
	hitscanComponent.SetUpDir(gpGlobals->v_up);
	hitscanComponent.SetAttacker(m_pPlayer->pev);
	hitscanComponent.SetInflictor(pev);
	hitscanComponent.SetRandomSeed(m_pPlayer->random_seed);
	hitscanComponent.SetWeapon(this);
	hitscanComponent.SetBulletsPerShot(hitscanAttack->BulletsPerShot);
	hitscanComponent.SetBaseDamagePerShot(hitscanAttack->BaseDamagePerShot);
	hitscanComponent.SetSpreadPattern(hitscanAttack->Accuracy.FireSpreadPattern);
	hitscanComponent.SetSpread(hitscanAttack->Accuracy, GetInaccuracy());
	hitscanComponent.SetSendTracerMessage(false);

	hitscanComponent.FireBullets();

	const int eventID = GetEventIDForAttackMode(hitscanAttack);

	if ( eventID >= 0 )
	{
		using namespace EventConstructor;
		CEventConstructor event;

		event << Flags(DefaultEventFlags()) << Invoker(m_pPlayer->edict())
			  << EventIndex(static_cast<unsigned short>(eventID)) << IntParam1(m_pPlayer->random_seed)
			  << BoolParam1(m_iClip == 0) << FloatParam1(GetInaccuracy());

		event.Send();
	}

	DelayFiring(1.0f / hitscanAttack->AttackRate);

	if ( !HasAmmo(hitscanAttack, 1, true) && !HasAmmo(hitscanAttack, 1, false) )
	{
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}

	SetNextIdleTime(5, true);
	return true;
}
