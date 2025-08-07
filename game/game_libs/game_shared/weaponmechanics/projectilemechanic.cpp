#include "weaponmechanics/projectilemechanic.h"
#include "weaponattributes/weaponatts_projectileattack.h"
#include "weapons.h"
#include "soundent.h"
#include "weapons/genericweapon.h"
#include "eventConstructor/eventConstructor.h"

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

	const WeaponAtts::WAProjectileAttack* CProjectileMechanic::ProjectileAttackMode() const
	{
		return GetAttackMode<WeaponAtts::WAProjectileAttack>();
	}

	void CProjectileMechanic::SetCreateProjectileCallback(CreateProjectileCallback callback)
	{
		m_CreateProjectileCallback = std::move(callback);
	}

	Vector CProjectileMechanic::GetProjectileLaunchAngles(float extraPitch) const
	{
		CBasePlayer* player = GetPlayer();

		Vector viewAngles = Vector(player->pev->v_angle) + Vector(player->pev->punchangle);

		if ( extraPitch != 0.0f )
		{
			// Add some more pitch depending on the cosine of the original pitch.
			// If the player is looking horizontally, we want the grenade to be
			// launched a little more upward; if they are looking straight up or
			// down, we don't want any modification at all.
			viewAngles[0] -= cosf(UTIL_DegreesToRadians(viewAngles[0])) * extraPitch;
		}

		if ( viewAngles[0] < -89.0f )
		{
			viewAngles[0] = -89.0f;
		}
		else if ( viewAngles[0] > 89.0f )
		{
			viewAngles[0] = 89.0f;
		}

		return viewAngles;
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

	InvocationResult CProjectileMechanic::Invoke(uint32_t step)
	{
		const InvocationResult result = CBaseMechanic::Invoke(step);

		if ( result.WasRejected() )
		{
			return result;
		}

		// We expect only two steps.
		if ( step > 1 )
		{
			ASSERT(false);
			return InvocationResult::Rejected(*this);
		}

		if ( step == 0 )
		{
			InitialWeaponFire();
		}

		const WeaponAtts::WAProjectileAttack* attackMode = ProjectileAttackMode();

		if ( step == 0 && attackMode->ProjectileDelay > 0.0f )
		{
			// Defer to the next step.
			return InvocationResult::Incomplete(*this, attackMode->ProjectileDelay);
		}

		CreateProjectileAndDecrementAmmo();
		return InvocationResult::Complete(*this);
	}

	void CProjectileMechanic::InitialWeaponFire()
	{
		const WeaponAtts::WAProjectileAttack* attackMode = ProjectileAttackMode();
		CBasePlayer* player = GetPlayer();

		player->m_iWeaponVolume = attackMode->Volume;
		player->m_iWeaponFlash = attackMode->MuzzleFlashBrightness;

		player->m_iExtraSoundTypes = bits_SOUND_DANGER;
		player->m_flStopExtraSoundTime = UTIL_WeaponTimeBase() + 0.2f;

		player->SetAnimation(PLAYER_ATTACK1);

		FireEvent();
		DelayFiring(1.0f / attackMode->AttackRate);
		SetNextIdleTime(5, true);
	}

	void CProjectileMechanic::CreateProjectileAndDecrementAmmo()
	{
		// Need to check this, as it should only be set on the server.
		if ( m_CreateProjectileCallback && !m_CreateProjectileCallback(*this) )
		{
			return;
		}

		DecrementAmmo(GetAttackMode());

		if ( !HasAmmo(1, true) && !HasAmmo(1, false) )
		{
			// HEV suit - indicate out of ammo condition
			GetPlayer()->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
		}
	}

	void CProjectileMechanic::FireEvent()
	{
		using namespace EventConstructor;

		if ( GetEventIndex() < 0 )
		{
			return;
		}

		CBasePlayer* player = GetPlayer();
		CGenericWeapon* weapon = GetWeapon();
		const bool willBeEmpty = AmmoDecrementWillEmptyWeaponClip(1);

		CEventConstructor event;
		event << Flags(DefaultEventFlags()) << Invoker(player->edict())
			  << EventIndex(static_cast<unsigned short>(GetEventIndex())) << IntParam1(player->random_seed)
			  << BoolParam1(willBeEmpty) << FloatParam1(weapon->GetInaccuracy());

		event.Send();
	}

	bool CProjectileMechanic::AmmoDecrementWillEmptyWeaponClip(int decrement) const
	{
		const int ammoLeft = AmmoLeft(GetAttackMode());

		if ( ammoLeft < 0 )
		{
			return false;
		}

		return decrement >= ammoLeft;
	}
}  // namespace WeaponMechanics
