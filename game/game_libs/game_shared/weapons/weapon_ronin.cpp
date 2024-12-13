#include "weapon_ronin.h"
#include "ammodefs.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_ronin_atts.h"
#include "eventConstructor/eventConstructor.h"
#include "MathLib/utils.h"

#ifndef CLIENT_DLL
#include <limits>
#include <algorithm>
#include "bot.h"
#include "npc/npc_ronin_turret.h"
#include "customGeometry/constructors/crosshair3DConstructor.h"
#include "customGeometry/messageWriter.h"

cvar_t debug_ronin_placement = CONSTRUCT_CVAR_T("debug_ronin_placement", 0, FCVAR_CHEAT);
#endif

LINK_ENTITY_TO_CLASS(weapon_ronin, CWeaponRonin);

// Sanity helpers:
#define ASSERT_HAS_PRI_AMMO() \
	ASSERT(m_iPrimaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] == RONIN_HOLD_AMMO)
#define ASSERT_HAS_NO_PRI_AMMO() ASSERT(m_iPrimaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] == 0)
#define ASSERT_HAS_SEC_AMMO() \
	ASSERT(m_iSecondaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] == RONIN_HOLD_AMMO)
#define ASSERT_HAS_NO_SEC_AMMO() ASSERT(m_iSecondaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] == 0)

CWeaponRonin::CWeaponRonin() :
	CGenericWeapon()
{
	AddMechanicByAttributeIndex<WeaponAtts::WAProjectileAttack>(VRONIN_ATTACKMODE_TOSS, m_ThrowMechanic);
	AddMechanicByAttributeIndex<WeaponAtts::WAEventAttack>(VRONIN_ATTACKMODE_PLACE, m_PlaceMechanic);
	AddMechanicByAttributeIndex<WeaponAtts::WAEventAttack>(VRONIN_ATTACKMODE_DEPLOY, m_DeployMechanic);

	m_ThrowMechanic->SetCreateProjectileCallback(
		[this](WeaponMechanics::CProjectileMechanic& mechanic)
		{
			// TODO: Do we need to allow this to return a value
			// and cancel the entire action? If spawning the
			// turret fails, I don't think the view model animation
			// will be cancelled.
			ThrowTurret(mechanic);
		});

	m_PlaceMechanic->SetCallback(
		[this](WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step)
		{
			return PlaceTurret(mechanic, step);
		});

	m_DeployMechanic->SetCallback(
		[this](WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step)
		{
			return ActivateTurret(mechanic, step);
		});

	SetPrimaryAttackMechanic(m_ThrowMechanic);
	SetSecondaryAttackMechanic(m_PlaceMechanic);

	// Base the view model animations on whatever mechanic is set to be primary.
	SetViewModelAnimationSource(WeaponAtts::AttackMode::Primary);
}

const WeaponAtts::WACollection& CWeaponRonin::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponRonin>();
}

void CWeaponRonin::ThrowTurret(const WeaponMechanics::CProjectileMechanic& mechanic)
{
	Vector forward;
	AngleVectors(mechanic.GetProjectileLaunchAngles(0.0f), forward, nullptr, nullptr);

	Vector spawnLocation;

	if ( !SelectRoninThrowSpawnLocation(forward, spawnLocation) )
	{
		ALERT(
			at_aiconsole,
			"Failed to find spawn location for Ronin near player origin (%f, %f, %f)\n",
			m_pPlayer->pev->origin[VEC3_X],
			m_pPlayer->pev->origin[VEC3_Y],
			m_pPlayer->pev->origin[VEC3_Z]);

		return;
	}

#ifndef CLIENT_DLL
	LaunchThrownTurret(forward, spawnLocation);
#endif

	PostCreateTurret();
}

WeaponMechanics::InvocationResult CWeaponRonin::PlaceTurret(
	WeaponMechanics::CDelegatedMechanic& mechanic,
	uint32_t step)
{
	if ( step == 0 )
	{
		// Play animation and delay for a bit.
		SendEvent(mechanic);

		const float delay = 1.0f / mechanic.GetAttackMode()->AttackRate;
		DelayFiring(delay);
		SetNextIdleTime(delay + 1.0f);
		return WeaponMechanics::InvocationResult::Incomplete(mechanic, delay);
	}
	else if ( step == 1 )
	{
		Vector location;

		if ( !SelectRoninPlaceSpawnLocation(location) )
		{
			ALERT(
				at_aiconsole,
				"Failed to find placement location for Ronin near player origin (%f, %f, %f)\n",
				m_pPlayer->pev->origin[VEC3_X],
				m_pPlayer->pev->origin[VEC3_Y],
				m_pPlayer->pev->origin[VEC3_Z]);

			// TODO: Reset view model

			return WeaponMechanics::InvocationResult::Rejected(mechanic);
		}

#ifndef CLIENT_DLL
		PlaceTurret(location);
#endif

		// We have to do this here, because this attack mechanic
		// won't do it for us.
		DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary, RONIN_HOLD_AMMO);

		// TODO: Play draw animation for remote

		PostCreateTurret();
		return WeaponMechanics::InvocationResult::Complete(mechanic);
	}
	else
	{
		return WeaponMechanics::InvocationResult::Rejected(mechanic);
	}
}

WeaponMechanics::InvocationResult CWeaponRonin::ActivateTurret(
	WeaponMechanics::CDelegatedMechanic& mechanic,
	uint32_t step)
{
	if ( step == 0 )
	{
#ifndef CLIENT_DLL
		ActivateThrownTurret();
#endif

		SendEvent(mechanic);

		const float delay = 1.0f / mechanic.GetAttackMode()->AttackRate;
		DelayFiring(delay);
		SetNextIdleTime(delay + 1.0f);
		return WeaponMechanics::InvocationResult::Incomplete(mechanic, delay);
	}
	else if ( step == 1 )
	{
		ASSERT_HAS_NO_PRI_AMMO();
		ASSERT_HAS_SEC_AMMO();

		DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool::Secondary, 1);

		ASSERT_HAS_NO_PRI_AMMO();
		ASSERT_HAS_NO_SEC_AMMO();

		RetireWeapon();
		return WeaponMechanics::InvocationResult::Complete(mechanic);
	}
	else
	{
		return WeaponMechanics::InvocationResult::Rejected(mechanic);
	}
}

void CWeaponRonin::SendEvent(const WeaponMechanics::CDelegatedMechanic& mechanic)
{
	if ( mechanic.GetEventIndex() >= 0 )
	{
		using namespace EventConstructor;
		CEventConstructor event;

		event << Flags(DefaultEventFlags()) << Invoker(m_pPlayer->edict())
			  << EventIndex(static_cast<unsigned short>(mechanic.GetEventIndex())) << IntParam1(m_pPlayer->random_seed);

		event.Send();
	}
}

bool CWeaponRonin::IsHoldingRonin() const
{
	return m_pPlayer->AmmoInventory(m_iPrimaryAmmoType) > 0;
}

bool CWeaponRonin::HasThrownButUndeployedRonin() const
{
	return m_pPlayer->AmmoInventory(m_iSecondaryAmmoType) > 0;
}

bool CWeaponRonin::SelectRoninThrowSpawnLocation(const Vector& forward, Vector& outLocation) const
{
	const Vector eyePos = Vector(m_pPlayer->pev->origin) + Vector(m_pPlayer->pev->view_ofs);
	return FitRoninAtLocation(eyePos, forward * PROJECTILE_SPAWN_DIST_IN_FRONT_OF_PLAYER, outLocation);
}

bool CWeaponRonin::SelectRoninPlaceSpawnLocation(Vector& outLocation) const
{
	const Vector eyePos = Vector(m_pPlayer->pev->origin) + Vector(m_pPlayer->pev->view_ofs);

	Vector viewVec;
	AngleVectors(m_pPlayer->pev->v_angle, viewVec, nullptr, nullptr);

	// Do an initial trace to find an ideal spawn pos,
	// then adjust later for the bounding box.
	Vector idealSpawnPos = eyePos + (viewVec * TURRET_PLACE_HDIST);

	TraceResult tr {};
	UTIL_TraceLine(eyePos, idealSpawnPos, dont_ignore_monsters, m_pPlayer->edict(), &tr);
	idealSpawnPos = eyePos + (tr.flFraction * (idealSpawnPos - eyePos));

	return FitRoninAtLocation(eyePos, idealSpawnPos - eyePos, outLocation);
}

bool CWeaponRonin::FitRoninAtLocation(const Vector& traceBegin, const Vector& deltaToIdealLocation, Vector& outLocation)
	const
{
	Vector location = traceBegin + deltaToIdealLocation;

	{
		TraceResult tr {};
		UTIL_TraceLine(traceBegin, location, dont_ignore_monsters, m_pPlayer->edict(), &tr);

		if ( tr.flFraction < 1.0f )
		{
			location = traceBegin + (tr.flFraction * deltaToIdealLocation);
		}
	}

	// Check all axes to see if the bounding box actually fits in this location.
	for ( size_t axis = 0; axis < 3; ++axis )
	{
		bool haveAdjustedOnThisAxis = false;

		for ( size_t iteration = 0; iteration < 2; ++iteration )
		{
			const int polarity = iteration == 0 ? 1 : -1;

			Vector basis;
			basis[axis] = static_cast<float>(polarity);

			const float scale = iteration == 0 ? RONIN_TURRET_MAXS[axis] : -RONIN_TURRET_MINS[axis];
			const Vector delta = basis * scale;

			TraceResult tr {};
			UTIL_TraceLine(traceBegin, location + delta, dont_ignore_monsters, m_pPlayer->edict(), &tr);

			if ( tr.flFraction >= 1.0f && !tr.fAllSolid && !tr.fStartSolid )
			{
				continue;
			}

			if ( haveAdjustedOnThisAxis )
			{
				// A collision + adjustment in one direction resulted in
				// another collision in the other direction, so there
				// is not space to spawn the Ronin here.
				return false;
			}

			// We hit something, so adjust our location accordingly.
			// Recede a little more from the surface in addition to the
			// computed contact point.
			location = Vector(tr.vecEndPos) - delta - basis;
			haveAdjustedOnThisAxis = true;
		}
	}

	outLocation = location;

#ifndef CLIENT_DLL
	DrawDebugCrosshair(location);
#endif

	return true;
}

void CWeaponRonin::PostCreateTurret()
{
	ASSERT_HAS_NO_PRI_AMMO();
	ASSERT_HAS_NO_SEC_AMMO();

	const WeaponAtts::WACollection& roninWeaponAtts = WeaponAtts::StaticWeaponAttributes<CWeaponRonin>();
	const CAmmoDef* secAmmo = roninWeaponAtts.Ammo.SecondaryAmmo;
	m_pPlayer->GiveAmmo(RONIN_HOLD_AMMO, secAmmo->AmmoName, secAmmo->MaxCarry);

	ASSERT_HAS_NO_PRI_AMMO();
	ASSERT_HAS_SEC_AMMO();

	SetPrimaryAttackMechanic(m_DeployMechanic);
	SetSecondaryAttackMechanic(nullptr);  // TODO: Detonate
}

#ifndef CLIENT_DLL
CNPCRoninTurret* CWeaponRonin::GetTurret() const
{
	return m_Turret.StaticCast<CNPCRoninTurret>();
}

bool CWeaponRonin::PickUpUndeployedTurret(CNPCRoninTurret* turret, TurretPickupType pickupType)
{
	if ( !turret )
	{
		return false;
	}

	const WeaponAtts::WACollection& roninWeaponAtts = WeaponAtts::StaticWeaponAttributes<CWeaponRonin>();
	const CAmmoDef* priAmmo = roninWeaponAtts.Ammo.PrimaryAmmo;

	// If the player is holding a Ronin, they can't pick up another.
	if ( IsHoldingRonin() )
	{
		return false;
	}

	// The player can only pick up a turret if it's undeployed.
	if ( !turret->IsUndeployed() )
	{
		return false;
	}

	const bool newTurretIsOwnTurret = m_Turret && m_Turret.StaticCast<CNPCRoninTurret>() == turret;

	// The player can't pick up someone else's turret while they still have one active.
	if ( m_Turret && !newTurretIsOwnTurret && !GetTurret()->IsUndeployed() )
	{
		return false;
	}

	// If the player has thrown a Ronin but hasn't deployed it,
	// they may only pick up a new turret if they +use it.
	if ( HasThrownButUndeployedRonin() && pickupType != TurretPickupType::ON_USE )
	{
		return false;
	}

	// Set weapon state to throwable by topping up primary ammo.
	ASSERT_HAS_NO_PRI_AMMO();
	m_pPlayer->GiveAmmo(m_ThrowMechanic->GetAmmoBasedAttackMode()->AmmoDecrement, priAmmo->AmmoName, priAmmo->MaxCarry);
	ASSERT_HAS_PRI_AMMO();

	// If we had a thrown but undeployed turret, and are picking up a new one,
	// disconnect the old one from us.
	if ( m_Turret && !newTurretIsOwnTurret )
	{
		GetTurret()->pev->owner = nullptr;
	}

	// Make sure we specify that we no longer have an undeployed turret out.
	if ( HasThrownButUndeployedRonin() )
	{
		DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool::Secondary, RONIN_HOLD_AMMO);
	}

	ASSERT_HAS_NO_SEC_AMMO();
	m_Turret = nullptr;

	SetPrimaryAttackMechanic(m_ThrowMechanic);
	SetSecondaryAttackMechanic(m_PlaceMechanic);

	if ( m_pPlayer->m_pActiveItem == this )
	{
		// Replay deploy amimation.
		Deploy();
		DelayPendingActions(0.75f);
	}
	else if ( g_pGameRules->FShouldSwitchWeapon(m_pPlayer, this) )
	{
		m_pPlayer->SwitchWeapon(this);
	}

	return true;
}

float CWeaponRonin::Bot_CalcDesireToUse(CBaseBot&, CBaseEntity&, float) const
{
	// TODO
	return 0.0f;
}

void CWeaponRonin::Bot_SetFightStyle(CBaseBotFightStyle&) const
{
	// TODO
}

void CWeaponRonin::LaunchThrownTurret(const Vector& forward, const Vector& spawnLocation)
{
	CNPCRoninTurret* turret = CreateTurret();

	Vector velocity = Vector(m_pPlayer->pev->velocity) + (200.0f * forward) + (150.0f * Vector(0, 0, 1));

	const float rotDir = RANDOM_LONG(0, 1) ? 1.0f : -1.0f;
	Vector avelocity(0, RANDOM_FLOAT(360.0f * rotDir, 540.0f * rotDir), 0);

	turret->StartToss(spawnLocation, velocity, avelocity);
}

void CWeaponRonin::PlaceTurret(const Vector& spawnLocation)
{
	CNPCRoninTurret* turret = CreateTurret();
	turret->StartToss(spawnLocation, Vector(), Vector());
}

CNPCRoninTurret* CWeaponRonin::CreateTurret()
{
	CNPCRoninTurret* turret = GetClassPtr<CNPCRoninTurret>(nullptr);
	m_Turret = turret;
	turret->pev->owner = m_pPlayer->edict();
	turret->Spawn();
	turret->SetAllowsPickupWhenUndeployed(true);

	return turret;
}

void CWeaponRonin::ActivateThrownTurret()
{
	if ( !m_Turret )
	{
		return;
	}

	CNPCRoninTurret* roninTurret = m_Turret.StaticCast<CNPCRoninTurret>();
	roninTurret->DeployNow();
}

void CWeaponRonin::DrawDebugCrosshair(const Vector& location, float scale) const
{
	if ( debug_ronin_placement.value == 0.0f )
	{
		return;
	}

	CustomGeometry::CCrosshair3DConstructor constructor;
	constructor.SetOrigin(location);
	constructor.SetScale(scale);

	CustomGeometry::GeometryItemPtr_t geom = constructor.Construct();
	geom->SetColour(0xFF3FFBFF);

	CustomGeometry::CMessageWriter writer(CustomGeometry::Category::RoninDebugging);
	writer.WriteClearMessage();
	writer.WriteMessage(*geom);
}
#endif

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponRonin>()
	{
		return ::StaticWeaponAttributes;
	}
}  // namespace WeaponAtts

class CAmmoRonin : public CGenericAmmo
{
public:
	CAmmoRonin() :
		CGenericAmmo("models/weapon_ronin/w_ammo_ronin.mdl", Ammo_RoninThrow)
	{
	}
};

LINK_ENTITY_TO_CLASS(ammo_ronin, CAmmoRonin)
