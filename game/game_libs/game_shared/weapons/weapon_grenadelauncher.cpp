#include "weapon_grenadelauncher.h"
#include "ammodefs.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_grenadelauncher_atts.h"

#ifndef CLIENT_DLL
#include <limits>
#include <algorithm>
#include "bot.h"
#endif

// Grenade attributes
static constexpr const char* GRENADELAUNCHER_GRENADE_MODEL = "models/weapon_grenadelauncher/w_grenade_projectile.mdl";
static const Vector GRENADELAUNCHER_HALF_BBOX = Vector(4,4,4);
static constexpr float GRENADELAUNCHER_GRENADE_FRICTION = 0.95;
static constexpr float GRENADELAUNCHER_GRENADE_SPRITE_SCALE = 60;

// Launcher attributes
static constexpr float GRENADELAUNCHER_TUMBLEVEL_MIN = -100.0f;
static constexpr float GRENADELAUNCHER_TUMBLEVEL_MAX = -500.0f;

LINK_ENTITY_TO_CLASS(weapon_grenadelauncher, CWeaponGrenadeLauncher);

#ifdef AFTERBURNER_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known weapons to this one.
LINK_ENTITY_TO_CLASS(weapon_ronin, CWeaponGrenadeLauncher)
LINK_ENTITY_TO_CLASS(weapon_rocketlauncher, CWeaponGrenadeLauncher)
#endif

CWeaponGrenadeLauncher::CWeaponGrenadeLauncher() :
	CGenericProjectileWeapon()
{
	SetPrimaryAttackModeFromAttributes(ATTACKMODE_CONTACT);
	SetSecondaryAttackModeFromAttributes(ATTACKMODE_TIMED);
}

const WeaponAtts::WACollection& CWeaponGrenadeLauncher::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponGrenadeLauncher>();
}

void CWeaponGrenadeLauncher::Precache()
{
	CGenericProjectileWeapon::Precache();

	PRECACHE_MODEL(GRENADELAUNCHER_GRENADE_MODEL);
}

#ifndef CLIENT_DLL
float CWeaponGrenadeLauncher::Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const
{
	const float explosionRadius = grenadelauncher_explosion_radius.value;

	// Default, unmodified preference:
	float pref = static_cast<float>(WeaponAttributes().Core.SwitchWeight) / static_cast<float>(WeaponPref_Max);

	// If the enemy is close enough that we'd damage ourselves with the grenade, scale back the preference accordingly.
	if ( distanceToEnemy > explosionRadius )
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
		return distanceToEnemy <= explosionRadius ? 0.0f : pref;
	}

	const float closestDistance = (1.0f - explosionFringeToleranceFactor) * explosionRadius;

	// How does the distance to the enemy compare to these two radii?
	// If <= closestDistance, we don't want to use the weapon.
	// If >= furtherstDistance, we use the original weapon preference.
	// Anything in-between is lerped.
	const float prefModifier = (distanceToEnemy - closestDistance) / (explosionRadius - closestDistance);
	return prefModifier * pref;
}

void CWeaponGrenadeLauncher::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
	fightStyle.SetSecondaryFire(false);
	fightStyle.SetAimAt(AIM_SPLASH);
	fightStyle.SetNextShootTime(m_flNextPrimaryAttack, 0.2f, 2.0f);
}

void CWeaponGrenadeLauncher::CreateProjectile(const WeaponAtts::WAProjectileAttack& projectileAttack)
{
	const bool isPrimaryAttack = projectileAttack.Signature()->Index == 0;

	UTIL_MakeVectors(GetGrenadeLaunchAngles());
	const Vector forward = gpGlobals->v_forward;
	const Vector location = m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + forward * 16.0f;

	CWeaponGrenadeLauncher_Grenade* grenade = CreateGrenade(m_pPlayer->pev, location, forward);

	grenade->SetExplodeOnContact(isPrimaryAttack);
	grenade->SetRandomTumbleAngVel(GRENADELAUNCHER_TUMBLEVEL_MIN, GRENADELAUNCHER_TUMBLEVEL_MAX);
	grenade->SetDamageOnExplode(gSkillData.plrDmgGrenadeLauncher);
	grenade->SetPlayerContactDamageMultiplier(gSkillData.plrDmgMultGrenadelauncherHit);
	grenade->SetOwnerDamageMultiplier(gSkillData.plrSelfDmgMultGrenadeLauncher);
	grenade->SetSpeed(grenadelauncher_launch_speed.value);
	grenade->SetFuseTime(isPrimaryAttack ? -1 : grenadelauncher_fuse_time.value);
}

CWeaponGrenadeLauncher_Grenade* CWeaponGrenadeLauncher::CreateGrenade(entvars_t *pevOwner, const Vector& location, const Vector& launchDir)
{
	CWeaponGrenadeLauncher_Grenade *pGrenade = GetClassPtr<CWeaponGrenadeLauncher_Grenade>(NULL);
	pGrenade->Spawn();

	UTIL_SetOrigin(pGrenade->pev, location);
	pGrenade->pev->velocity = launchDir;
	pGrenade->pev->angles = UTIL_VecToAngles(pGrenade->pev->velocity);
	pGrenade->pev->owner = ENT(pevOwner);
	pGrenade->pev->gravity = 1.4f;
	pGrenade->SetFuseTime(-1.0f);
	pGrenade->SetExplodeSpriteScale(GRENADELAUNCHER_GRENADE_SPRITE_SCALE);

	return pGrenade;
}

Vector CWeaponGrenadeLauncher::GetGrenadeLaunchAngles() const
{
	Vector viewAngles = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;

	// Add some more pitch depending on the cosine of the original pitch.
	// If the player is looking horizontally, we want the grenade to be
	// launched a little more upward; if they are looking straight up or
	// down, we don't want any modification at all.
	float extraPitch = -grenadelauncher_launch_pitch_adjust.value;
	viewAngles[0] += cos(UTIL_DegreesToRadians(viewAngles[0])) * extraPitch;

	if ( viewAngles[0] < -89.0f )
	{
		viewAngles[0] = -89.0f;
	}
	else if ( viewAngles[0] > 89.0f )
	{
		viewAngles[0] = 89.0f;
	}

	return viewAngles;
}
#endif

void CWeaponGrenadeLauncher_Grenade::Spawn()
{
	CGrenade::Spawn();
	SET_MODEL(ENT(pev), GRENADELAUNCHER_GRENADE_MODEL);
	UTIL_SetSize(pev, -GRENADELAUNCHER_HALF_BBOX, GRENADELAUNCHER_HALF_BBOX);
	pev->friction = GRENADELAUNCHER_GRENADE_FRICTION;
}

#ifndef CLIENT_DLL
void CWeaponGrenadeLauncher_Grenade::SetExplodeOnContact(bool explodeOnContact)
{
	SetTouch(explodeOnContact ? &CGrenade::ExplodeTouch : &CGrenade::BounceTouch);
}

void CWeaponGrenadeLauncher_Grenade::SetTumbleAngVel(float vel)
{
	pev->avelocity.x = vel;
}

void CWeaponGrenadeLauncher_Grenade::SetRandomTumbleAngVel(float min, float max)
{
	SetTumbleAngVel(RANDOM_FLOAT(min, max));
}

void CWeaponGrenadeLauncher_Grenade::SetDamageOnExplode(float damage)
{
	pev->dmg = damage;
}

void CWeaponGrenadeLauncher_Grenade::SetSpeed(float speed)
{
	Vector& vel = pev->velocity;

	if ( vel.Length() == 0.0f )
	{
		return;
	}

	vel = vel.Normalize() * speed;
}

void CWeaponGrenadeLauncher_Grenade::SetFuseTime(float fuseTime)
{
	if ( fuseTime < 0.0f )
	{
		SetThink(&CGrenade::DangerSoundThink);
		pev->nextthink = gpGlobals->time;
		return;
	}

	SetThink(&CGrenade::TumbleThink);

	pev->dmgtime = gpGlobals->time + fuseTime;
	pev->nextthink = gpGlobals->time + 0.1;

	if( fuseTime < 0.1 )
	{
		pev->nextthink = gpGlobals->time;
		pev->velocity = Vector(0, 0, 0);
	}
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponGrenadeLauncher>()
	{
		return ::StaticWeaponAttributes;
	}
}

class CAmmoGrenadeLauncher : public CGenericAmmo
{
public:
	CAmmoGrenadeLauncher()
		: CGenericAmmo("models/weapon_grenadelauncher/w_ammo_grenadelauncher.mdl", Ammo_GrenadeLauncher)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_grenadelauncher, CAmmoGrenadeLauncher)

#ifdef AFTERBURNER_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known ammo to this one.
LINK_ENTITY_TO_CLASS(ammo_rocketlauncher, CAmmoGrenadeLauncher)
#endif
