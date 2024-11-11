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
#endif

static constexpr const char* GRENADE_MODEL = "models/weapon_frag_grenade/w_frag_grenade.mdl";
static const Vector HALF_BBOX = Vector(4, 4, 4);
static constexpr float FRICTION = 0.7f;
static constexpr float GRAVITY = 1.4f;
static constexpr unsigned char SPRITE_SCALE = 60;
static constexpr float TUMBLEVEL_MIN = -100.0f;
static constexpr float TUMBLEVEL_MAX = -500.0f;
static constexpr float LAUNCH_SPEED = 1000.0f;
static constexpr float FUSE_TIME = 2.0f;
static constexpr float PITCH_ADJUST = 5;
static constexpr float PULL_DURATION_SECS = 0.75f;

LINK_ENTITY_TO_CLASS(weapon_frag_grenade, CWeaponFragGrenade);

// For Nightfire compatibility:
LINK_ENTITY_TO_CLASS(weapon_fraggrenade, CWeaponFragGrenade);

CWeaponFragGrenade::CWeaponFragGrenade() :
	CBaseGrenadeLauncher()
{
	SetPrimaryAttackModeFromAttributes(ATTACKMODE_THROW);
}

const WeaponAtts::WACollection& CWeaponFragGrenade::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponFragGrenade>();
}

void CWeaponFragGrenade::Precache()
{
	CBaseGrenadeLauncher::Precache();

	PRECACHE_MODEL(GRENADE_MODEL);
}

void CWeaponFragGrenade::WeaponTick()
{
	CBaseGrenadeLauncher::WeaponTick();

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

			CBaseGrenadeLauncher::PrimaryAttack();
			SetNextPrimaryAttack(1.0f / GetPrimaryAttackMode()->AttackRate);
			m_ThrowState = ThrowState::Idle;
			break;
		}

		default:
		{
			if ( CanAttack(m_flNextPrimaryAttack, gpGlobals->time, UseDecrement()) &&
				 !HasAmmo(GetPrimaryAttackMode(), 1, false) )
			{
				RetireWeapon();
			}

			break;
		}
	}
}

void CWeaponFragGrenade::PrimaryAttack()
{
	if ( !CGenericWeapon::InvokeWithAttackMode(WeaponAtts::AttackMode::Primary, GetPrimaryAttackMode()) ||
		 m_ThrowState != ThrowState::Idle )
	{
		return;
	}

	SendWeaponAnim(FRAGGRENADE_PULL);

	// Set minimum time we have to wait until grenade can be thrown.
	SetNextPrimaryAttack(PULL_DURATION_SECS);

	m_ThrowState = ThrowState::Primed;
}

bool CWeaponFragGrenade::ReadPredictionData(const weapon_data_t* from)
{
	if ( !CBaseGrenadeLauncher::ReadPredictionData(from) )
	{
		return false;
	}

	m_ThrowState = static_cast<ThrowState>(pev->iuser1);
	return true;
}

bool CWeaponFragGrenade::WritePredictionData(weapon_data_t* to)
{
	if ( !CBaseGrenadeLauncher::WritePredictionData(to) )
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

IMPLEMENT_SAVERESTORE(CWeaponFragGrenade, CBaseGrenadeLauncher)

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

void CWeaponFragGrenade::CreateProjectile(const WeaponAtts::WAProjectileAttack&)
{
	CBaseGrenadeLauncher_Grenade* grenade = CreateGrenade(PITCH_ADJUST, 16.0f);

	grenade->SetModelName(GRENADE_MODEL);
	grenade->SetSize(HALF_BBOX);
	grenade->SetFriction(FRICTION);
	grenade->SetGravity(GRAVITY);
	grenade->SetExplodeSpriteScale(SPRITE_SCALE);
	grenade->SetExplodeOnContact(false);
	grenade->SetRandomTumbleAngVel(TUMBLEVEL_MIN, TUMBLEVEL_MAX);
	grenade->SetDamageOnExplode(gSkillData.plrDmgFragGrenade);
	grenade->SetPlayerContactDamageMultiplier(1.0f);
	grenade->SetOwnerDamageMultiplier(gSkillData.plrSelfDmgMultFragGrenade);
	grenade->SetSpeed(LAUNCH_SPEED);
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
