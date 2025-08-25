#pragma once

#include "weaponatts_base.h"
#include "weaponatts_baseattack.h"
#include "weaponatts_skillrecord.h"
#include <memory>

struct CAmmoDef;

namespace WeaponAtts
{
	struct WAAmmoDef;
	struct WAHitscanAttack;
	struct WAProjectileAttack;
	struct WAMeleeAttack;

	// Botrix attributes that we need to cater for:
	// - Is primary or secondary: unsure whether this is relevant here. This is probably
	//   more for games like TF2 where weapons are in slots.
	// - Flags: Things like "is trigger for remote detonation", or "will extinguish players".
	//   These would probably best be calculated based on whatever stats are relevant.
	// - Weapon type: eg. melee, grenade, pistol, rifle, etc.
	// - Preference: is a category instead of an absolute number. This is probably a better
	//   way of doing it than what we do.
	// - Team: whether a weapon is usable only by a specific team. I think this would
	//   only apply to Oddjob's hat. Should be kept in mind.
	// - Where to aim: head, body, foot.
	// - Clip size: from attack mode.
	// - Damage: this is actually dynamic based on the difficulty. We can store the member
	//   pointer that we already use, and re-apply Botrix config at the same time as
	//   we reapply difficulty.
	// - Delay: seconds per attack cycle, from attack mode.
	// - Time to hold attack before releasing it: useful for grenades.
	//   This is only really useful for bots. Could be set on attack mode.
	// - How many bullets are reloaded each time: only really useful for
	//   bots. Could be set on attack mode.
	// - Time taken to reload: only really useful for bots. Could be set on attack mode.
	// - Time to start reloading: only really useful for bots. Could be set on attack mode.
	// - Holster time: could be set on core.
	// - Default ammo when weapon is picked up: is set in AmmoDef.
	// - Max ammo: is set in AmmoDef.
	// - Bullets fired per cycle: from attack mode.
	// - Zoom distance and time: not currently catered for. Not clear how to calculate distance.
	//   Probably can't live in attack mode since this describes the bullet being fired, rather
	//   than the zoom function.
	// - Ammo classname and count given: can get from AmmoDef.
	// - Parabolic range for projectiles: at 0 degrees and at 45 degrees. We don't currently
	//   have anything like this, but could calculate it given a launch velocity and gravity.
	// - Min and max effective range: don't currently have anything like this. Unclear
	//   whether it can be calculated, or should just be specified.
	// Since the bot code assumes a primary and a secondary function, perhaps we could create
	// a struct to provide this sort of interface, which can link to the relevant components
	// that are used for the weapon.

	enum class BotWeaponType
	{
		Melee,
		GrenadeProjectile,
		RemoteDetonation,
		HitscanSingleShot,  // "Pistol" in Botrix
		HitscanContinuous,  // "Rifle" in Botrix
		Shotgun,
		RocketProjectile,
	};

	enum class BotWeaponPreference
	{
		Never,
		Lowest,
		Low,
		Normal,
		High,
		Highest,
	};

	enum class BotWeaponTeamRestriction
	{
		None,
		MI6,
		Phoenix
	};

	enum class BotEnemyAimAt
	{
		Body,
		Head,
		Foot,
	};

	enum class BotWeaponReloadStyle
	{
		PerClip,
		PerShot,
	};

	struct WABotAttackMode
	{
		BotEnemyAimAt EnemyAimAt = BotEnemyAimAt::Body;
		BotWeaponReloadStyle ReloadStyle = BotWeaponReloadStyle::PerClip;
		float ZoomDistance = 0.0f;
		float ZoomTime = 0.0f;
		float ReloadStartDelay = 0.0f;

		// Max range <= 0 implies no max range.
		float MinEffectiveRange = 0.0f;
		float MaxEffectiveRange = 0.0f;

		// Set by ApplyMode():
		float AttackRate = 0.0f;
		size_t AmmoDecrementPerAttackCycle = 1;
		WASkillRecord::SkillDataEntryPtr BaseDamagePerShot = nullptr;
		float AttackButtonPressTime = 0.0f;
		float ReloadDuration = 0.0f;
		float ParabolicDistanceAt0Degrees = 0.0f;
		float ParabolicDistanceAt45Degrees = 0.0f;

		// Set by ApplyAmmo():
		int MaxAmmo = 0;
		int AmmoGivenOnPickup = 0;
		int DefaultAmmoOnFirstPickup = 0;
		int ClipSize = -1;
		size_t BulletsAddedPerReload = 1;

		void ApplyMode(const WABaseAttack* mode);
		void ApplyAmmo(const CAmmoDef& ammoDef, int defaultAmmo, int maxClip);

		void Validate(bool needsAmmo) const
		{
			ASSERTSZ_Q(AttackRate > 0, "Expected attack rate > 0");
			ASSERTSZ_Q(BaseDamagePerShot, "Expected base damage ptr");
			ASSERTSZ_Q(AttackButtonPressTime > 0.0f, "Expected attack button press time > 0");

			if ( needsAmmo )
			{
				ASSERTSZ_Q(AmmoClassName > 0, "Expected ammo class name");
				ASSERTSZ_Q(AmmoName > 0, "Expected ammo name");
				ASSERTSZ_Q(MaxAmmo > 0, "Expected max ammo > 0");
				ASSERTSZ_Q(AmmoGivenOnPickup > 0, "Expected ammo given on pickup > 0");
			}
		}
	};

	struct WABotInterface : public WABase
	{
		BotWeaponType Type = BotWeaponType::HitscanSingleShot;
		BotWeaponPreference Preference = BotWeaponPreference::Never;
		BotWeaponTeamRestriction TeamRestriction = BotWeaponTeamRestriction::None;

		std::unique_ptr<WABotAttackMode> PrimaryAttackMode;
		std::unique_ptr<WABotAttackMode> SecondaryAttackMode;

		void Validate() const override
		{
			if ( Preference == BotWeaponPreference::Never )
			{
				ASSERTSZ_Q(!PrimaryAttackMode.get(), "Expeted no primary attack mode");
				ASSERTSZ_Q(!SecondaryAttackMode.get(), "Expeted no secondary attack mode");
			}
			else
			{
				ASSERTSZ_Q(PrimaryAttackMode.get(), "Expeted to have a primary attack mode");
			}

			if ( PrimaryAttackMode )
			{
				PrimaryAttackMode->Validate(Type != BotWeaponType::Melee);
			}

			if ( SecondaryAttackMode )
			{
				SecondaryAttackMode->Validate(Type != BotWeaponType::Melee);
			}
		}
	};
}  // namespace WeaponAtts
