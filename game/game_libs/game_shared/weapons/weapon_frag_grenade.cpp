#include "weapons/weapon_frag_grenade.h"
#include "ammodefs.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapons/weapon_frag_grenade_atts.h"
#include "BuildPlatform/Decorators.h"

#ifndef CLIENT_DLL
#include <limits>
#include <algorithm>
#include "bot.h"
#include "genericgrenade.h"
#endif

static const Vector HALF_BBOX = Vector(4, 4, 4);
static constexpr float FRICTION = 0.7f;
static constexpr float GRAVITY = 1.4f;
static constexpr unsigned char SPRITE_SCALE = 60;
static constexpr float TUMBLEVEL_MIN = -100.0f;
static constexpr float TUMBLEVEL_MAX = -500.0f;
static constexpr float FUSE_TIME = 2.0f;
static constexpr float PITCH_ADJUST = 5;
static constexpr float PULL_DURATION_SECS = 0.75f;

LINK_ENTITY_TO_CLASS(weapon_frag_grenade, CWeaponFragGrenade);

// For Nightfire compatibility:
LINK_ENTITY_TO_CLASS(weapon_fraggrenade, CWeaponFragGrenade);

CWeaponFragGrenade::CWeaponFragGrenade() :
	CGenericWeapon()
{
	AddMechanicByAttributeIndex<WeaponAtts::WAProjectileAttack>(ATTACKMODE_THROW, m_ThrowMechanic);

#ifndef CLIENT_DLL
	m_ThrowMechanic->SetCreateProjectileCallback(
		[this](WeaponMechanics::CProjectileMechanic& mechanic)
		{
			CreateProjectile(mechanic);
			return true;
		}
	);
#endif

	SetPrimaryAttackMechanic(m_ThrowMechanic);
}

const WeaponAtts::WACollection& CWeaponFragGrenade::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponFragGrenade>();
}

void CWeaponFragGrenade::WeaponTick()
{
	CGenericWeapon::WeaponTick();

	const WeaponAtts::WAProjectileAttack* attackMode = m_ThrowMechanic->ProjectileAttackMode();

	switch ( m_ThrowState )
	{
		case ThrowState::Primed:
		{
			if ( m_pPlayer->pev->button & IN_ATTACK )
			{
				// Still holding the button
				break;
			}

			m_ThrowState = ThrowState::Released;
			DELIBERATE_FALL_THROUGH
		}

		case ThrowState::Released:
		{
			if ( !CanAttack(m_flNextPrimaryAttack, gpGlobals->time, UseDecrement()) )
			{
				break;
			}

			PrimaryAttack();
			SetNextPrimaryAttack(1.0f / attackMode->AttackRate);
			m_ThrowState = ThrowState::Idle;
			break;
		}

		default:
		{
			if ( CanAttack(m_flNextPrimaryAttack, gpGlobals->time, UseDecrement()) && !HasAmmo(attackMode, 1, false) )
			{
				RetireWeapon();
			}

			break;
		}
	}
}

bool CWeaponFragGrenade::PrepareToInvokeAttack(WeaponAtts::AttackMode mode)
{
	if ( !CGenericWeapon::PrepareToInvokeAttack(mode) )
	{
		return false;
	}

	if ( m_ThrowState == ThrowState::Idle )
	{
		SendWeaponAnim(FRAGGRENADE_PULL);

		// Set minimum time we have to wait until grenade can be thrown.
		SetNextPrimaryAttack(PULL_DURATION_SECS);

		m_ThrowState = ThrowState::Primed;
	}

	// Only actually allow the invocation of the projectile mechanic
	// once WeaponTick() has put us into the Released state.
	return m_ThrowState == ThrowState::Released;
}

bool CWeaponFragGrenade::ReadPredictionData(const weapon_data_t* from)
{
	if ( !CGenericWeapon::ReadPredictionData(from) )
	{
		return false;
	}

	m_ThrowState = static_cast<ThrowState>(pev->iuser1);
	return true;
}

bool CWeaponFragGrenade::WritePredictionData(weapon_data_t* to)
{
	if ( !CGenericWeapon::WritePredictionData(to) )
	{
		return false;
	}

	pev->iuser1 = static_cast<int>(m_ThrowState);
	return true;
}

#ifndef CLIENT_DLL
TYPEDESCRIPTION CWeaponFragGrenade::m_SaveData[] = {
	DEFINE_FIELD(CWeaponFragGrenade, m_ThrowState, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CWeaponFragGrenade, CGenericWeapon)

float CWeaponFragGrenade::Bot_CalcDesireToUse(CBaseBot&, CBaseEntity&, float) const
{
	return static_cast<float>(WeaponAttributes().Core.SwitchWeight) / static_cast<float>(WeaponPref_Max);
}

void CWeaponFragGrenade::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
	fightStyle.SetSecondaryFire(false);
	fightStyle.SetAimAt(AIM_SPLASH);
	fightStyle.SetNextShootTime(m_flNextPrimaryAttack, 0.2f, 2.0f);
}

void CWeaponFragGrenade::CreateProjectile(const WeaponMechanics::CProjectileMechanic& mechanic)
{
	CGenericGrenade* grenade =
		CGenericGrenade::CreateGrenade(m_pPlayer, mechanic.GetProjectileLaunchAngles(PITCH_ADJUST), 16.0f);

	grenade->SetModelName(mechanic.ProjectileAttackMode()->ProjectileModelName);
	grenade->SetSize(HALF_BBOX);
	grenade->SetFriction(FRICTION);
	grenade->SetGravity(GRAVITY);
	grenade->SetExplodeSpriteScale(SPRITE_SCALE);
	grenade->SetExplodeOnContact(false);
	grenade->SetRandomTumbleAngVel(TUMBLEVEL_MIN, TUMBLEVEL_MAX);
	grenade->SetDamageOnExplode(mechanic.ProjectileAttackMode()->BaseExplosionDamage);
	grenade->SetPlayerContactDamageMultiplier(1.0f);
	grenade->SetOwnerDamageMultiplier(gSkillData.plrSelfDmgMultFragGrenade);
	grenade->SetSpeed(mechanic.ProjectileAttackMode()->LaunchSpeed);
	grenade->SetFuseTime(FUSE_TIME);
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFragGrenade>()
	{
		return ::StaticWeaponAttributes;
	}
}  // namespace WeaponAtts

class CAmmoFragGrenade : public CGenericAmmo
{
public:
	CAmmoFragGrenade() :
		CGenericAmmo("models/weapon_frag_grenade/w_ammo_frag_grenade.mdl", Ammo_FragGrenade)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_frag_grenade, CAmmoFragGrenade)

// For Nightfire compatibility:
LINK_ENTITY_TO_CLASS(ammo_fraggrenade, CAmmoFragGrenade)
