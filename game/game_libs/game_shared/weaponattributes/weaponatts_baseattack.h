#pragma once

#include "weaponatts_base.h"
#include "weightedvaluelist.h"
#include "weaponatts_soundset.h"
#include "weaponids.h"
#include "weapons.h"

namespace WeaponAtts
{
	struct WABaseAttack : public WABase
	{
		enum class Classification
		{
			Melee = 0,
			Hitscan,
			Projectile
		};

		struct AttackModeSignature
		{
			WeaponId_e WeaponId = WeaponId_e::WeaponNone;

			// Global index of this attack mode in the weapon.
			uint32_t Index;
		};

		const char* EventScript = nullptr;
		bool FunctionsUnderwater = false;
		bool IsContinuous = false;
		float AttackRate = 1.0f;	// Cycles per second

		WeightedValueList<int> ViewModelAnimList_Attack;
		WeightedValueList<int> ViewModelAnimList_AttackEmpty;
		WASoundSet AttackSounds;
		int Volume = NORMAL_GUN_VOLUME;

		// If specified, attack uses this body index for the view model.
		int ViewModelBodyOverride = -1;

		// If non-empty, this set of sounds will be played to the player holding the weapon.
		// The normal AttackSounds will be played to all other players.
		WASoundSet ViewModelAttackSounds;

		virtual Classification Classify() const = 0;

		virtual void Validate() const override
		{
			ASSERTSZ_Q(EventScript, "Event script must be specified.");
			ASSERTSZ_Q(AttackRate > 0.0f, "Attack rate must be positive.");
		}

		inline const AttackModeSignature* Signature() const
		{
			return &m_Signature;
		}

		inline void SetSignature(WeaponId_e id, uint32_t index)
		{
			m_Signature.WeaponId = id;
			m_Signature.Index = index;
		}

		inline bool HasViewModelAttackSounds() const
		{
			return ViewModelAttackSounds.SoundNames.Count() > 0;
		}

	private:
		mutable AttackModeSignature m_Signature;
	};
}
