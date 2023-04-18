#include "weapon_frinesi.h"
#include "weaponinfo.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_frinesi_atts.h"

#ifndef CLIENT_DLL
#include "bot.h"
#endif

enum FrinesiReloadState_e
{
	RELOAD_IDLE = 0,
	RELOAD_LOAD_SHELL,
	RELOAD_INCREMENT_CLIP
};

static constexpr int RELOAD_MASK = 0x3;
static constexpr int RELOAD_FLAG_INTERRUPTED = (1 << 2);
static constexpr int RELOAD_FLAG_LOADED_ONCE = (1 << 3);
static constexpr int NextReloadState(int orig, int next)
{
	return (orig & ~(RELOAD_MASK)) | next;
}

LINK_ENTITY_TO_CLASS(weapon_frinesi, CWeaponFrinesi)

#ifdef AFTERBURNER_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known weapons to this one.
LINK_ENTITY_TO_CLASS(weapon_commando, CWeaponFrinesi)
LINK_ENTITY_TO_CLASS(weapon_minigun, CWeaponFrinesi)
#endif

CWeaponFrinesi::CWeaponFrinesi()
	: CGenericHitscanWeapon(),
	  m_flNextPumpSoundTime(0.0f),
	  m_flNextReloadSoundTime(0.0f)
{
	m_pAutoAttackMode = GetAttackModeFromAttributes<WeaponAtts::WAHitscanAttack>(ATTACKMODE_AUTO);
	m_pPumpAttackMode = GetAttackModeFromAttributes<WeaponAtts::WAHitscanAttack>(ATTACKMODE_PUMP);

	SetPrimaryAttackMode(m_pAutoAttackMode);
	SetSecondaryAttackMode(m_pPumpAttackMode);
}

void CWeaponFrinesi::Precache()
{
	CGenericHitscanWeapon::Precache();
	PRECACHE_SOUND(FRINESI_COCK_SOUND);
}

bool CWeaponFrinesi::InvokeWithAttackMode(const CGenericWeapon::WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode)
{
	if ( FlagReloadInterrupt() )
	{
		return false;
	}

	// We set the primary attack mode based on the type.
	// This allows us to continue to use weapon inaccuracy properly,
	// as this only acts on the primary attack.
	const WeaponAtts::WABaseAttack* overrideMode = type == WeaponAttackType::Secondary ? m_pPumpAttackMode : m_pAutoAttackMode;
	SetPrimaryAttackMode(overrideMode);

	const bool invokeResult = CGenericHitscanWeapon::InvokeWithAttackMode(WeaponAttackType::Primary, overrideMode);

	if ( invokeResult && type == WeaponAttackType::Secondary )
	{
		m_flNextPumpSoundTime = gpGlobals->time + FRINESI_PUMP_SOUND_OFFSET_AFTER_FIRING;
	}

	return invokeResult;
}

void CWeaponFrinesi::Holster(int skipLocal)
{
	CGenericHitscanWeapon::Holster(skipLocal);
	DelayPendingActions(0.1f, true);
}

void CWeaponFrinesi::WeaponTick()
{
	// If we're reloading and have already flagged an interrupt, clear the input buttons.
	// This is to prevent the player holding down attack buttons and preventing
	// the weapon going into an idle state.
	if ( (m_fInSpecialReload & RELOAD_MASK) && (m_fInSpecialReload & RELOAD_FLAG_INTERRUPTED) )
	{
		m_pPlayer->pev->button &= ~(IN_ATTACK | IN_ATTACK2);
	}

	if ( m_flNextPumpSoundTime != 0.0f && m_flNextPumpSoundTime < gpGlobals->time )
	{
		PlayPumpSound();
		m_flNextPumpSoundTime = 0.0f;
	}

	if ( m_flNextReloadSoundTime != 0.0f && m_flNextReloadSoundTime < gpGlobals->time )
	{
		PlaySound(WeaponAttributes().ViewModel.ReloadSounds, CHAN_ITEM);
		m_flNextReloadSoundTime = 0.0f;
	}
}

bool CWeaponFrinesi::FlagReloadInterrupt()
{
	// If we're reloading, allow an interruption.
	if ( (m_fInSpecialReload & RELOAD_MASK) != RELOAD_IDLE )
	{
		m_fInSpecialReload |= RELOAD_FLAG_INTERRUPTED;
		return true;
	}

	return false;
};

int CWeaponFrinesi::HandleSpecialReload(int currentState)
{
	switch (currentState & RELOAD_MASK)
	{
		case RELOAD_IDLE:
		{
			m_flNextReloadSoundTime = 0.0f;
			m_flNextPumpSoundTime = 0.0f;
			SendWeaponAnim(FRINESI_START_RELOAD);

			// Set both our next firing times to be now.
			// This allows the player to flag a reload interrupt by firing
			// at any point from now on. It'll only be honoured at the
			// appropriate time, though.
			DelayFiring(0, true);

			// Go into load shell state after intro animation has finished.
			SetNextIdleTime(FRINESI_RELOAD_START_DELAY, true);
			return NextReloadState(currentState, RELOAD_LOAD_SHELL);
		}

		case RELOAD_LOAD_SHELL:
		{
			// If we haven't finished whatever animation is currently playing, don't change state.
			if( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
			{
				return NextReloadState(currentState, RELOAD_LOAD_SHELL);
			}

			if ( ((currentState & RELOAD_FLAG_INTERRUPTED) && (currentState & RELOAD_FLAG_LOADED_ONCE)) ||
				 m_iClip >= WeaponAttributes().Ammo.MaxClip ||
				 m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] < 1 )
			{
				// Reloading has finished. Do a pump and delay any further activity until it's finished.
				m_flNextReloadSoundTime = 0.0f;
				m_flNextPumpSoundTime = gpGlobals->time + FRINESI_PUMP_SOUND_OFFSET_WHEN_RELOADING;
				SendWeaponAnim(FRINESI_PUMP);
				DelayPendingActions(FRINESI_PUMP_DELAY, true);

				return NextReloadState(0, RELOAD_IDLE);
			}

			m_flNextReloadSoundTime = gpGlobals->time + FRINESI_RELOAD_SOUND_OFFSET;
			SendWeaponAnim(FRINESI_RELOAD);

			// Go into the increment clip state once this animation has finished.
			SetNextIdleTime(FRINESI_SHELL_RELOAD_DELAY, true);
			return NextReloadState(currentState, RELOAD_INCREMENT_CLIP);
		}

		case RELOAD_INCREMENT_CLIP:
		{
			if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] > 0 )
			{
				++m_iClip;
				--m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType];
			}

			return NextReloadState(currentState, RELOAD_LOAD_SHELL | RELOAD_FLAG_LOADED_ONCE);
		}

		default:
		{
			return NextReloadState(0, RELOAD_IDLE);
		}
	};
}

void CWeaponFrinesi::PlayPumpSound()
{
	EMIT_SOUND_DYN(ENT(m_pPlayer->pev),
					   CHAN_ITEM,
					   FRINESI_COCK_SOUND,
					   0.9f,
					   ATTN_NORM,
					   0,
					   UTIL_SharedRandomLong(m_pPlayer->random_seed, 98, 101));
}

bool CWeaponFrinesi::ReadPredictionData(const weapon_data_t* from)
{
	if ( !CGenericHitscanWeapon::ReadPredictionData(from) )
	{
		return false;
	}

	m_flNextPumpSoundTime = from->fuser1;
	m_flNextReloadSoundTime = from->fuser2;
	return true;
}

bool CWeaponFrinesi::WritePredictionData(weapon_data_t* to)
{
	if ( !CGenericHitscanWeapon::WritePredictionData(to) )
	{
		return false;
	}

	to->fuser1 = m_flNextPumpSoundTime;
	to->fuser2 = m_flNextReloadSoundTime;
	return true;
}

const WeaponAtts::WACollection& CWeaponFrinesi::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponFrinesi>();
}

#ifndef CLIENT_DLL
TYPEDESCRIPTION	CWeaponFrinesi::m_SaveData[] =
{
	DEFINE_FIELD(CWeaponFrinesi, m_flNextPumpSoundTime, FIELD_FLOAT),
	DEFINE_FIELD(CWeaponFrinesi, m_flNextReloadSoundTime, FIELD_FLOAT)
};

IMPLEMENT_SAVERESTORE(CWeaponFrinesi, CGenericHitscanWeapon)

float CWeaponFrinesi::Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const
{
	return static_cast<float>(WeaponAttributes().Core.SwitchWeight) / static_cast<float>(WeaponPref_Max);
}

void CWeaponFrinesi::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
	static constexpr float SECONDARY_FIRE_PROXIMITY = 700.0f;

	fightStyle.RandomizeAimAtHead(60);
	int chanceOfSecondaryFire = 20;

	// If we're able to determine that the enemy is near enough,
	// increase the chance of using secondary fire.

	CBaseBot* bot = fightStyle.GetOwner();
	CBaseEntity* enemy = bot->GetEnemy();

	if ( enemy && (enemy->pev->origin - bot->pev->origin).Length() <= SECONDARY_FIRE_PROXIMITY )
	{
		chanceOfSecondaryFire = 90;
	}

	fightStyle.RandomizeSecondaryFire(chanceOfSecondaryFire);
	fightStyle.SetNextShootTime(fightStyle.GetSecondaryFire() ? m_flNextSecondaryAttack : m_flNextPrimaryAttack, 0.0f, 1.0f);
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFrinesi>()
	{
		return ::StaticWeaponAttributes;
	}
}

class CAmmoFrinesi : public CGenericAmmo
{
public:
	CAmmoFrinesi()
		: CGenericAmmo("models/weapon_frinesi/w_ammo_shotgun.mdl", Ammo_Frinesi)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_frinesi, CAmmoFrinesi)
LINK_ENTITY_TO_CLASS(ammo_shotgun, CAmmoFrinesi)	// For Nightfire compatibility

#ifdef AFTERBURNER_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known ammo to this one.
LINK_ENTITY_TO_CLASS(ammo_commando, CAmmoFrinesi)
LINK_ENTITY_TO_CLASS(ammo_mini, CAmmoFrinesi)
#endif
