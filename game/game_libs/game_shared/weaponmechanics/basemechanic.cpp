#include "standard_includes.h"
#include "weaponmechanics/basemechanic.h"
#include "weaponattributes/weaponatts_baseattack.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"
#include "weapons/genericweapon.h"

namespace WeaponMechanics
{
	CBaseMechanic::CBaseMechanic(CGenericWeapon* weapon, const WeaponAtts::WABaseAttack* attackMode) :
		m_Weapon(weapon),
		m_BaseAttackMode(attackMode)
	{
		ASSERT(m_Weapon);
		ASSERT(m_BaseAttackMode);
	}

	void CBaseMechanic::Precache()
	{
		PrecacheSoundSet(m_BaseAttackMode->AttackSounds);
		PrecacheSoundSet(m_BaseAttackMode->ViewModelAttackSounds);

		if ( m_BaseAttackMode->OverrideAnimations )
		{
			PrecacheSoundSet(m_BaseAttackMode->OverrideAnimations->ReloadSounds);
		}

		m_EventIndex = PRECACHE_EVENT(1, m_BaseAttackMode->EventScript);
	}

	InvocationResult CBaseMechanic::Invoke(uint32_t)
	{
		if ( IsUnderwaterAndCannotFire() )
		{
			return InvocationResult::Rejected(*this, InvocationResult::REJECTED_CANNOT_ATTACK_UNDERWATER);
		}

		if ( !HasAmmo(1, m_Weapon->m_iClip >= 0) )
		{
			return InvocationResult::Rejected(*this, InvocationResult::REJECTED_NO_AMMO);
		}

		return InvocationResult::Complete(*this);
	}

	void CBaseMechanic::Reset()
	{
	}

	void CBaseMechanic::WeaponIdle()
	{
	}

	const WeaponAtts::WABaseAttack* CBaseMechanic::GetAttackMode() const
	{
		return m_BaseAttackMode;
	}

	const WeaponAtts::WAAmmoBasedAttack* CBaseMechanic::GetAmmoBasedAttackMode() const
	{
		return dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(m_BaseAttackMode);
	}

	int CBaseMechanic::GetEventIndex() const
	{
		return m_EventIndex;
	}

	int CBaseMechanic::DefaultEventFlags()
	{
#ifdef CLIENT_WEAPONS
		return FEV_NOTHOST;
#else
		return 0;
#endif
	}

	CGenericWeapon* CBaseMechanic::GetWeapon() const
	{
		return m_Weapon;
	}

	CBasePlayer* CBaseMechanic::GetPlayer() const
	{
		return m_Weapon->m_pPlayer;
	}

	WeaponAtts::AttackMode CBaseMechanic::GetAttackModeInWeapon() const
	{
		return m_Weapon->GetAttackModeForMechanic(this);
	}

	void CBaseMechanic::PrecacheSoundSet(const WeaponAtts::WASoundSet& sounds)
	{
		const WeightedValueList<const char*>& soundList = sounds.SoundNames;

		for ( uint32_t index = 0; index < soundList.Count(); ++index )
		{
			PRECACHE_SOUND(soundList.Value(index));
		}
	}

	bool CBaseMechanic::HasAmmo(int minCount, bool useClip) const
	{
		const WeaponAtts::WAAmmoBasedAttack* ammoAttack =
			dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(m_BaseAttackMode);

		if ( !ammoAttack )
		{
			// Treat as an infinite pool.
			return true;
		}

		return m_Weapon->HasAmmo(ammoAttack->UsesAmmoPool, minCount, useClip);
	}

	int CBaseMechanic::AmmoLeft(WeaponAtts::WAAmmoBasedAttack::AmmoPool pool) const
	{
		return m_Weapon->AmmoLeft(pool);
	}

	int CBaseMechanic::AmmoLeft(const WeaponAtts::WABaseAttack* attackMode) const
	{
		return m_Weapon->AmmoLeft(attackMode);
	}

	bool CBaseMechanic::IsUnderwaterAndCannotFire() const
	{
		return GetPlayer()->pev->waterlevel == 3 && !m_BaseAttackMode->FunctionsUnderwater;
	}

	void CBaseMechanic::PlaySound(const WeaponAtts::WASoundSet& sound, int channel, float volModifier)
	{
		if ( sound.SoundNames.Count() < 1 )
		{
			return;
		}

		CBasePlayer* player = GetPlayer();

		const float flRand = UTIL_SharedRandomFloat(player->random_seed, 0.0, 1.0);
		const char* soundName = sound.SoundNames.ItemByProbabilisticValue(flRand);
		const float volume = (sound.MinVolume < sound.MaxVolume)
			? UTIL_SharedRandomFloat(player->random_seed, sound.MinVolume, sound.MaxVolume)
			: sound.MaxVolume;
		const int pitch = (sound.MinPitch < sound.MaxPitch)
			? UTIL_SharedRandomLong(player->random_seed, sound.MinPitch, sound.MaxPitch)
			: sound.MaxPitch;

		EMIT_SOUND_DYN(
			ENT(player->pev),
			channel,
			soundName,
			volume * volModifier,
			sound.Attenuation,
			sound.Flags,
			pitch);
	}

	bool CBaseMechanic::DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool pool, int decrement)
	{
		return m_Weapon->DecrementAmmo(pool, decrement);
	}

	bool CBaseMechanic::DecrementAmmo(const WeaponAtts::WABaseAttack* attackMode)
	{
		const WeaponAtts::WAAmmoBasedAttack* ammoAttack =
			dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(attackMode);

		if ( !ammoAttack || ammoAttack->AmmoDecrement < 1 )
		{
			// Treat as an infinite pool.
			return true;
		}

		return DecrementAmmo(ammoAttack->UsesAmmoPool, ammoAttack->AmmoDecrement);
	}

	void CBaseMechanic::SetNextIdleTime(float secsInFuture, bool allowIfEarlier)
	{
		m_Weapon->SetNextIdleTime(secsInFuture, allowIfEarlier);
	}

	void CBaseMechanic::DelayFiring(float secs, bool allowIfEarlier, WeaponAtts::AttackMode attackMode)
	{
		m_Weapon->DelayFiring(secs, allowIfEarlier, attackMode);
	}
}  // namespace WeaponMechanics
