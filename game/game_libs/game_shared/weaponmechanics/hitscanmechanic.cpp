#include "standard_includes.h"
#include "weaponmechanics/hitscanmechanic.h"
#include "weaponattributes/weaponatts_hitscanattack.h"
#include "gameplay/hitscanaction.h"
#include "weapons/genericweapon.h"
#include "eventConstructor/eventConstructor.h"

namespace WeaponMechanics
{
	CHitscanMechanic::CHitscanMechanic(CGenericWeapon* weapon, const WeaponAtts::WAHitscanAttack* attackMode) :
		CBaseMechanic(weapon, attackMode)
	{
	}

	CHitscanMechanic::CHitscanMechanic(CGenericWeapon* weapon, uint32_t attackIndex) :
		CBaseMechanic(weapon, weapon->GetAttackModeFromAttributes<WeaponAtts::WAHitscanAttack>(attackIndex))
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

	InvocationResult CHitscanMechanic::Invoke(uint32_t step)
	{
		const WeaponAtts::WAHitscanAttack* hitscanAttack = HitscanAttackMode();

		if ( hitscanAttack->AttackRate <= 0.0f || hitscanAttack->BulletsPerShot < 1 )
		{
			return InvocationResult::Rejected(*this);
		}

		{
			const InvocationResult result = CBaseMechanic::Invoke(step);

			if ( result.result > InvocationResult::INCOMPLETE )
			{
				return result;
			}
		}

		CBasePlayer* player = GetPlayer();
		CGenericWeapon* weapon = GetWeapon();

		DecrementAmmo(hitscanAttack);

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
			// NFTODO: Use pev->v_angles and make sure that this works as expected.
			// We should not be relying on the global vectors here!!
			// Do this for other weapon mechanics as well.
			vecAiming = gpGlobals->v_forward;
		}

		CHitscanAction hitscanAction;

		hitscanAction.SetGunPos(vecSrc);
		hitscanAction.SetShootDir(vecAiming);
		hitscanAction.SetRightDir(gpGlobals->v_right);
		hitscanAction.SetUpDir(gpGlobals->v_up);
		hitscanAction.SetAttacker(player->pev);
		hitscanAction.SetInflictor(weapon->pev);
		hitscanAction.SetRandomSeed(player->random_seed);
		hitscanAction.SetWeapon(weapon);
		hitscanAction.SetBulletsPerShot(hitscanAttack->BulletsPerShot);
		hitscanAction.SetBaseDamagePerShot(hitscanAttack->BaseDamagePerShot);
		hitscanAction.SetSpreadPattern(hitscanAttack->Accuracy.FireSpreadPattern);
		hitscanAction.SetSpread(hitscanAttack->Accuracy, weapon->GetInaccuracy());
		hitscanAction.SetSendTracerMessage(false);

		hitscanAction.FireBullets();

		if ( GetEventIndex() >= 0 )
		{
			using namespace EventConstructor;
			CEventConstructor event;

			event << Flags(DefaultEventFlags()) << Invoker(player->edict())
				  << EventIndex(static_cast<unsigned short>(GetEventIndex())) << IntParam1(player->random_seed)
				  << BoolParam1(weapon->m_iClip == 0) << FloatParam1(weapon->GetInaccuracy());

			event.Send();
		}

		DelayFiring(1.0f / hitscanAttack->AttackRate);

		if ( !HasAmmo(1, true) && !HasAmmo(1, false) )
		{
			// HEV suit - indicate out of ammo condition
			player->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
		}

		// NFTODO: Is there a better value than just 5 here?
		SetNextIdleTime(5, true);

		return InvocationResult::Complete(*this);
	}

	void CHitscanMechanic::WeaponIdle()
	{
		if ( GetAttackModeInWeapon() == WeaponAtts::AttackMode::Primary )
		{
			GetPlayer()->GetAutoaimVector(GetAttackMode<WeaponAtts::WAHitscanAttack>()->AutoAim);
		}

		CBaseMechanic::WeaponIdle();
	}

	const WeaponAtts::WAHitscanAttack* CHitscanMechanic::HitscanAttackMode() const
	{
		return GetAttackMode<WeaponAtts::WAHitscanAttack>();
	}
}  // namespace WeaponMechanics
