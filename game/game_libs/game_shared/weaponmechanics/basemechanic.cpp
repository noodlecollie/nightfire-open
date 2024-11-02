#include "standard_includes.h"
#include "weaponmechanics/basemechanic.h"
#include "weaponattributes/weaponatts_baseattack.h"

namespace WeaponMechanics
{
	CBaseMechanic::CBaseMechanic(const WeaponAtts::WABaseAttack* attackMode) :
		m_BaseAttackMode(attackMode)
	{
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

	int CBaseMechanic::EventIndex() const
	{
		return m_EventIndex;
	}

	void CBaseMechanic::PrecacheSoundSet(const WeaponAtts::WASoundSet& sounds)
	{
		const WeightedValueList<const char*>& soundList = sounds.SoundNames;

		for ( uint32_t index = 0; index < soundList.Count(); ++index )
		{
			PRECACHE_SOUND(soundList.Value(index));
		}
	}
}
