#include "standard_includes.h"
#include "weaponmechanics/hitscanmechanic.h"
#include "weaponattributes/weaponatts_hitscanattack.h"
#include "gameplay/hitscancomponent.h"
#include "weapons/genericweapon.h"
#include "eventConstructor/eventConstructor.h"

namespace WeaponMechanics
{
	CHitscanMechanic::CHitscanMechanic(CGenericWeapon* weapon, const WeaponAtts::WAHitscanAttack* attackMode) :
		CBaseMechanic(weapon, attackMode)
	{
	}

	void CHitscanMechanic::Precache()
	{
		CBaseMechanic::Precache();

		const WeaponAtts::WAHitscanAttack* attackMode = HitscanAttackMode();

		if ( attackMode->ShellModelName )
		{
			PRECACHE_MODEL(attackMode->ShellModelName);
		}
	}

	InvocationResult CHitscanMechanic::Invoke()
	{
		const WeaponAtts::WAHitscanAttack* hitscanAttack = HitscanAttackMode();

		if ( hitscanAttack->AttackRate <= 0.0f || hitscanAttack->BulletsPerShot < 1 )
		{
			return InvocationResult::Rejected();
		}

		CBasePlayer* player = GetPlayer();
		CGenericWeapon* weapon = GetWeapon();

		if ( player->pev->waterlevel == 3 && !hitscanAttack->FunctionsUnderwater )
		{
			return InvocationResult::Rejected(InvocationResult::REJECTED_CANNOT_ATTACK_UNDERWATER);
		}

		if ( !HasAmmo(1, weapon->m_iClip >= 0) )
		{
			return InvocationResult::Rejected(InvocationResult::REJECTED_NO_AMMO);
		}

		weapon->DecrementAmmo(hitscanAttack);

		player->pev->effects |= EF_MUZZLEFLASH;

		// player "shoot" animation
		player->SetAnimation(PLAYER_ATTACK1);

		player->m_iWeaponVolume = hitscanAttack->Volume;
		player->m_iWeaponFlash = hitscanAttack->MuzzleFlashBrightness;

		Vector vecSrc = player->GetGunPosition();
		Vector vecAiming;

		if ( hitscanAttack->AutoAim > 0.0f )
		{
			vecAiming = player->GetAutoaimVector(hitscanAttack->AutoAim);
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
		hitscanComponent.SetAttacker(player->pev);
		hitscanComponent.SetInflictor(weapon->pev);
		hitscanComponent.SetRandomSeed(player->random_seed);
		hitscanComponent.SetWeapon(weapon);
		hitscanComponent.SetBulletsPerShot(hitscanAttack->BulletsPerShot);
		hitscanComponent.SetBaseDamagePerShot(hitscanAttack->BaseDamagePerShot);
		hitscanComponent.SetSpreadPattern(hitscanAttack->Accuracy.FireSpreadPattern);
		hitscanComponent.SetSpread(hitscanAttack->Accuracy, weapon->GetInaccuracy());
		hitscanComponent.SetSendTracerMessage(false);

		hitscanComponent.FireBullets();

		if ( GetEventIndex() >= 0 )
		{
			using namespace EventConstructor;
			CEventConstructor event;

			event << Flags(DefaultEventFlags()) << Invoker(player->edict())
				  << EventIndex(static_cast<unsigned short>(GetEventIndex())) << IntParam1(player->random_seed)
				  << BoolParam1(weapon->m_iClip == 0) << FloatParam1(weapon->GetInaccuracy());

			event.Send();
		}

		weapon->DelayFiring(1.0f / hitscanAttack->AttackRate);

		if ( !HasAmmo(1, true) && !HasAmmo(1, false) )
		{
			// HEV suit - indicate out of ammo condition
			player->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
		}

		// NFTODO: Is there a better value than just 5 here?
		weapon->SetNextIdleTime(5, true);

		return InvocationResult::Complete();
	}
}  // namespace WeaponMechanics
