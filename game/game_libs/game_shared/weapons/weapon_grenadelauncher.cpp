#include "weapon_grenadelauncher.h"
#include "ammodefs.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_grenadelauncher_atts.h"
#include "weapons/genericgrenade.h"

#ifndef CLIENT_DLL
#include <limits>
#include <algorithm>
#include "bot.h"
#endif

static const Vector GRENADELAUNCHER_HALF_BBOX = Vector(4, 4, 4);
static constexpr float GRENADELAUNCHER_GRENADE_FRICTION = 0.95f;
static constexpr float GRENADELAUNCHER_GRENADE_GRAVITY = 1.4f;
static constexpr unsigned char GRENADELAUNCHER_GRENADE_SPRITE_SCALE = 60;
static constexpr float GRENADELAUNCHER_TUMBLEVEL_MIN = -100.0f;
static constexpr float GRENADELAUNCHER_TUMBLEVEL_MAX = -500.0f;
static constexpr float GRENADELAUNCHER_EXPLOSION_RADIUS = 250.0f;
static constexpr float GRENADELAUNCHER_FUSE_TIME = 2.0f;
static constexpr float GRENADELAUNCHER_PITCH_ADJUST = 5;

LINK_ENTITY_TO_CLASS(weapon_grenadelauncher, CWeaponGrenadeLauncher);

#ifdef NFOPEN_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known weapons to this one.
LINK_ENTITY_TO_CLASS(weapon_rocketlauncher, CWeaponGrenadeLauncher)
#endif

CWeaponGrenadeLauncher::CWeaponGrenadeLauncher() :
	CGenericWeapon()
{
	AddMechanicByAttributeIndex<WeaponAtts::WAProjectileAttack>(ATTACKMODE_CONTACT, m_ContactGrenadeAttack);
	AddMechanicByAttributeIndex<WeaponAtts::WAProjectileAttack>(ATTACKMODE_TIMED, m_TimedGrenadeAttack);

#ifndef CLIENT_DLL
	const auto createProjectile = [this](WeaponMechanics::CProjectileMechanic& mechanic)
	{
		CreateProjectile(mechanic);
		return true;
	};

	m_ContactGrenadeAttack->SetCreateProjectileCallback(createProjectile);
	m_TimedGrenadeAttack->SetCreateProjectileCallback(createProjectile);
#endif

	SetPrimaryAttackMechanic(m_ContactGrenadeAttack);
	SetSecondaryAttackMechanic(m_TimedGrenadeAttack);
}

const WeaponAtts::WACollection& CWeaponGrenadeLauncher::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponGrenadeLauncher>();
}

#ifndef CLIENT_DLL
float CWeaponGrenadeLauncher::Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity&, float distanceToEnemy) const
{
	// Default, unmodified preference:
	float pref = static_cast<float>(WeaponAttributes().Core.SwitchWeight) / static_cast<float>(WeaponPref_Max);

	// If the enemy is close enough that we'd damage ourselves with the grenade, scale back the preference accordingly.
	if ( distanceToEnemy > GRENADELAUNCHER_EXPLOSION_RADIUS )
	{
		return pref;
	}

	// This is the fringe around the edge of the explosion sphere, proportional to the
	// radius, that we will tolerate being in.
	float explosionFringeToleranceFactor = 0.15f;

	// Scale this by how much health we have. If we have 1hp, we want it to be 0.
	const float toleranceScale = std::max<float>(bot.pev->health - 1, 0) / std::max<float>(bot.pev->max_health - 1, 0);
	explosionFringeToleranceFactor *= toleranceScale;

	// If there is no tolerance zone at all, return 0 if we touch the radius.
	if ( explosionFringeToleranceFactor < std::numeric_limits<float>::min() )
	{
		return distanceToEnemy <= GRENADELAUNCHER_EXPLOSION_RADIUS ? 0.0f : pref;
	}

	const float closestDistance = (1.0f - explosionFringeToleranceFactor) * GRENADELAUNCHER_EXPLOSION_RADIUS;

	// How does the distance to the enemy compare to these two radii?
	// If <= closestDistance, we don't want to use the weapon.
	// If >= furtherstDistance, we use the original weapon preference.
	// Anything in-between is lerped.
	const float prefModifier =
		(distanceToEnemy - closestDistance) / (GRENADELAUNCHER_EXPLOSION_RADIUS - closestDistance);
	return prefModifier * pref;
}

void CWeaponGrenadeLauncher::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
	fightStyle.SetSecondaryFire(false);
	fightStyle.SetAimAt(AIM_SPLASH);
	fightStyle.SetNextShootTime(m_flNextPrimaryAttack, 0.2f, 2.0f);
}

void CWeaponGrenadeLauncher::CreateProjectile(const WeaponMechanics::CProjectileMechanic& mechanic)
{
	CGenericGrenade* grenade = CGenericGrenade::CreateGrenade(
		m_pPlayer,
		mechanic.GetProjectileLaunchAngles(GRENADELAUNCHER_PITCH_ADJUST),
		16.0f);

	const bool explodeOnContact = &mechanic == m_ContactGrenadeAttack;

	grenade->SetModelName(mechanic.ProjectileAttackMode()->ProjectileModelName);
	grenade->SetSize(GRENADELAUNCHER_HALF_BBOX);
	grenade->SetFriction(GRENADELAUNCHER_GRENADE_FRICTION);
	grenade->SetGravity(GRENADELAUNCHER_GRENADE_GRAVITY);
	grenade->SetExplodeSpriteScale(GRENADELAUNCHER_GRENADE_SPRITE_SCALE);
	grenade->SetExplodeOnContact(explodeOnContact);
	grenade->SetRandomTumbleAngVel(GRENADELAUNCHER_TUMBLEVEL_MIN, GRENADELAUNCHER_TUMBLEVEL_MAX);
	grenade->SetDamageOnExplode(gSkillData.plrDmgGrenadeLauncher);
	grenade->SetPlayerContactDamageMultiplier(gSkillData.plrDmgMultGrenadelauncherHit);
	grenade->SetOwnerDamageMultiplier(gSkillData.plrSelfDmgMultGrenadeLauncher);
	grenade->SetSpeed(mechanic.ProjectileAttackMode()->LaunchSpeed);
	grenade->SetFuseTime(explodeOnContact ? -1.0f : GRENADELAUNCHER_FUSE_TIME);
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponGrenadeLauncher>()
	{
		return ::StaticWeaponAttributes;
	}
}  // namespace WeaponAtts

class CAmmoGrenadeLauncher : public CGenericAmmo
{
public:
	CAmmoGrenadeLauncher() :
		CGenericAmmo("models/weapon_grenadelauncher/w_ammo_grenadelauncher.mdl", Ammo_GrenadeLauncher)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_grenadelauncher, CAmmoGrenadeLauncher)

#ifdef NFOPEN_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known ammo to this one.
LINK_ENTITY_TO_CLASS(ammo_rocketlauncher, CAmmoGrenadeLauncher)
#endif
