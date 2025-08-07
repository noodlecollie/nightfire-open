#include "weapon_ronin.h"
#include "EnginePublicAPI/eiface.h"
#include "ammodefs.h"
#include "cbase.h"
#include "genericweapon.h"
#include "player.h"
#include "skill.h"
#include "gamerules.h"
#include "util.h"
#include "weapon_pref_weights.h"
#include "weapon_ronin_atts.h"
#include "eventConstructor/eventConstructor.h"
#include "MathLib/utils.h"
#include "EnginePublicAPI/weaponinfo.h"
#include "weaponmechanics/basemechanic.h"

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

// Uncomment to enable logging when state transitions occur
// #define LOG_STATE

#ifdef LOG_STATE
static const char* GetRoninStateString(uint32_t state)
{
	static const char* RONIN_STATE_STRINGS[] = {
#define LIST_ITEM(name) #name,
		RONIN_STATE_LIST
#undef LIST_ITEM
	};

	return state < SIZE_OF_ARRAY(RONIN_STATE_STRINGS) ? RONIN_STATE_STRINGS[state] : "UNKNOWN";
}
#endif  // LOG_STATE

LINK_ENTITY_TO_CLASS(weapon_ronin, CWeaponRonin);

const vec3_t RONIN_TURRET_MINS = {-14, -12, 0};
const vec3_t RONIN_TURRET_MAXS = {14, 12, 16};

CWeaponRonin::CWeaponRonin() :
	CGenericWeapon()
{
	AddMechanicByAttributeIndex<WeaponAtts::WAProjectileAttack>(VRONIN_ATTACKMODE_TOSS, m_ThrowMechanic);
	AddMechanicByAttributeIndex<WeaponAtts::WAEventAttack>(VRONIN_ATTACKMODE_PLACE, m_PlaceMechanic);
	AddMechanicByAttributeIndex<WeaponAtts::WAEventAttack>(VRONIN_ATTACKMODE_DEPLOY, m_DeployMechanic);
	AddMechanicByAttributeIndex<WeaponAtts::WAEventAttack>(VRONIN_ATTACKMODE_DETONATE, m_DetonateMechanic);

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

	m_DetonateMechanic->SetCallback(
		[this](WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step)
		{
			return DetonateTurret(mechanic, step);
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

void CWeaponRonin::Spawn()
{
	CGenericWeapon::Spawn();

	// This should always be the default state on spawn.
	// No need to call UpdateState() just yet.
	m_State = State::RONIN_HELD;
}

BOOL CWeaponRonin::IsUseable()
{
	return CanDeploy();
}

BOOL CWeaponRonin::CanDeploy()
{
	UpdateState();
	return CGenericWeapon::CanDeploy();
}

BOOL CWeaponRonin::Deploy()
{
	const BOOL ret = CGenericWeapon::Deploy();

	if ( ret && !m_InStateUpdate )
	{
		UpdateState();
	}

	return ret;
}

void CWeaponRonin::Drop()
{
#ifndef CLIENT_DLL
	DestroyTurret();
#endif

	CGenericWeapon::Drop();
}

int CWeaponRonin::AddToPlayer(CBasePlayer* pPlayer)
{
	if ( m_State != State::RONIN_HELD )
	{
		return FALSE;
	}

	if ( !CGenericWeapon::AddToPlayer(pPlayer) )
	{
		return FALSE;
	}

	UpdateState(State::RONIN_HELD);
	return TRUE;
}

int CWeaponRonin::AddDuplicate(CBasePlayerItem* pOriginal)
{
#ifndef CLIENT_DLL
	CWeaponRonin* playerWeapon = dynamic_cast<CWeaponRonin*>(pOriginal);

	// Don't allow pickup unless we have no Ronin at all.
	if ( playerWeapon && playerWeapon->m_State != State::RONIN_GONE )
	{
		return FALSE;
	}
#endif

	return CGenericWeapon::AddDuplicate(pOriginal);
}

void CWeaponRonin::DetachFromPlayer(CBasePlayer* pPlayer)
{
#ifndef CLIENT_DLL
	DestroyTurret();
#endif

	CGenericWeapon::DetachFromPlayer(pPlayer);
}

// WeaponTick() is only run if the weapon can attack.
// ItemPostFrame() is always run, and we need to manage
// state independently of whether we can attack,
// so we use that instead.
void CWeaponRonin::ItemPostFrame()
{
	// Cache this before we call into the base class,
	// as it might change it.
	const bool weaponShouldIdle = m_flTimeWeaponIdle <= UTIL_WeaponTimeBase();

	CGenericWeapon::ItemPostFrame();
	UpdateState();

	// Decide if we need to retire the weapon. This check is to
	// allow animations like the post-detonation hand-dusting to
	// play before the weapon is put away.
	// Either we will be in a deploying state, which means the
	// turret is in the process of unfolding, or we will have no
	// more ammo, in order to signify that the weapon is no
	// longer usable. If we are in either of these situations,
	// the idle time represents the point at which we should
	// retire the weapon.
	const bool shouldRetireWhenIdle =
		m_State == State::RONIN_DEPLOYING || AmmoLeft(WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary) < 1;

	if ( weaponShouldIdle && shouldRetireWhenIdle && IsActiveItem() )
	{
		RetireWeapon();
	}
}

bool CWeaponRonin::ReadPredictionData(const weapon_data_t* from)
{
	if ( !CGenericWeapon::ReadPredictionData(from) )
	{
		return false;
	}

	m_State = static_cast<State>(from->iuser1);
	return true;
}

bool CWeaponRonin::WritePredictionData(weapon_data_t* to)
{
	if ( !CGenericWeapon::WritePredictionData(to) )
	{
		return false;
	}

	to->iuser1 = static_cast<int>(m_State);
	return true;
}

bool CWeaponRonin::ThrowTurret(const WeaponMechanics::CProjectileMechanic& mechanic)
{
	// Sanity:
	if ( m_State != State::RONIN_HELD )
	{
		return false;
	}

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

	UpdateState();

	return true;
}

WeaponMechanics::InvocationResult CWeaponRonin::PlaceTurret(
	WeaponMechanics::CDelegatedMechanic& mechanic,
	uint32_t step)
{
	// Sanity:
	if ( m_State != State::RONIN_HELD )
	{
		return WeaponMechanics::InvocationResult::Rejected(mechanic);
	}

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

		UpdateState();

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
	// Sanity:
	if ( m_State != State::RONIN_THROWN )
	{
		return WeaponMechanics::InvocationResult::Rejected(mechanic);
	}

	if ( step == 0 )
	{
#ifndef CLIENT_DLL
		ActivateThrownTurret();
#endif

		SendEvent(mechanic);

		const float delay = 1.0f / mechanic.GetAttackMode()->AttackRate;
		DelayFiring(delay);

		// TODO: We should actually have a setting in the generic weapon class
		// to specify whether the weapon should play idle animations.
		SetNextIdleTime(1000.0f, true);

		return WeaponMechanics::InvocationResult::Complete(mechanic);
	}
	else
	{
		return WeaponMechanics::InvocationResult::Rejected(mechanic);
	}
}

WeaponMechanics::InvocationResult CWeaponRonin::DetonateTurret(
	WeaponMechanics::CDelegatedMechanic& mechanic,
	uint32_t step)
{
	// Sanity:
	if ( m_State != State::RONIN_THROWN )
	{
		return WeaponMechanics::InvocationResult::Rejected(mechanic);
	}

	if ( step == 0 )
	{
#ifndef CLIENT_DLL
		DetonateThrownTurret();
#endif

		SendEvent(mechanic);

		const float delay = 1.0f / mechanic.GetAttackMode()->AttackRate;
		DelayFiring(delay);

		// TODO: We should actually have a setting in the generic weapon class
		// to specify whether the weapon should play idle animations.
		SetNextIdleTime(1000.0f, true);

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

// Holding Ronin in hands => no turret is out
bool CWeaponRonin::IsHoldingRonin() const
{
	return m_State == State::RONIN_HELD;
}

bool CWeaponRonin::HasThrownButUndeployedRonin() const
{
	bool ret = m_State == State::RONIN_THROWN;

#ifndef CLIENT_DLL
	// If the turret state is TOSSED then we can return true.
	// If the state is UNDEPLOYED then the turret has been
	// deployed previously, which is not what we're checking here.
	ret = ret && m_Turret && GetTurret()->GetDeployState() == CNPCRoninTurret::DeployState::TOSSED;
#endif  // CLIENT_DLL

	return ret;
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
	const Vector2D delta2DDir = deltaToIdealLocation.Make2D().Normalize();
	const Vector2D perpendicularDelta2DDir(delta2DDir[1], -delta2DDir[0]);

	TraceResult tr {};
	bool success = false;

	// Note: A hull trace doesn't actually trace the exact bounds passed in.
	// It traces using a preset hull most appropriate to represent those bounds.
	// In our case, the hull selected is actually slightly larger than the
	// bounds (I think it's probably the player's crouching hull). If the
	// begin area for the trace intersects with anything, that means the
	// whole trace might end up being considered to be within a solid surface
	// if the end intersected as well, and this would cause everything to fail.
	// Here we try some different trace starting positions to try and avoid this
	// case.
	for ( size_t index = 0; index < 6 && !success; ++index )
	{
		Vector traceAttemptStart = traceBegin;

		if ( index != 0 )
		{
			if ( (index % 2) != 0 )
			{
				// Alternate moving the starting point back a bit.
				traceAttemptStart -= 8.0f * Vector(delta2DDir.x, delta2DDir.y, 0.0f);
			}

			// Perhaps also shift to the side
			if ( index == 2 || index == 3 )
			{
				traceAttemptStart -= 8.0f * Vector(perpendicularDelta2DDir.x, perpendicularDelta2DDir.y, 0.0f);
			}
			else if ( index == 4 || index == 5 )
			{
				traceAttemptStart += 8.0f * Vector(perpendicularDelta2DDir.x, perpendicularDelta2DDir.y, 0.0f);
			}
		}

		tr = TraceResult {};

		UTIL_TraceHull(
			traceAttemptStart,
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
		success = !tr.fAllSolid && tr.flFraction > 0.0f;
	}

	if ( tr.flFraction < 1.0f )
	{
		location = tr.vecEndPos;
	}

#ifndef CLIENT_DLL
	uint32_t startColour = GREEN;

	if ( tr.fStartSolid )
	{
		startColour = YELLOW;
	}

	DrawDebugBounds(debugMsg.get(), traceBegin, startColour);

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

void CWeaponRonin::Redeploy()
{
	Deploy();
	DelayPendingActions(0.75f);
}

void CWeaponRonin::UpdateState()
{
	SetNewState(DecideNewState(m_State));
}

void CWeaponRonin::UpdateState(State idealState)
{
	SetNewState(DecideNewState(idealState));
}

CWeaponRonin::State CWeaponRonin::DecideNewState(State baseState) const
{
	State newState = baseState;

	if ( newState == State::RONIN_GONE && AmmoLeft(WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary) > 0 )
	{
		// No turret, but we picked up ammo from somewhere.
		newState = State::RONIN_HELD;
	}

#ifndef CLIENT_DLL
	newState = ComputeTurretRelatedState(newState);
#endif

	return newState;
}

void CWeaponRonin::SetNewState(State state)
{
	if ( state == m_State )
	{
		return;
	}

	m_InStateUpdate = true;

#ifdef LOG_STATE
	ALERT(
		at_aiconsole,
		"CWeaponRonin: State %s -> %s\n",
		GetRoninStateString(static_cast<uint32_t>(m_State)),
		GetRoninStateString(static_cast<uint32_t>(state)));
#endif

	State oldState = m_State;
	m_State = state;

	RespondToNewState(oldState);

	m_InStateUpdate = false;
}

void CWeaponRonin::RespondToNewState(State oldState)
{
	UpdateAmmo();
	UpdateWeaponMechanics();

	if ( oldState == State::RONIN_GONE && m_State == State::RONIN_HELD )
	{
		DelayPendingActions(0.5f, true);
		Redeploy();
	}
}

void CWeaponRonin::UpdateAmmo()
{
	const int ammoAmount = AmmoLeft(WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary);
	const int intendedAmmo = (m_State == State::RONIN_DEPLOYED || m_State == State::RONIN_GONE) ? 0 : 1;

	if ( ammoAmount == intendedAmmo )
	{
		return;
	}

	if ( intendedAmmo > ammoAmount )
	{
		// It'd be nice to actually have an IncrementAmmo() function, but currently the
		// ammo pools don't seem to have an easily accessible max cap (or I've not been
		// thorough enough in my cursory check of the code). Given we know the max cap here,
		// we just do it this way.
		m_pPlayer->GiveAmmo(intendedAmmo - ammoAmount, Ammo_Ronin.AmmoName, Ammo_Ronin.MaxCarry);
	}
	else
	{
		DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary, ammoAmount - intendedAmmo);

		// TODO: Put this into generic weapon?
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}
}

void CWeaponRonin::UpdateWeaponMechanics()
{
	switch ( m_State )
	{
		case State::RONIN_THROWN:
		{
			SetPrimaryAttackMechanic(m_DeployMechanic);
			SetSecondaryAttackMechanic(m_DetonateMechanic);
			break;
		}

		case State::RONIN_HELD:
		{
			SetPrimaryAttackMechanic(m_ThrowMechanic);
			SetSecondaryAttackMechanic(m_PlaceMechanic);
			break;
		}

		default:
		{
			SetPrimaryAttackMechanic(nullptr);
			SetSecondaryAttackMechanic(nullptr);
			break;
		}
	}
}

#ifndef CLIENT_DLL
TYPEDESCRIPTION CWeaponRonin::m_SaveData[] = {
	DEFINE_FIELD(CWeaponRonin, m_State, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CWeaponRonin, CGenericWeapon)

bool CWeaponRonin::AddRoninWeaponToPlayer(CBasePlayer* player, TurretPickupType pickupType, CNPCRoninTurret* turret)
{
	if ( !player || player->pev->deadflag != DEAD_NO )
	{
		return false;
	}

	const WeaponAtts::WACollection& roninWeaponAtts = WeaponAtts::StaticWeaponAttributes<CWeaponRonin>();
	CWeaponRonin* playerWeapon = static_cast<CWeaponRonin*>(player->GetNamedItem(roninWeaponAtts.Core.Classname));

	if ( playerWeapon )
	{
		// Let the weapon handle the rest of the logic.
		return playerWeapon->PickUpTurret(pickupType, turret);
	}

	// The player doesn't have a ronin weapon, so add one.
	CWeaponRonin* weapon = GetClassPtr<CWeaponRonin>(nullptr);

	// Must spawn so that data is set up.
	weapon->Spawn();

	if ( !g_pGameRules->CanHavePlayerItem(player, weapon) )
	{
		UTIL_Remove(weapon);
		return false;
	}

	if ( !player->AddPlayerItem(weapon) )
	{
		UTIL_Remove(weapon);
		return false;
	}

	weapon->AttachToPlayer(player);
	EMIT_SOUND(ENT(player->pev), CHAN_ITEM, weapon->PickupSound(), 1, ATTN_NORM);

	if ( turret )
	{
		turret->SUB_UseTargets(player, USE_TOGGLE, 0);
	}

	return true;
}

CNPCRoninTurret* CWeaponRonin::GetTurret() const
{
	return m_Turret.StaticCast<CNPCRoninTurret>();
}

bool CWeaponRonin::PickUpTurret(TurretPickupType pickupType, CNPCRoninTurret* turret)
{
	// If the player is holding a Ronin, they can't pick up another.
	if ( IsHoldingRonin() )
	{
		return false;
	}

	// If the player has an active turret, they can't pick up another.
	if ( m_Turret && !GetTurret()->IsUndeployed() )
	{
		return false;
	}

	// The player can only pick up a turret if it's undeployed.
	if ( turret && !turret->IsUndeployed() )
	{
		return false;
	}

	// If the player has thrown a Ronin but hasn't deployed it,
	// they may only pick up a new turret if they +use it.
	// This makes sure their thrown turret is not disconnected
	// from them just by walking over a pickup.
	if ( HasThrownButUndeployedRonin() && pickupType != TurretPickupType::ON_USE )
	{
		return false;
	}

	// Disconnect our old turret from us :'(
	if ( m_Turret )
	{
		GetTurret()->pev->owner = nullptr;

		// For the moment, we just remove the old turret
		// If we left it and reset the owner, it would
		// make us stuck if we were standing in it.
		// Once we implement custom Ronin physics collisions,
		// we can leave the undeployed turret around for
		// someone else to pick up.
		UTIL_Remove(GetTurret());

		m_Turret = nullptr;
	}

	UpdateState(State::RONIN_HELD);

	if ( IsActiveItem() )
	{
		Redeploy();
	}
	else if ( g_pGameRules->FShouldSwitchWeapon(m_pPlayer, this) )
	{
		m_pPlayer->SwitchWeapon(this);
	}

	return true;
}

void CWeaponRonin::RefreshStateFromTurret(CNPCRoninTurret* turret)
{
	// We should never have this called with a turret we don't know about.
	if ( turret && static_cast<CBaseEntity*>(turret) != m_Turret.operator->() )
	{
		ALERT(
			at_warning,
			"CWeaponRonin::RefreshStateFromTurret: Ignoring request from turret that's different to ours! This should "
			"never happen!\n");

		return;
	}

	if ( !turret )
	{
		m_Turret = nullptr;
	}

	UpdateState();
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

CWeaponRonin::State CWeaponRonin::ComputeTurretRelatedState(State currentState) const
{
	CNPCRoninTurret* turret = GetTurret();

	if ( turret )
	{
		switch ( turret->GetDeployState() )
		{
			case CNPCRoninTurret::DeployState::TOSSED:
			case CNPCRoninTurret::DeployState::UNDEPLOYED:
			{
				return State::RONIN_THROWN;
			}

			// We return this if a Ronin is undeploying as well,
			// since otherwise we change our own state too early.
			// We only want to transition once the Ronin reaches
			// an undeployed state.
			case CNPCRoninTurret::DeployState::UNDEPLOYING:
			case CNPCRoninTurret::DeployState::DEPLOYED:
			{
				return State::RONIN_DEPLOYED;
			}

			case CNPCRoninTurret::DeployState::DEPLOYING:
			{
				return State::RONIN_DEPLOYING;
			}

			default:
			{
				// Should never be hit
				ASSERT(false);
				break;
			}
		}
	}
	else if (
		currentState == State::RONIN_THROWN || currentState == State::RONIN_DEPLOYING ||
		currentState == State::RONIN_DEPLOYED )
	{
		// We had a Ronin linked but now we don't.
		return State::RONIN_GONE;
	}

	return currentState;
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

	turret->Spawn();

	turret->pev->owner = m_pPlayer->edict();
	turret->pev->health = gSkillData.roninBaseHealth;
	turret->SetAllowsPickupWhenUndeployed(true);
	turret->SetAllowUndeployWithUseKey(true);
	turret->SetExplosionDamage(static_cast<int>(gSkillData.plrDmgRoninExplosion));

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

void CWeaponRonin::DetonateThrownTurret()
{
	if ( !m_Turret )
	{
		return;
	}

	CNPCRoninTurret* turret = m_Turret.StaticCast<CNPCRoninTurret>();

	turret->Explode();
	m_Turret = nullptr;
}

void CWeaponRonin::DestroyTurret()
{
	if ( m_Turret )
	{
		GetTurret()->pev->owner = nullptr;
		UTIL_Remove(GetTurret());
		m_Turret = nullptr;
	}
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
		CGenericAmmo("models/weapon_ronin/w_ammo_ronin.mdl", Ammo_Ronin)
	{
	}

	BOOL AddAmmo(CBaseEntity* pOther) override
	{
#ifndef CLIENT_DLL
		if ( !pOther || !pOther->IsPlayer() )
		{
			return CGenericAmmo::AddAmmo(pOther);
		}

		return CWeaponRonin::AddRoninWeaponToPlayer(
				   dynamic_cast<CBasePlayer*>(pOther),
				   CWeaponRonin::TurretPickupType::ON_TOUCH)
			? TRUE
			: FALSE;
#else
		return CGenericAmmo::AddAmmo(pOther);
#endif
	}
};

LINK_ENTITY_TO_CLASS(ammo_ronin, CAmmoRonin)
