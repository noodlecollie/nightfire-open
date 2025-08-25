#include "weaponatts_botinterface.h"
#include "hulldefs.h"
#include "skill.h"
#include "weaponatts_hitscanattack.h"
#include "weaponatts_meleeattack.h"
#include "weaponatts_projectileattack.h"
#include "weaponatts_ammodef.h"
#include "ammodefs.h"
#include "util/projectileMotion.h"

namespace WeaponAtts
{
	static float CalcParabolicDistance(float launchPitch, float launchSpeed)
	{
		static constexpr float DEFAULT_GRAVITY = 800.0f;
		static constexpr float DEFAULT_EYE_OFFSET_FROM_GROUND = VEC_VIEW.z - VEC_HULL_MIN.z;

		return CalculateProjectileDistanceAtGroundContact(
			DEFAULT_EYE_OFFSET_FROM_GROUND,
			launchPitch,
			launchSpeed,
			DEFAULT_GRAVITY
		);
	}

	void WABotAttackMode::ApplyMode(const WABaseAttack* mode)
	{
		ASSERTSZ_Q(mode, "Expected a valid attack mode");

		AttackRate = mode->AttackRate;
		AttackButtonPressTime = 3.0f / AttackRate;

		switch ( mode->Classify() )
		{
			case WABaseAttack::Classification::Hitscan:
			{
				const WAHitscanAttack* hitscan = dynamic_cast<const WAHitscanAttack*>(mode);
				ASSERTSZ_Q(hitscan, "Expected valid hitscan attack");

				AmmoDecrementPerAttackCycle = hitscan->AmmoDecrement;
				BaseDamagePerShot = hitscan->BaseDamagePerShot;
				ReloadDuration = hitscan->ReloadDuration;

				break;
			}

			case WABaseAttack::Classification::Projectile:
			{
				const WAProjectileAttack* projectile = dynamic_cast<const WAProjectileAttack*>(mode);
				ASSERTSZ_Q(projectile, "Expected valid projectile attack");

				AmmoDecrementPerAttackCycle = projectile->AmmoDecrement;
				ReloadDuration = projectile->ReloadDuration;
				ParabolicDistanceAt0Degrees = CalcParabolicDistance(0.0f, projectile->LaunchSpeed);
				ParabolicDistanceAt45Degrees = CalcParabolicDistance(-45.0f, projectile->LaunchSpeed);
				BaseDamagePerShot = projectile->BaseExplosionDamage;

				break;
			}

			case WABaseAttack::Classification::Melee:
			{
				const WAMeleeAttack* melee = dynamic_cast<const WAMeleeAttack*>(mode);
				ASSERTSZ_Q(melee, "Expected valid melee attack");

				BaseDamagePerShot = melee->BaseDamagePerHit;

				break;
			}

			case WABaseAttack::Classification::Event:
			{
				// Event attack modes just delegate to the weapon,
				// so we don't have any information from them that
				// we can use here.
				ASSERTSZ_Q(false, "Event attack mode is not supported");
				break;
			}

			default:
			{
				ASSERTSZ_Q(false, "Unexpected attack mode type");
				break;
			}
		}
	}

	void WABotAttackMode::ApplyAmmo(const CAmmoDef& ammoDef, int defaultAmmo, int maxClip)
	{
		MaxAmmo = ammoDef.MaxCarry;
		AmmoGivenOnPickup = ammoDef.AmmoBoxGive;
		ClipSize = maxClip;
		DefaultAmmoOnFirstPickup = defaultAmmo;
		BulletsAddedPerReload = ClipSize > 0 ? static_cast<size_t>(ClipSize) : 0;
	}
}  // namespace WeaponAtts
