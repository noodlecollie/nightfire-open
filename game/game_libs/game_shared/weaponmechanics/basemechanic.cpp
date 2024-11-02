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

	InvocationResult CBaseMechanic::Invoke()
	{
		return InvocationResult::Rejected();
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
}  // namespace WeaponMechanics
