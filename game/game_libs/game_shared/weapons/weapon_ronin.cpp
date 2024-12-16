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
#include "customGeometry/constructors/aabboxConstructor.h"
#include "customGeometry/messageWriter.h"

cvar_t debug_ronin_placement = CONSTRUCT_CVAR_T("debug_ronin_placement", 0, FCVAR_CHEAT);
cvar_t sv_ronin_place_fov = CONSTRUCT_CVAR_T("sv_ronin_place_fov", 170.0f, FCVAR_SERVER);
#endif

LINK_ENTITY_TO_CLASS(weapon_ronin, CWeaponRonin);

const vec3_t RONIN_TURRET_MINS = {-14, -12, 0};
const vec3_t RONIN_TURRET_MAXS = {14, 12, 16};

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
			return ThrowTurret(mechanic);
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

bool CWeaponRonin::ThrowTurret(const WeaponMechanics::CProjectileMechanic& mechanic)
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

		Redeploy();
		return false;
	}

#ifndef CLIENT_DLL
	LaunchThrownTurret(forward, spawnLocation);
#endif

	PostCreateTurret(false);
	return true;
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

			Redeploy();
			return WeaponMechanics::InvocationResult::Rejected(mechanic);
		}

#ifndef CLIENT_DLL
		PlaceTurret(location);
#endif

		PostCreateTurret(true);

		// Play the deploy animation again.
		Redeploy();

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

		DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool::Secondary, RONIN_HOLD_AMMO);

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
#ifndef CLIENT_DLL
	static constexpr uint32_t RED = 0xFF0000FF;
	static constexpr uint32_t YELLOW = 0xFFD800FF;
	static constexpr uint32_t GREEN = 0x00FF00FF;

	std::unique_ptr<CustomGeometry::CMessageWriter> debugMsg;

	if ( debug_ronin_placement.value != 0.0f )
	{
		debugMsg.reset(new CustomGeometry::CMessageWriter(CustomGeometry::Category::RoninDebugging));
		debugMsg->SetTargetClient(m_pPlayer);
		debugMsg->WriteClearMessage();
	}
#endif

	Vector location = traceBegin + deltaToIdealLocation;

	TraceResult tr {};
	UTIL_TraceHull(
		traceBegin,
		location,
		dont_ignore_monsters,
		RONIN_TURRET_MINS,
		RONIN_TURRET_MAXS,
		m_pPlayer->edict(),
		&tr);

	// Note that the trace is OK if it started solid - the hull could have been
	// intersecting with something around the player's head, for example, while
	// the actual end point is free. The bad case is if the trace was all solid,
	// which means that there was no free space anywhere.
	bool success = !tr.fAllSolid && tr.flFraction > 0.0f;

	if ( !success )
	{
		// Try again - we might have hit some awkward bit of brushwork.
		const Vector newStart = traceBegin - (4.0f * deltaToIdealLocation.Normalize());
		tr = TraceResult {};

		UTIL_TraceHull(
			newStart,
			location,
			dont_ignore_monsters,
			RONIN_TURRET_MINS,
			RONIN_TURRET_MAXS,
			m_pPlayer->edict(),
			&tr);

		success = !tr.fAllSolid && tr.flFraction > 0.0f;
	}

	if ( tr.flFraction < 1.0f )
	{
		location = tr.vecEndPos;
	}

#ifndef CLIENT_DLL
	DrawDebugBounds(debugMsg.get(), traceBegin, success ? YELLOW : RED);

	uint32_t endColour = GREEN;

	if ( !success )
	{
		endColour = RED;
	}
	else if ( tr.flFraction < 1.0f )
	{
		endColour = YELLOW;
	}

	DrawDebugBounds(debugMsg.get(), location, endColour);
#endif

	outLocation = location;
	return success;
}

void CWeaponRonin::PostCreateTurret(bool decrementPrimaryAmmo)
{
	// If this is not passed, it means that the primary ammo
	// will be decremented elsewhere (ie. in the weapon mechanic).
	if ( decrementPrimaryAmmo )
	{
		ASSERT_HAS_PRI_AMMO();
		DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary, RONIN_HOLD_AMMO);
		ASSERT_HAS_NO_PRI_AMMO();
	}

	ASSERT_HAS_NO_SEC_AMMO();

	const WeaponAtts::WACollection& roninWeaponAtts = WeaponAtts::StaticWeaponAttributes<CWeaponRonin>();
	const CAmmoDef* secAmmo = roninWeaponAtts.Ammo.SecondaryAmmo;
	m_pPlayer->GiveAmmo(RONIN_HOLD_AMMO, secAmmo->AmmoName, secAmmo->MaxCarry);

	ASSERT_HAS_SEC_AMMO();

	SetPrimaryAttackMechanic(m_DeployMechanic);
	SetSecondaryAttackMechanic(nullptr);  // TODO: Detonate
}

void CWeaponRonin::Redeploy()
{
	Deploy();
	DelayPendingActions(0.75f);
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
		Redeploy();
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

	Vector angles(m_pPlayer->pev->v_angle);
	angles[PITCH] = 0.0f;
	angles[ROLL] = 0.0f;

	float fov = sv_ronin_place_fov.value;

	if ( fov > 360.0f )
	{
		fov = 360.0f;
	}
	else if ( fov < 1.0f )
	{
		fov = 1.0f;
	}

	turret->Place(spawnLocation, angles, fov);
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

void CWeaponRonin::DrawDebugBounds(CustomGeometry::CMessageWriter* writer, const Vector& location, uint32_t colour)
	const
{
	if ( !writer )
	{
		return;
	}

	CustomGeometry::CCrosshair3DConstructor origin;
	origin.SetOrigin(location);
	origin.SetScale(4.0f);

	CustomGeometry::GeometryItemPtr_t geom = origin.Construct();
	geom->SetColour(colour);

	writer->WriteMessage(*geom);

	CustomGeometry::CAABBoxConstructor bbox;
	bbox.SetBounds(location + Vector(RONIN_TURRET_MINS), location + Vector(RONIN_TURRET_MAXS));

	geom = bbox.Construct();
	geom->SetColour(colour);

	writer->WriteMessage(*geom);
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
