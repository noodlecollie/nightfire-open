#include "weapon_ronin.h"
#include "ammodefs.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_ronin_atts.h"
#include "eventConstructor/eventConstructor.h"

#ifndef CLIENT_DLL
#include <limits>
#include <algorithm>
#include "bot.h"
#include "npc/npc_ronin_turret.h"
#include "MathLib/utils.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_ronin, CWeaponRonin);

CWeaponRonin::CWeaponRonin() :
	CGenericWeapon()
{
	AddMechanicByAttributeIndex<WeaponAtts::WAProjectileAttack>(VRONIN_ATTACKMODE_TOSS, m_ThrowMechanic);
	AddMechanicByAttributeIndex<WeaponAtts::WAEventAttack>(VRONIN_ATTACKMODE_DEPLOY, m_DeployMechanic);

	m_ThrowMechanic->SetCreateProjectileCallback(
		[this](WeaponMechanics::CProjectileMechanic& mechanic)
		{
			ThrowTurret(mechanic);
		});

	m_DeployMechanic->SetCallback(
		[this](WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step)
		{
			return ActivateTurret(mechanic, step);
		});

	SetPrimaryAttackMechanic(m_ThrowMechanic);

	// Base the view model animations on whatever mechanic is set to be primary.
	SetViewModelAnimationSource(WeaponAtts::AttackMode::Primary);
}

const WeaponAtts::WACollection& CWeaponRonin::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponRonin>();
}

void CWeaponRonin::ThrowTurret(const WeaponMechanics::CProjectileMechanic& mechanic)
{
	(void)mechanic;

#ifndef CLIENT_DLL
	LaunchThrownTurret(mechanic);
#endif

	uint8_t decrement = mechanic.GetAmmoBasedAttackMode()->AmmoDecrement;
	DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary, decrement);

	ASSERT(m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] == 0);

	const WeaponAtts::WACollection& roninWeaponAtts = WeaponAtts::StaticWeaponAttributes<CWeaponRonin>();
	const CAmmoDef* secAmmo = roninWeaponAtts.Ammo.SecondaryAmmo;
	m_pPlayer->GiveAmmo(1, secAmmo->AmmoName, secAmmo->MaxCarry);

	SetPrimaryAttackMechanic(m_DeployMechanic);
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

		SendDeployEvent(mechanic);

		const float delay = 1.0f / mechanic.GetAttackMode()->AttackRate;
		DelayFiring(delay);
		SetNextIdleTime(delay + 1.0f);
		return WeaponMechanics::InvocationResult::Incomplete(mechanic, delay);
	}
	else if ( step == 1 )
	{
		ASSERT(m_iSecondaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] == 1);
		DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool::Secondary, 1);
		ASSERT(m_iSecondaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] == 0);

		RetireWeapon();
		return WeaponMechanics::InvocationResult::Complete(mechanic);
	}
	else
	{
		return WeaponMechanics::InvocationResult::Rejected(mechanic);
	}
}

void CWeaponRonin::SendDeployEvent(const WeaponMechanics::CDelegatedMechanic& mechanic)
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
	ASSERT(m_iPrimaryAmmoType >= 0 && m_iSecondaryAmmoType >= 0);

	// If the player is holding a Ronin, they can't pick up another.
	if ( m_pPlayer->AmmoInventory(m_iPrimaryAmmoType) > 0 )
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
	if ( m_pPlayer->AmmoInventory(m_iSecondaryAmmoType) > 0 && pickupType != TurretPickupType::ON_USE )
	{
		return false;
	}

	// Set weapon state to throwable.
	ASSERT(m_iPrimaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] == 0);
	m_pPlayer->GiveAmmo(m_ThrowMechanic->GetAmmoBasedAttackMode()->AmmoDecrement, priAmmo->AmmoName, priAmmo->MaxCarry);

	ASSERT(
		m_iPrimaryAmmoType >= 0 &&
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] == m_ThrowMechanic->GetAmmoBasedAttackMode()->AmmoDecrement);

	ASSERT(m_iSecondaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] == 1);
	DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool::Secondary, 1);
	ASSERT(m_iSecondaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] == 0);

	// If we had an existing turret, disconnect it from us.
	if ( m_Turret && !newTurretIsOwnTurret )
	{
		GetTurret()->pev->owner = nullptr;
	}

	m_Turret = nullptr;
	SetPrimaryAttackMechanic(m_ThrowMechanic);

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

void CWeaponRonin::LaunchThrownTurret(const WeaponMechanics::CProjectileMechanic& mechanic)
{
	CNPCRoninTurret* turret = GetClassPtr<CNPCRoninTurret>(nullptr);
	m_Turret = turret;
	turret->pev->owner = m_pPlayer->edict();
	turret->Spawn();
	turret->SetAllowsPickupWhenUndeployed(true);

	Vector forward;
	AngleVectors(mechanic.GetProjectileLaunchAngles(0.0f), forward, nullptr, nullptr);

	Vector spawnLocation;

	if ( !SelectRoninSpawnLocation(*turret, forward, spawnLocation) )
	{
		// TODO: Handle this case properly.
		// We actually want to cancel the fire action.
		ALERT(at_aiconsole, "Failed to find spawn location for %s\n", STRING(turret->pev->classname));
		UTIL_Remove(turret);
		return;
	}

	Vector velocity = Vector(m_pPlayer->pev->velocity) + (200.0f * forward) + (150.0f * Vector(0, 0, 1));

	const float rotDir = RANDOM_LONG(0, 1) ? 1.0f : -1.0f;
	Vector avelocity(0, RANDOM_FLOAT(360.0f * rotDir, 540.0f * rotDir), 0);

	turret->StartToss(spawnLocation, velocity, avelocity);
}

bool CWeaponRonin::SelectRoninSpawnLocation(CNPCRoninTurret& turret, const Vector& forward, Vector& outLocation) const
{
	const Vector eyePos = Vector(m_pPlayer->pev->origin) + Vector(m_pPlayer->pev->view_ofs);
	const Vector deltaToIdealLocation = forward * PROJECTILE_SPAWN_DIST_IN_FRONT_OF_PLAYER;
	Vector location = eyePos + deltaToIdealLocation;

	{
		TraceResult tr {};
		UTIL_TraceLine(eyePos, location, dont_ignore_monsters, m_pPlayer->edict(), &tr);

		if ( tr.flFraction < 1.0f )
		{
			location = eyePos + (tr.flFraction * deltaToIdealLocation);
		}
	}

	for ( size_t axis = 0; axis < 3; ++axis )
	{
		bool haveAdjustedOnThisAxis = false;

		for ( size_t iteration = 0; iteration < 2; ++iteration )
		{
			const int polarity = iteration == 0 ? 1 : -1;

			Vector basis;
			basis[axis] = static_cast<float>(polarity);

			const float scale = iteration == 0 ? turret.pev->maxs[axis] : -turret.pev->mins[axis];
			const Vector delta = basis * scale;

			TraceResult tr {};
			UTIL_TraceLine(eyePos, location + delta, dont_ignore_monsters, m_pPlayer->edict(), &tr);

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
	return true;
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
