#include "weapon_l96a1.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_l96a1_atts.h"
#include "EnginePublicAPI/weaponinfo.h"

#ifndef CLIENT_DLL
#include "bot.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_l96a1, CWeaponL96A1)

CWeaponL96A1::CWeaponL96A1() :
	CGenericWeapon(),
	m_iZoomLevel(0)
{
	AddMechanicByAttributeIndex<WeaponAtts::WAHitscanAttack>(ATTACKMODE_UNSCOPED, m_AttackUnscoped);
	AddMechanicByAttributeIndex<WeaponAtts::WAHitscanAttack>(ATTACKMODE_SCOPED, m_AttackScoped);

	SetPrimaryAttackMechanic(m_AttackUnscoped);
}

const WeaponAtts::WACollection& CWeaponL96A1::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponL96A1>();
}

void CWeaponL96A1::Precache()
{
	CGenericWeapon::Precache();

	PRECACHE_SOUND(L96A1_ZOOM_IN_SOUND);
	PRECACHE_SOUND(L96A1_ZOOM_OUT_SOUND);
}

void CWeaponL96A1::SecondaryAttack()
{
	if ( !m_pPlayer )
	{
		SetZoomLevel(0);
		return;
	}

	SetZoomLevel(m_iZoomLevel + 1);
	PlayZoomSound();
	DelayPendingActions(L96A1_ZOOM_TOGGLE_DELAY);
}

void CWeaponL96A1::Reload()
{
	if ( CanReload() )
	{
		SetZoomLevel(0);
		PlayZoomSound();
	}

	CGenericWeapon::Reload();
}

void CWeaponL96A1::Holster(int skiplocal)
{
	CGenericWeapon::Holster(skiplocal);
	SetZoomLevel(0);
}

void CWeaponL96A1::WeaponTick()
{
	if ( m_iZoomLevel > 0 )
	{
		m_pPlayer->pev->maxspeed *= SCOPE_MOVE_SPEED_FACTOR;
	}
}

void CWeaponL96A1::SetZoomLevel(uint32_t level)
{
	if ( level >= L96A1_ZOOM_LEVEL_COUNT )
	{
		level = 0;
	}

	m_iZoomLevel = level;

	if ( m_pPlayer )
	{
		if ( m_iZoomLevel > 0 )
		{
			m_pPlayer->SetScreenOverlay(ScreenOverlays::OverlayId::Overlay_SniperScope);
			m_pPlayer->SetFOV(L96A1_ZOOM_LEVELS[m_iZoomLevel]);
			m_pPlayer->pev->effects |= EF_HIDEVIEWMODEL;
			SetPrimaryAttackMechanic(m_AttackScoped);
		}
		else
		{
			m_pPlayer->SetScreenOverlay(ScreenOverlays::OverlayId::Overlay_None);
			m_pPlayer->ResetFOV();
			m_pPlayer->pev->effects &= ~EF_HIDEVIEWMODEL;
			SetPrimaryAttackMechanic(m_AttackUnscoped);
		}
	}
}

void CWeaponL96A1::PlayZoomSound()
{
	EMIT_SOUND_DYN(
		ENT(m_pPlayer->pev),
		CHAN_ITEM,
		m_iZoomLevel > 0 ? L96A1_ZOOM_IN_SOUND : L96A1_ZOOM_OUT_SOUND,
		0.3f,
		ATTN_NORM,
		0,
		100);
}

bool CWeaponL96A1::ReadPredictionData(const weapon_data_t* from)
{
	if ( !CGenericWeapon::ReadPredictionData(from) )
	{
		return false;
	}

	SetZoomLevel(static_cast<uint32_t>(from->iuser1));

	return true;
}

bool CWeaponL96A1::WritePredictionData(weapon_data_t* to)
{
	if ( !CGenericWeapon::WritePredictionData(to) )
	{
		return false;
	}

	to->iuser1 = static_cast<int>(m_iZoomLevel);

	return true;
}

#ifndef CLIENT_DLL
float CWeaponL96A1::Bot_CalcDesireToUse(CBaseBot&, CBaseEntity&, float) const
{
	return static_cast<float>(WeaponAttributes().Core.SwitchWeight) / static_cast<float>(WeaponPref_Max);
}

void CWeaponL96A1::Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const
{
	fightStyle.SetSecondaryFire(false);
	fightStyle.RandomizeAimAtHead(70);
	fightStyle.SetNextShootTime(m_flNextPrimaryAttack, 0.2f, 0.7f);
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponL96A1>()
	{
		return ::StaticWeaponAttributes;
	}
}  // namespace WeaponAtts

class CAmmol96A1 : public CGenericAmmo
{
public:
	CAmmol96A1() :
		CGenericAmmo("models/weapon_l96a1/w_ammo_l96a1.mdl", Ammo_L96A1)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_l96a1, CAmmol96A1)
LINK_ENTITY_TO_CLASS(ammo_sniper, CAmmol96A1)  // For backwards-compatibility.
