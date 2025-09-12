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
void CWeaponGrenadeLauncher::CreateProjectile(const WeaponMechanics::CProjectileMechanic& mechanic)
{
	CGenericGrenade* grenade = CGenericGrenade::CreateGrenade(
		m_pPlayer,
		mechanic.GetProjectileLaunchAngles(GRENADELAUNCHER_PITCH_ADJUST),
		16.0f
	);

	const bool explodeOnContact = &mechanic == m_ContactGrenadeAttack;

	grenade->SetModelName(mechanic.ProjectileAttackMode()->ProjectileModelName);
	grenade->SetSize(GRENADELAUNCHER_HALF_BBOX);
	grenade->SetFriction(GRENADELAUNCHER_GRENADE_FRICTION);
	grenade->SetGravity(GRENADELAUNCHER_GRENADE_GRAVITY);
	grenade->SetExplodeSpriteScale(GRENADELAUNCHER_GRENADE_SPRITE_SCALE);
	grenade->SetExplodeOnContact(explodeOnContact);
	grenade->SetRandomTumbleAngVel(GRENADELAUNCHER_TUMBLEVEL_MIN, GRENADELAUNCHER_TUMBLEVEL_MAX);
	grenade->SetDamageOnExplode(mechanic.ProjectileAttackMode()->BaseExplosionDamage);
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
