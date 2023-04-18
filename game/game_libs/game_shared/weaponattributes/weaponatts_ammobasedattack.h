#pragma once

#include "vector_classes.h"
#include "weaponatts_baseattack.h"
#include "weapons.h"
#include "resources/ShellImpactSoundResources.h"

namespace WeaponAtts
{
	struct AccuracyParameters
	{
		// Weapon inaccuracy calculations take a number of parameters. The inaccuracy
		// of a weapon depends on the speed at which the player is moving, as well as
		// whether the weapon has been fired recently. The inaccuracy value is always
		// hard clamped within the range [0 1].

		// To modify these settings while debugging/testing, see the list of convars
		// in game_libs/dlls/gameplay/gameplayCvars.cpp

		// To begin, we define two points in the [0 1] range: the inaccuracy value
		// at rest, and the inaccuracy value when the player is running at full speed.

		// Weapon's inaccuracy, in range [0 1], when the player is standing still.
		float RestValue = 0.1f;

		// Weapon's inaccuracy, in range [0 1], when the player is running at
		// sv_weapon_inaccuracy_maxspeed units per second.
		float RunValue = 0.5f;

		// The first step of calculating the weapon inaccuracy checks the player's
		// XY speed, and determines an inaccuracy value linearly interpolated between
		// these two values. After this, one or more shifts are added based on
		// other states that the player might be in. The resulting value will be
		// clamped to the range [0 1].

		// Shift applied when the player is crouching.
		float CrouchShift = -0.08f;

		// Shift applied when the player is not touching the ground.
		float AirShift = 0.2f;

		// Shift applied when the player is moving on Z at sv_weapon_inaccuracy_maxfallspeed
		// units per second. If the player is moving slower than this, the
		// actual shift that is applied is interpolated based on the Z speed.
		float FallShift = 0.1f;

		// This creates a base, instantaneous level of inaccuracy for the weapon on this
		// particular frame. A smoothed inaccuracy level is then calculated based on the
		// difference between the current and previous frames. The attack coefficient
		// specifies how much smoothing is applied when the inaccuracy value is increasing.
		// A value of 1 means no smoothing (the instantaneous inaccuracy is used verbatim),
		// and values closer to 0 cause more smoothing (ie. the smoothed value tends towards
		// the instantaneous value more slowly). A value of 0 means the inaccuracy value
		// will never actually change.
		float AttackCoefficient = 0.3f;

		// This coefficient is used in the same way, but when the inaccuracy value is decreasing.
		float DecayCoefficient = 0.3f;

		// Next, if the weapon has been fired on the current frame, an inaccuracy penalty
		// is applied to the smoothed value. FireImpulse specifies how large this penalty is.
		// The inaccuracy value will remain clamped to a maximum of 1.
		float FireImpulse = 0.1f;

		// The fire impulse is only allowed to deviate a certain amount from the instantaneous
		// inaccuracy value. FireImpulseCeiling specifies this max deviation.
		float FireImpulseCeiling = 0.3f;

		// Once the fire impulse has been applied, the value is held at this point for a certain
		// amount of time before receding. The hold time is specified here, in seconds.
		float FireImpulseHoldTime = 0.05f;

		// Finally, the inaccuracy value is used to calculate the weapon's spread when fired.
		// The two points that define the spread scale are specified below, and use RestValue
		// and RunValue as reference points. Note that other factors (eg. crouching, falling,
		// firing) may mean that the actual weapon spread is larger or smaller than these
		// values. The resulting spread is clamped to a minimum of zero.

		// Weapon spread when the inaccuracy is equal to RestValue.
		Vector2D RestSpread;

		// Weapon spread when the inaccuracy is equal to RunValue.
		Vector2D RunSpread;

		inline void Validate() const
		{
			ASSERTSZ_Q(RestValue >= 0.0f && RestValue <= 1.0f, "Rest value must be in range [0 1].");
			ASSERTSZ_Q(RunValue >= 0.0f && RunValue <= 1.0f, "Run value must be in range [0 1].");
			ASSERTSZ_Q(AttackCoefficient >= 0.0f && AttackCoefficient <= 1.0f, "Attack coefficient must be in range [0 1].");
			ASSERTSZ_Q(DecayCoefficient >= 0.0f && DecayCoefficient <= 1.0f, "Decay coefficient must be in range [0 1].");
			ASSERTSZ_Q(RestSpread.x >= 0.0f && RestSpread.y >= 0.0f, "Rest spread cannot be negative.");
			ASSERTSZ_Q(RunSpread.x >= 0.0f && RunSpread.y >= 0.0f, "Run spread cannot be negative.");
		}
	};

	struct CrosshairParameters
	{
		bool HasCrosshair = true;

		// To modify these settings while debugging/testing, see the list of convars
		// in game_libs/cl_dll/gameplay/crosshairCvars.cpp

		// The radius is how far away from the centre of the screen each
		// crosshair bar is. A value of 1 means the length of the shortest
		// screen dimension.
		float RadiusMin = 0.0f;
		float RadiusMax = 0.5f;

		// These scales specify how long the crosshair bars are at minimum
		// and maximum inaccuracy. A value of 1 means the bar is the length
		// of the shortest screen dimension.
		float BarScaleMin = 0.04f;
		float BarScaleMax = 0.03f;
	};

	struct WAAmmoBasedAttack : public WABaseAttack
	{
		enum class AmmoPool
		{
			None = -1,
			Primary = 0,
			Secondary = 1
		};

		static constexpr size_t AMMO_POOL_COUNT = 2;

		AmmoPool UsesAmmoPool = AmmoPool::None;
		AccuracyParameters Accuracy;
		CrosshairParameters Crosshair;
		int MuzzleFlashBrightness = NORMAL_GUN_FLASH;
		float ViewPunchY = 0.0f;
		const char* ShellModelName = nullptr;
		ShellType ShellModelType = ShellType::Default;
		bool AutoReload = true;
		bool SpecialReload = false;

		virtual void Validate() const override
		{
			Accuracy.Validate();
		}
	};
}
