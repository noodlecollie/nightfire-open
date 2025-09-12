#include "weapon_p99.h"
#include "weaponregistry.h"
#include "EnginePublicAPI/weaponinfo.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_p99_atts.h"

#ifndef CLIENT_DLL
#include "bot.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_p99, CWeaponP99)
LINK_ENTITY_TO_CLASS(weapon_pp9, CWeaponP99)  // NF devs called it "PP9" for some reason.

#ifdef NFOPEN_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known weapons to this one.
LINK_ENTITY_TO_CLASS(weapon_kowloon, CWeaponP99)
#endif

CWeaponP99::CWeaponP99() :
	CGenericWeapon(),
	m_bSilenced(false)
{
	AddMechanicByAttributeIndex<WeaponAtts::WAHitscanAttack>(P99_ATTACKMODE_SILENCED, m_SilencedAttack);
	AddMechanicByAttributeIndex<WeaponAtts::WAHitscanAttack>(P99_ATTACKMODE_UNSILENCED, m_UnsilencedAttack);

	SetPrimaryAttackMechanic(m_UnsilencedAttack);
}

const WeaponAtts::WACollection& CWeaponP99::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponP99>();
}

void CWeaponP99::SecondaryAttack()
{
	const int anim = m_bSilenced ? P99_REMOVE_SILENCER : P99_ADD_SILENCER;

	// We must animate using the silenced body group in either case, so we can see the silencer.
	SendWeaponAnim(anim, P99_BODY_SILENCED);

	m_bSilenced = !m_bSilenced;
	SetViewModelBody(m_bSilenced ? P99_BODY_SILENCED : P99_BODY_UNSILENCED);
	SetPrimaryAttackMechanic(m_bSilenced ? m_SilencedAttack : m_UnsilencedAttack);

	DelayPendingActions(ViewModelAnimationDuration(anim));
}

bool CWeaponP99::ReadPredictionData(const weapon_data_t* from)
{
	if ( !CGenericWeapon::ReadPredictionData(from) )
	{
		return false;
	}

	const bool newSilencedState = from->iuser1 == 1;
	if ( m_bSilenced != newSilencedState )
	{
		m_bSilenced = newSilencedState;

		// The silenced state has changed, so immediately update the viewmodel body to be accurate.
		SetViewModelBody(m_bSilenced ? P99_BODY_SILENCED : P99_BODY_UNSILENCED, true);
		SetPrimaryAttackMechanic(m_bSilenced ? m_SilencedAttack : m_UnsilencedAttack);
	}

	return true;
}

bool CWeaponP99::WritePredictionData(weapon_data_t* to)
{
	if ( !CGenericWeapon::WritePredictionData(to) )
	{
		return false;
	}

	to->iuser1 = m_bSilenced ? 1 : 0;
	return true;
}

#ifndef CLIENT_DLL
TYPEDESCRIPTION CWeaponP99::m_SaveData[] = {
	DEFINE_FIELD(CWeaponP99, m_bSilenced, FIELD_BOOLEAN),
};

IMPLEMENT_SAVERESTORE(CWeaponP99, CGenericWeapon)
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponP99>()
	{
		return ::StaticWeaponAttributes;
	}
}  // namespace WeaponAtts

class CAmmoP99 : public CGenericAmmo
{
public:
	CAmmoP99() :
		CGenericAmmo("models/weapon_p99/w_ammo_p99.mdl", Ammo_P99)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_p99, CAmmoP99)

#ifdef NFOPEN_GAMEPLAY_PLACEHOLDERS
// To make weapons less sparse for testing, map some other known ammo to this one.
LINK_ENTITY_TO_CLASS(ammo_kowloon, CAmmoP99)
#endif
