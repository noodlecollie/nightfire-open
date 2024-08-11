#include <limits>
#include <cmath>
#include "npc/npc_ronin_turret.h"
#include "monsters.h"
#include "weapons.h"
#include "gameplay/hitscancomponent.h"
#include "weaponregistry.h"

static constexpr const char* const RONIN_MODEL = "models/weapon_ronin/w_ronin.mdl";
static constexpr const char* const INFO_RONIN_TARGET = "info_ronin_target";

static bool IsInfoRoninTarget(CBaseEntity* ent)
{
	return ent && FClassnameIs(ent->pev, INFO_RONIN_TARGET);
}

static float CalculateFOVDotProduct(float degrees)
{
	if ( degrees > 0.0f && degrees < 360.0f )
	{
		return cosf(DEG2RADF(degrees) / 2.0f);
	}
	else
	{
		// The view calculations check whether the dot product
		// is strictly greater than the entity's FOV.
		// To avoid boundary conditions, set this value to be
		// more negative than any view calculation result.
		return std::numeric_limits<float>::min();
	}
}

// Helper entity - if this is within a Ronin's range, the Ronin will shoot at it.
LINK_ENTITY_TO_CLASS(info_ronin_target, CPointEntity)

LINK_ENTITY_TO_CLASS(npc_ronin_turret, CNPCRoninTurret)

TYPEDESCRIPTION CNPCRoninTurret::m_SaveData[] = {
	DEFINE_FIELD(CNPCRoninTurret, m_DeployState, FIELD_INTEGER),
};

int CNPCRoninTurret::BloodColor(void)
{
	return DONT_BLEED;
}

void CNPCRoninTurret::GibMonster(void)
{
	// No gibs
}

int CNPCRoninTurret::Classify(void)
{
	return CLASS_MACHINE;
}

int CNPCRoninTurret::ObjectCaps(void)
{
	return CBaseMonster::ObjectCaps() | FCAP_IMPULSE_USE;
}

void CNPCRoninTurret::KeyValue(KeyValueData* data)
{
	if ( FStrEq(data->szKeyName, "sightfov") )
	{
		m_KVSightFOV = CalculateFOVDotProduct(static_cast<float>(atof(data->szValue)));
		data->fHandled = true;
		return;
	}

	if ( FStrEq(data->szKeyName, "shootfov") )
	{
		m_ShootFOV = CalculateFOVDotProduct(static_cast<float>(atof(data->szValue)));
		data->fHandled = true;
		return;
	}

	if ( FStrEq(data->szKeyName, "searchrange") )
	{
		m_SearchRange = static_cast<float>(atof(data->szValue));
		data->fHandled = true;
		return;
	}

	CBaseMonster::KeyValue(data);
}

void CNPCRoninTurret::Spawn(void)
{
	CBaseMonster::Spawn();

	Precache();

	SET_MODEL(ENT(pev), RONIN_MODEL);
	UTIL_SetSize(pev, Vector(-14, -12, 0), Vector(14, 12, 24));

	pev->movetype = MOVETYPE_TOSS;
	pev->sequence = NPCRONIN_IDLE1;
	pev->frame = 0;
	pev->solid = SOLID_SLIDEBOX;
	pev->takedamage = static_cast<float>(pev->max_health > 0.0f ? DAMAGE_AIM : DAMAGE_NO);

	SetBits(pev->flags, FL_MONSTER);
	SetUse(&CNPCRoninTurret::RoninUse);

	if ( !std::isnan(m_KVSightFOV) )
	{
		m_flFieldOfView = m_KVSightFOV;
	}
	else
	{
		m_flFieldOfView = CalculateFOVDotProduct(90.0f);
	}

	if ( std::isnan(m_ShootFOV) )
	{
		m_ShootFOV = CalculateFOVDotProduct(30.0f);
	}

	m_hEnemy = nullptr;
	ResetSequenceInfo();

	// If the saved state was during a deploy or undeploy,
	// re-initiate the action.
	if ( m_DeployState == DeployState::DEPLOYING )
	{
		m_DeployState = DeployState::NOT_DEPLOYED;
		DeployNow();
	}
	else if ( m_DeployState == DeployState::UNDEPLOYING )
	{
		m_DeployState = DeployState::DEPLOYED;
		UndeployNow();
	}
}

void CNPCRoninTurret::Precache(void)
{
	CBaseMonster::Precache();
	PRECACHE_MODEL(RONIN_MODEL);
}

void CNPCRoninTurret::RoninUse(
	CBaseEntity* /* pActivator */,
	CBaseEntity* /* pCaller */,
	USE_TYPE /* useType */,
	float /* value */)
{
	ToggleDeploy();
}

void CNPCRoninTurret::ToggleDeploy()
{
	switch ( m_DeployState )
	{
		case DeployState::NOT_DEPLOYED:
		{
			BeginDeploy();
			break;
		}

		case DeployState::DEPLOYED:
		{
			BeginUndeploy();
			break;
		}

		default:
		{
			// Don't interfere if we're between states.
			return;
		}
	}
}

void CNPCRoninTurret::DeployNow()
{
	if ( m_DeployState == DeployState::NOT_DEPLOYED )
	{
		BeginDeploy();
	}
}

void CNPCRoninTurret::UndeployNow()
{
	if ( m_DeployState == DeployState::DEPLOYED )
	{
		BeginUndeploy();
	}
}

void CNPCRoninTurret::ActiveThink()
{
	StudioFrameAdvance();

	m_hEnemy = FindBestTarget();

	if ( m_hEnemy )
	{
		AttackTarget();
	}

	pev->nextthink = gpGlobals->time + GetBestThinkInterval();
}

void CNPCRoninTurret::BeginDeploy()
{
	m_DeployState = DeployState::DEPLOYING;

	SetSequence(NPCRONIN_DEPLOY);

	SetThink(&CNPCRoninTurret::DeployFinished);
	pev->nextthink = gpGlobals->time + DEPLOY_DURATION;
}

void CNPCRoninTurret::DeployFinished()
{
	m_DeployState = DeployState::DEPLOYED;
	SetSequence(NPCRONIN_DEPLOY_IDLE);

	m_hEnemy = nullptr;
	SetThink(&CNPCRoninTurret::ActiveThink);
	pev->nextthink = gpGlobals->time + GetBestThinkInterval();
}

void CNPCRoninTurret::BeginUndeploy()
{
	m_DeployState = DeployState::UNDEPLOYING;

	// We don't currently have an undeploy animation,
	// but blending back to the idle animation works
	// well enough for now.
	SetSequence(NPCRONIN_IDLE1);

	SetThink(&CNPCRoninTurret::UndeployFinished);
	pev->nextthink = gpGlobals->time + UNDEPLOY_DURATION;
}

void CNPCRoninTurret::UndeployFinished()
{
	m_DeployState = DeployState::NOT_DEPLOYED;
	SetSequence(NPCRONIN_IDLE1);
	SetThink(nullptr);
}

void CNPCRoninTurret::SetSequence(NPCRoninTurretAnimations_e index)
{
	pev->sequence = index;
	ResetSequenceInfo();
}

CBaseEntity* CNPCRoninTurret::FindBestTarget()
{
	const float radius = GetSearchRange();

	if ( radius <= 0.0f )
	{
		// Don't search
		return nullptr;
	}

	CBaseEntity* bestTarget = nullptr;
	float bestRange = std::numeric_limits<float>::max();

	for ( CBaseEntity* ent = UTIL_FindEntityInSphere(nullptr, pev->origin, radius); ent;
		  ent = UTIL_FindEntityInSphere(ent, pev->origin, radius) )
	{
		// Quick filters first:
		if ( FBitSet(ent->pev->flags, FL_NOTARGET) || !FInViewCone(ent) || !EnemyVisible(ent) )
		{
			continue;
		}

		// Don't shoot at our owner
		if ( pev->owner && ent->edict() == pev->owner )
		{
			continue;
		}

		bool foundNewBestTarget = false;
		float distance = (Vector(ent->pev->origin) - Vector(pev->origin)).Length();

		if ( IsInfoRoninTarget(ent) )
		{
			// info_ronin_target always takes precedence over players
			if ( !bestTarget || bestTarget->IsPlayer() || distance < bestRange )
			{
				foundNewBestTarget = true;
			}
		}
		else if ( ent->IsPlayer() && !IsInfoRoninTarget(bestTarget) )
		{
			if ( ent->IsAlive() && distance < bestRange )
			{
				foundNewBestTarget = true;
			}
		}

		if ( foundNewBestTarget )
		{
			bestTarget = ent;
			bestRange = distance;
		}
	}

	return bestTarget;
}

void CNPCRoninTurret::AttackTarget()
{
	if ( !m_hEnemy )
	{
		return;
	}

	// TODO: We want to fire along the Ronin's LOS if the target
	// is within the shooting FOV, rather than firing directly at the target.
	Vector gunPos = GetEyePos();
	Vector targetPos = GetBestTargetPosition(0.0f, 8.0f);

	CHitscanComponent hitscanComponent;

	hitscanComponent.SetGunPos(gunPos);
	hitscanComponent.SetShootDir(targetPos - gunPos);
	hitscanComponent.SetInflictor(pev);
	hitscanComponent.SetRandomSeed(0);
	hitscanComponent.SetRightDir(gpGlobals->v_right);
	hitscanComponent.SetUpDir(gpGlobals->v_up);
	hitscanComponent.SetAttacker(pev /*TODO: Ronin's owner*/);
	hitscanComponent.SetBulletsPerShot(1);
	hitscanComponent.SetBaseDamagePerShot(10 /*TODO: Make skill-based*/);
	hitscanComponent.SetSpread(Vector2D(0.1f, 0.1f /*TODO: Make configurable*/));
	hitscanComponent.SetSendTracerMessage(true);

	hitscanComponent.FireBullets();

	pev->effects = pev->effects | EF_MUZZLEFLASH;
}

bool CNPCRoninTurret::EnemyVisible(CBaseEntity* ent) const
{
	if ( !ent || !ent->pev || FBitSet(ent->pev->flags, FL_NOTARGET) )
	{
		return false;
	}

	// Don't look through water
	if ( (pev->waterlevel != 3 && ent->pev->waterlevel == 3) || (pev->waterlevel == 3 && ent->pev->waterlevel == 0) )
	{
		return false;
	}

	Vector eyePos = GetEyePos();
	Vector targetPos = ent->BodyTarget(eyePos);

	TraceResult tr;
	UTIL_TraceLine(eyePos, targetPos, ignore_monsters, ignore_glass, ENT(pev), &tr);

	return tr.flFraction >= 1.0f;
}

Vector CNPCRoninTurret::GetBestTargetPosition(float minUnitsDevFromTarget, float maxUnitsDevFromTarget) const
{
	if ( !m_hEnemy )
	{
		return Vector();
	}

	Vector eyePos = GetEyePos();
	Vector targetPos = m_hEnemy->BodyTarget(eyePos);
	Vector ray = targetPos - eyePos;
	Vector up(0, 0, 1);

	Vector horizAxis;
	CrossProduct(ray, up, horizAxis);

	if ( !VectorIsNull(horizAxis) )
	{
		horizAxis.Normalize();
	}

	for ( size_t attempt = 0; attempt < 5; ++attempt )
	{
		Vector currentTarget = targetPos;
		float devDirection = static_cast<float>((attempt % 2) * -1);

		switch ( attempt )
		{
			// Deviate horizontally
			case 1:
			case 2:
			{
				currentTarget += horizAxis * devDirection * RANDOM_FLOAT(minUnitsDevFromTarget, maxUnitsDevFromTarget);
				break;
			}

			// Deviate vertically
			case 3:
			case 4:
			{
				currentTarget += up * devDirection * RANDOM_FLOAT(minUnitsDevFromTarget, maxUnitsDevFromTarget);
				break;
			}

			default:
			{
				break;
			}
		}

		TraceResult tr;
		UTIL_TraceLine(eyePos, currentTarget, ignore_monsters, ignore_glass, ENT(pev), &tr);

		if ( tr.flFraction == 1.0 )
		{
			return currentTarget;
		}
	}

	// Couldn't hit on any ray that we tested, so just shoot straight down the line.
	return targetPos;
}

Vector CNPCRoninTurret::GetEyePos() const
{
	return Vector(pev->origin) + Vector(pev->view_ofs);
}

float CNPCRoninTurret::GetBestThinkInterval() const
{
	// TODO: Once we add the fire rate variable, this should return
	// an interval which is at least quick enough to cater
	// for this fire rate, or MAX_ACTIVE_THINK_INTERVAL
	// if the fire rate is slow.
	return MAX_ACTIVE_THINK_INTERVAL;
}

float CNPCRoninTurret::GetSearchRange()
{
	return !std::isnan(m_SearchRange) ? m_SearchRange : DEFAULT_SEARCH_RANGE;
}
