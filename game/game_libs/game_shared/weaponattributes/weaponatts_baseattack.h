#pragma once

#include <memory>
#include "weaponatts_base.h"
#include "weightedvaluelist.h"
#include "weaponatts_soundset.h"
#include "weaponatts_viewmodel.h"
#include "weaponids.h"
#include "weapons.h"

namespace WeaponAtts
{
	enum class CrosshairStyle
	{
		None = 0,
		QuadLine,
		Circle
	};

	enum class AttackMode
	{
		None = -1,
		Primary = 0,
		Secondary = 1
	};

	struct CrosshairParameters
	{
		// The type of crosshair to use.
		CrosshairStyle RenderStyle = CrosshairStyle::QuadLine;

		// To modify these settings while debugging/testing, see the list of convars
		// in game_libs/cl_dll/gameplay/crosshairCvars.cpp

		// The radius is how far away from the centre of the screen each
		// crosshair bar is. A value of 1 means the length of the shortest
		// screen dimension. Minimum radius is when the weapon is at
		// rest spread, and maximum radius is when it is at run spread.
		float RadiusMin = 0.0f;
		float RadiusMax = 0.5f;

		// These scales specify how long the crosshair bars are at minimum
		// and maximum inaccuracy. A value of 1 means the bar is the length
		// of the shortest screen dimension. Minimum scale is when the weapon
		// is at rest spread, and maximum scale is when it is at run spread.
		float BarScaleMin = 0.04f;
		float BarScaleMax = 0.03f;

		// This is the thickness of the crosshair lines, in pixels.
		float Thickness = 2.0f;
	};

	struct WABaseAttack : public WABase
	{
		enum class Classification
		{
			Melee = 0,
			Hitscan,
			Projectile,
			Event
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
		float AttackRate = 1.0f;  // Cycles per second

		WeightedValueList<int> ViewModelAnimList_Attack;
		WeightedValueList<int> ViewModelAnimList_AttackEmpty;
		WASoundSet AttackSounds;
		int Volume = NORMAL_GUN_VOLUME;

		// If specified, attack uses this body index for the view model.
		int ViewModelBodyOverride = -1;

		// If non-empty, this set of sounds will be played to the player holding the weapon.
		// The normal AttackSounds will be played to all other players.
		WASoundSet ViewModelAttackSounds;

		// If false, does not play the dry fire noise if the
		// weapon is fired with no ammo.
		bool PlayDryFireSoundOnEmpty = true;

		// If set, these animations will override the default ones specified
		// in the view model attributes when this attack mode is used.
		std::shared_ptr<ViewModelAnimationSet> OverrideAnimations;

		CrosshairParameters Crosshair;

		virtual Classification Classify() const = 0;

		void Validate() const override
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
}  // namespace WeaponAtts
