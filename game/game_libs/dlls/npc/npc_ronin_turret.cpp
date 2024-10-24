#include <limits>
#include <cmath>
#include "npc/npc_ronin_turret.h"
#include "monsters.h"
#include "weapons.h"
#include "gameplay/hitscancomponent.h"
#include "weaponregistry.h"
#include "MathLib/utils.h"

static constexpr const char* const RONIN_MODEL = "models/weapon_ronin/w_ronin.mdl";
static constexpr const char* const INFO_RONIN_TARGET = "info_ronin_target";

static constexpr int DEFAULT_RANDOM_SEED = 0x1B7F23DE;

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
	DEFINE_FIELD(CNPCRoninTurret, m_KVSightFOV, FIELD_FLOAT),
	DEFINE_FIELD(CNPCRoninTurret, m_ShootFOV, FIELD_FLOAT),
	DEFINE_FIELD(CNPCRoninTurret, m_SearchRange, FIELD_FLOAT),
	DEFINE_FIELD(CNPCRoninTurret, m_FireInterval, FIELD_FLOAT),
	DEFINE_FIELD(CNPCRoninTurret, m_SpreadCone, FIELD_FLOAT),
	DEFINE_FIELD(CNPCRoninTurret, m_CurrentGunAngles, FIELD_VECTOR),
	DEFINE_FIELD(CNPCRoninTurret, m_LastAngleUpdate, FIELD_TIME),
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

	if ( FStrEq(data->szKeyName, "firerate") )
	{
		float fireRate = static_cast<float>(atof(data->szValue));

		if ( fireRate != 0.0f )
		{
			if ( fireRate > MAX_FIRE_RATE )
			{
				fireRate = MAX_FIRE_RATE;
			}

			m_FireInterval = 1.0f / fireRate;
		}

		data->fHandled = true;
		return;
	}

	if ( FStrEq(data->szKeyName, "bulletspread") )
	{
		float degrees = static_cast<float>(atof(data->szValue)) / 2.0f;

		if ( degrees < 0.0f )
		{
			degrees = 0.0f;
		}
		else if ( degrees > MAX_HALF_BULLET_SPREAD_DEGREES )
		{
			degrees = MAX_HALF_BULLET_SPREAD_DEGREES;
		}

		m_SpreadCone = std::tanf(DEG2RADF(degrees));
		data->fHandled = true;
		return;
	}

	if ( FStrEq(data->szKeyName, "rotationspeed") )
	{
		m_RotationSpeed = static_cast<float>(atof(data->szValue));

		if ( m_RotationSpeed < 1.0f )
		{
			m_RotationSpeed = 1.0f;
		}

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
	if ( std::isnan(m_LastAngleUpdate) )
	{
		m_LastAngleUpdate = gpGlobals->time;
	}

	StudioFrameAdvance();

	m_hEnemy = FindBestTarget();

	if ( m_hEnemy )
	{
		RotateTowardsTarget();
		AttackTarget();
	}

	m_LastAngleUpdate = gpGlobals->time;
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
	UpdateModelControllerValues();

	m_hEnemy = nullptr;
	m_LastAngleUpdate = NAN;
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

	m_LastAngleUpdate = NAN;
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

void CNPCRoninTurret::UpdateModelControllerValues()
{
	SetBoneController(0, m_CurrentGunAngles[YAW]);
	SetBoneController(1, m_CurrentGunAngles[PITCH]);
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

void CNPCRoninTurret::RotateTowardsTarget()
{
	if ( !m_hEnemy || std::isnan(m_LastAngleUpdate) )
	{
		return;
	}

	const float timeDelta = gpGlobals->time - m_LastAngleUpdate;

	if ( timeDelta <= 0.0f )
	{
		return;
	}

	Vector gunPos = GetGunBarrelPos();
	Vector targetPos = GetBestTargetPosition(0.0f, 8.0f);
	Vector targetDir = (targetPos - gunPos).Normalize();

	Vector anglesToTarget;
	VectorAngles(targetDir, anglesToTarget);
	anglesToTarget[PITCH] *= -1.0f;

	Vector currentAngles(pev->angles);
	currentAngles += m_CurrentGunAngles;
	NormalizeAngles(currentAngles);
	currentAngles[ROLL] = 0.0f;

	Vector angleDelta = anglesToTarget - currentAngles;
	NormalizeAngles(angleDelta);

	// Turn angles past 180 degrees into negative angles.
	if ( angleDelta[PITCH] > 180.0f )
	{
		angleDelta[PITCH] -= 360.0f;
	}

	if ( angleDelta[YAW] > 180.0f )
	{
		angleDelta[YAW] -= 360.0f;
	}

	const float maxRotSpeed = GetRotationSpeed() * timeDelta;

	if ( angleDelta[PITCH] > maxRotSpeed )
	{
		angleDelta[PITCH] = maxRotSpeed;
	}
	else if ( angleDelta[PITCH] < -maxRotSpeed )
	{
		angleDelta[PITCH] = -maxRotSpeed;
	}

	if ( angleDelta[YAW] > maxRotSpeed )
	{
		angleDelta[YAW] = maxRotSpeed;
	}
	else if ( angleDelta[YAW] < -maxRotSpeed )
	{
		angleDelta[YAW] = -maxRotSpeed;
	}

	m_CurrentGunAngles += angleDelta;
	NormalizeAngles(m_CurrentGunAngles);

	if ( m_CurrentGunAngles[PITCH] > MAX_PITCH_DEVIATION )
	{
		m_CurrentGunAngles[PITCH] = MAX_PITCH_DEVIATION;
	}
	else if ( m_CurrentGunAngles[PITCH] < -MAX_PITCH_DEVIATION )
	{
		m_CurrentGunAngles[PITCH] = -MAX_PITCH_DEVIATION;
	}

	SetBoneController(0, m_CurrentGunAngles[YAW]);
}

void CNPCRoninTurret::AttackTarget()
{
	if ( !m_hEnemy )
	{
		return;
	}

	CBasePlayer* playerOwner = pev->owner ? GetClassPtr<CBasePlayer>(VARS(pev->owner)) : nullptr;

	Vector gunPos = GetGunBarrelPos();
	const float spread = GetSpreadCone();

	Vector shootAngles(pev->angles);
	shootAngles += m_CurrentGunAngles;
	NormalizeAngles(shootAngles);

	Vector shootDir;
	AngleVectors(shootAngles, shootDir, nullptr, nullptr);

	CHitscanComponent hitscanComponent;

	hitscanComponent.SetGunPos(gunPos);
	hitscanComponent.SetShootDir(shootDir);
	hitscanComponent.SetInflictor(pev);
	hitscanComponent.SetRandomSeed(playerOwner ? playerOwner->random_seed : DEFAULT_RANDOM_SEED);
	hitscanComponent.SetRightDir(gpGlobals->v_right);
	hitscanComponent.SetUpDir(gpGlobals->v_up);
	hitscanComponent.SetAttacker(pev->owner ? VARS(pev->owner) : pev);
	hitscanComponent.SetBulletsPerShot(1);
	hitscanComponent.SetBaseDamagePerShot(10 /*TODO: Make skill-based*/);
	hitscanComponent.SetSpread(Vector2D(spread, spread));
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

Vector CNPCRoninTurret::GetGunBarrelPos() const
{
	Vector out;
	Vector dummyAngles;
	GetAttachment(0, out, dummyAngles);
	return out;
}

float CNPCRoninTurret::GetBestThinkInterval() const
{
	// We think at least twice as often as we need to fire, to facilitate tracking enemies.
	const float thinkInterval = GetFireInterval() / 2.0f;
	return thinkInterval < MAX_ACTIVE_THINK_INTERVAL ? thinkInterval : MAX_ACTIVE_THINK_INTERVAL;
}

float CNPCRoninTurret::GetSearchRange() const
{
	return !std::isnan(m_SearchRange) ? m_SearchRange : DEFAULT_SEARCH_RANGE;
}

float CNPCRoninTurret::GetFireInterval() const
{
	return !std::isnan(m_FireInterval) ? m_FireInterval : (1.0f / DEFAULT_FIRE_RATE);
}

float CNPCRoninTurret::GetSpreadCone() const
{
	return !std::isnan(m_SpreadCone) ? m_SpreadCone : 0.0f;
}

float CNPCRoninTurret::GetRotationSpeed() const
{
	return !std::isnan(m_RotationSpeed) ? m_RotationSpeed : DEFAULT_ROT_DEGREES_PER_SECOND;
}
