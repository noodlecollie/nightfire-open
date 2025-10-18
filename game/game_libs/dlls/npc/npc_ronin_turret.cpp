#include <limits>
#include <cmath>
#include "npc/npc_ronin_turret.h"
#include "EnginePublicAPI/const.h"
#include "EnginePublicAPI/eiface.h"
#include "cbase.h"
#include "monsters.h"
#include "player.h"
#include "weapons.h"
#include "gameplay/hitscanaction.h"
#include "weaponregistry.h"
#include "MathLib/utils.h"
#include "weapons/weapon_ronin.h"
#include "gamerules.h"
#include "explode.h"
#include "resources/SoundResources.h"
#include "soundent.h"

static constexpr const char* const RONIN_MODEL = "models/weapon_ronin/w_ronin.mdl";
static constexpr const char* const INFO_RONIN_TARGET = "info_ronin_target";
static constexpr const char* const FIRE_SOUND = "weapons/weapon_ronin/fire.wav";
static constexpr const char* const POWERDOWN_SOUND = "weapons/weapon_ronin/powerdown.wav";

static constexpr int DEFAULT_RANDOM_SEED = 0x1B7F23DE;
static constexpr float ATTEN_BULLET = 0.6f;
static constexpr float ATTEN_POWERDOWN = 1.2f;
static constexpr float ATTEN_SPARK = 1.2f;

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
		return std::numeric_limits<float>::lowest();
	}
}

// Helper entity - if this is within a Ronin's range, the Ronin will shoot at it.
LINK_ENTITY_TO_CLASS(info_ronin_target, CPointEntity)

LINK_ENTITY_TO_CLASS(npc_ronin_turret, CNPCRoninTurret)

cvar_t sv_ronin_slide_friction = CONSTRUCT_CVAR_T("sv_ronin_slide_friction", 0.88, FCVAR_SERVER);
cvar_t sv_ronin_death_delay = CONSTRUCT_CVAR_T("sv_ronin_death_delay", 6.0, FCVAR_SERVER);

TYPEDESCRIPTION CNPCRoninTurret::m_SaveData[] = {
	DEFINE_FIELD(CNPCRoninTurret, m_DeployState, FIELD_INTEGER),
	DEFINE_FIELD(CNPCRoninTurret, m_DeploySequenceEnd, FIELD_TIME),
	DEFINE_FIELD(CNPCRoninTurret, m_KVSightFOV, FIELD_FLOAT),
	DEFINE_FIELD(CNPCRoninTurret, m_ShootFOV, FIELD_FLOAT),
	DEFINE_FIELD(CNPCRoninTurret, m_SearchRange, FIELD_FLOAT),
	DEFINE_FIELD(CNPCRoninTurret, m_FireInterval, FIELD_FLOAT),
	DEFINE_FIELD(CNPCRoninTurret, m_SpreadCone, FIELD_FLOAT),
	DEFINE_FIELD(CNPCRoninTurret, m_ExplosionDamage, FIELD_INTEGER),
	DEFINE_FIELD(CNPCRoninTurret, m_CurrentGunAngles, FIELD_VECTOR),
	DEFINE_FIELD(CNPCRoninTurret, m_LastActiveThink, FIELD_TIME),
	DEFINE_FIELD(CNPCRoninTurret, m_GunBarrelMinZOffset, FIELD_FLOAT),
	DEFINE_FIELD(CNPCRoninTurret, m_LastTossedPos, FIELD_VECTOR),
	DEFINE_FIELD(CNPCRoninTurret, m_LastTossedTime, FIELD_TIME),
	DEFINE_FIELD(CNPCRoninTurret, m_NextDyingTick, FIELD_TIME),
	DEFINE_FIELD(CNPCRoninTurret, m_TargetSelfDestructionTime, FIELD_TIME),
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

		m_SpreadCone = tanf(DEG2RADF(degrees));
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

	// NFTODO: Make Ronin disable itself but not explode if this is 0?
	// It would need to essentially become a corpse that fades after a while.
	// Check what the monsters do to achieve this.
	if ( FStrEq(data->szKeyName, "explosiondamage") )
	{
		m_ExplosionDamage = atoi(data->szValue);

		if ( m_ExplosionDamage < 0 )
		{
			m_ExplosionDamage = 0;
		}

		data->fHandled = true;
		return;
	}

	CBaseMonster::KeyValue(data);
}

void CNPCRoninTurret::Killed(
	entvars_t* pevAttacker,
	int iGib,
	int bitsDamageType,
	float damageApplied,
	float damageTaken
)
{
	Remember(bits_MEMORY_KILLED);
	pev->health = 0.0f;
	pev->takedamage = DAMAGE_NO;
	pev->deadflag = DEAD_DYING;

	// Store the attacker, so that we can credit
	// them for damage caused when the Ronin explodes.
	m_hActivator.Set(ENT(pevAttacker));

	float deathDuration = sv_ronin_death_delay.value;

	if ( deathDuration < 0.0f )
	{
		deathDuration = 0.0f;
	}

	if ( deathDuration > 0.0f )
	{
		EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, POWERDOWN_SOUND, 1, ATTEN_POWERDOWN, 0, 100);
		EmitDisablingSmoke();
	}

	m_TargetSelfDestructionTime = gpGlobals->time + deathDuration;
	m_NextDyingTick = gpGlobals->time;

	CBaseMonster::Killed(pevAttacker, iGib, bitsDamageType, damageApplied, damageTaken);
}

void CNPCRoninTurret::Spawn(void)
{
	CBaseMonster::Spawn();

	pev->classname = MAKE_STRING("npc_ronin_turret");
	Precache();

	// TODO: We probably want custom solidity here. If we use SOLID_TRIGGER,
	// the Ronin cannot take damage. If we use SOLID_BBOX, the Ronin cannot be
	// touched by its owner, so can't be picked up under certain circumstances
	// when we want it to.
	pev->solid = SOLID_BBOX;

	pev->movetype = MOVETYPE_NONE;
	pev->friction = 1.0f;
	pev->sequence = NPCRONIN_IDLE1;
	pev->frame = 0;
	pev->max_health = pev->health;
	pev->takedamage = DAMAGE_NO;
	pev->deadflag = DEAD_NO;

	// These calls should go AFTER the stuff above, particularly
	// because UTIL_SetSize() relies on the solid type being set
	// in order to correctly place the entity in the BSP tree.
	SET_MODEL(ENT(pev), RONIN_MODEL);
	UTIL_SetSize(pev, RONIN_TURRET_MINS, RONIN_TURRET_MAXS);

	m_AllowPickupWhenUndeployed = (pev->spawnflags & SF_ALLOW_PICKUP) != 0;
	m_AllowUndeployWithUseKey = (pev->spawnflags & SF_UNDEPLOY_WITH_USE_KEY) != 0;

	SetBits(pev->flags, FL_MONSTER);
	SetUse(&CNPCRoninTurret::RoninUse);
	SetTouch(&CNPCRoninTurret::OnTouch);

	if ( !std::isnan(m_KVSightFOV) )
	{
		m_flFieldOfView = m_KVSightFOV;
	}
	else
	{
		m_flFieldOfView = CalculateFOVDotProduct(DEFAULT_SIGHT_FOV);
	}

	if ( std::isnan(m_ShootFOV) )
	{
		m_ShootFOV = CalculateFOVDotProduct(DEFAULT_SHOOT_FOV);
	}

	m_hEnemy = nullptr;
	ResetSequenceInfo();
	UpdateBodyAndSkin();

	SetThink(&CNPCRoninTurret::MainThink);
	pev->nextthink = gpGlobals->time + THINK_INTERVAL;

	if ( pev->spawnflags & SF_START_DEPLOYED )
	{
		DeployFinished();

		// Reset the spawn flag, so that it's not still set in Ronin's saved data.
		pev->spawnflags &= ~SF_START_DEPLOYED;
	}

	// If the saved state was during a deploy or undeploy,
	// re-initiate the action.
	if ( m_DeployState == DeployState::DEPLOYING )
	{
		SetDeployState(DeployState::UNDEPLOYED);
		DeployNow();
	}
	else if ( m_DeployState == DeployState::UNDEPLOYING )
	{
		SetDeployState(DeployState::DEPLOYED);
		UndeployNow();
	}
}

void CNPCRoninTurret::Precache(void)
{
	CBaseMonster::Precache();

	PRECACHE_MODEL(RONIN_MODEL);
	PRECACHE_SOUND(FIRE_SOUND);
	PRECACHE_SOUND(POWERDOWN_SOUND);
}

bool CNPCRoninTurret::AllowsPickupWhenUndeployed() const
{
	return m_AllowPickupWhenUndeployed;
}

void CNPCRoninTurret::SetAllowsPickupWhenUndeployed(bool allow)
{
	m_AllowPickupWhenUndeployed = allow;
}

bool CNPCRoninTurret::AllowUndeployWithUseKey() const
{
	return m_AllowUndeployWithUseKey;
}

void CNPCRoninTurret::SetAllowUndeployWithUseKey(bool allow)
{
	m_AllowUndeployWithUseKey = allow;
}

bool CNPCRoninTurret::Explode()
{
	if ( pev->deadflag == DEAD_DYING )
	{
		// Don't allow the explosion, because we are dying and
		// will soon explode ourselves. This class will take
		// care of resetting this flag before calling Explode().
		// This stops players from detonating their Ronin
		// if it's already in the process of dying.
		return false;
	}

	pev->solid = SOLID_NOT;
	pev->model = iStringNull;
	pev->deadflag = DEAD_DEAD;

	edict_t* explosionOwner = ENT(pev);

	if ( m_hActivator )
	{
		// We were killed by someone and stored their pointer.
		// They should get the credit over the owner, since they
		// managed to initiate this explosion.
		explosionOwner = m_hActivator->edict();
	}
	else if ( pev->owner )
	{
		// We have an owner, so they should get the credit.
		explosionOwner = pev->owner;
	}

	ExplosionCreate(pev->origin, Vector(), explosionOwner, m_ExplosionDamage, TRUE);

	UTIL_Remove(this);
	CommunicateStateToOwner();
	return true;
}

void CNPCRoninTurret::SetExplosionDamage(int damage)
{
	m_ExplosionDamage = damage;
}

void CNPCRoninTurret::
	RoninUse(CBaseEntity* pActivator, CBaseEntity* /* pCaller */, USE_TYPE /* useType */, float /* value */)
{
	if ( m_DeployState == DeployState::DEPLOYED && m_AllowUndeployWithUseKey )
	{
		BeginUndeploy();
		return;
	}

	if ( CanTryToPickUp(pActivator) &&
		 CWeaponRonin::AddRoninWeaponToPlayer(
			 dynamic_cast<CBasePlayer*>(pActivator),
			 CWeaponRonin::TurretPickupType::ON_USE,
			 this
		 ) )
	{
		UTIL_Remove(this);
	}
}

void CNPCRoninTurret::ToggleDeploy()
{
	switch ( m_DeployState )
	{
		case DeployState::TOSSED:
		case DeployState::UNDEPLOYED:
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
	if ( IsUndeployed() )
	{
		BeginDeploy();
	}
}

void CNPCRoninTurret::UndeployNow()
{
	if ( IsFullyDeployed() )
	{
		BeginUndeploy();
	}
}

CNPCRoninTurret::DeployState CNPCRoninTurret::GetDeployState() const
{
	return m_DeployState;
}

bool CNPCRoninTurret::IsUndeployed() const
{
	return m_DeployState == DeployState::TOSSED || m_DeployState == DeployState::UNDEPLOYED;
}

bool CNPCRoninTurret::IsFullyDeployed() const
{
	return m_DeployState == DeployState::DEPLOYED;
}

Vector CNPCRoninTurret::GetCentre() const
{
	const float height = pev->maxs[VEC3_Z] - pev->mins[VEC3_Z];
	return Vector(pev->origin) + Vector(0.0f, 0.0f, height / 2.0f);
}

void CNPCRoninTurret::StartToss(const Vector& velocity, const Vector& angularVelocity)
{
	StartToss(pev->origin, velocity, angularVelocity);
}

void CNPCRoninTurret::StartToss(const Vector& origin, const Vector& velocity, const Vector& angularVelocity)
{
	pev->movetype = MOVETYPE_IN_TOSS;
	pev->gravity = 0.75f;

	UTIL_SetOrigin(pev, origin);
	velocity.CopyToArray(pev->velocity);
	angularVelocity.CopyToArray(pev->avelocity);

	pev->angles[PITCH] = 0.0f;
	pev->angles[YAW] = UTIL_VecToAngles(velocity.Normalize())[YAW];
	pev->angles[ROLL] = 0.0f;

	pev->sequence = NPCRONIN_IDLE1;
	pev->frame = 0;
	ResetSequenceInfo();

	// If the Ronin is tossed, it can search anywhere.
	m_flFieldOfView = FOV_SEARCH_ANYWHERE;

	SetCurrentGunAngles(Vector());
	SetDeployState(DeployState::TOSSED);

	m_LastTossedPos = pev->origin;
	m_LastTossedTime = gpGlobals->time;
}

void CNPCRoninTurret::Place(const Vector& origin, const Vector& angles, float fov)
{
	StartToss(origin, Vector(), Vector());
	m_flFieldOfView = CalculateFOVDotProduct(fov);
	angles.CopyToArray(pev->angles);
}

void CNPCRoninTurret::MainThink()
{
	if ( !IsInWorld() )
	{
		UTIL_Remove(this);
		return;
	}

	if ( m_DeployState != DeployState::DEPLOYED )
	{
		// Make sure this is always NaN outside of an active think.
		m_LastActiveThink = NAN;
	}

	UpdateVelocity();

	switch ( m_DeployState )
	{
		case DeployState::DEPLOYING:
		{
			DeployingThink();
			break;
		}

		case DeployState::DEPLOYED:
		{
			ActiveThink();
			break;
		}

		case DeployState::UNDEPLOYING:
		{
			UndeployingThink();
			break;
		}

		default:
		{
			break;
		}
	}

	pev->nextthink = gpGlobals->time + THINK_INTERVAL;
}

void CNPCRoninTurret::DeployingThink()
{
	if ( std::isnan(m_DeploySequenceEnd) || m_DeploySequenceEnd <= gpGlobals->time )
	{
		DeployFinished();
	}
}

void CNPCRoninTurret::UndeployingThink()
{
	if ( std::isnan(m_DeploySequenceEnd) || m_DeploySequenceEnd <= gpGlobals->time )
	{
		UndeployFinished();
	}
}

void CNPCRoninTurret::ActiveThink()
{
	if ( std::isnan(m_LastActiveThink) )
	{
		m_LastActiveThink = gpGlobals->time;
	}

	StudioFrameAdvance();

	if ( !HasMemory(bits_MEMORY_KILLED) )
	{
		AttackThink();
	}
	else
	{
		DyingThink();
	}

	m_LastActiveThink = gpGlobals->time;
}

void CNPCRoninTurret::AttackThink()
{
	m_hEnemy = FindBestTarget();
	pev->enemy = m_hEnemy ? m_hEnemy->edict() : nullptr;

	if ( m_hEnemy )
	{
		const Vector targetPos = GetBestTargetPosition(0.0f, 8.0f);
		RotateTowardsTarget(targetPos);

		if ( TargetIsInShootFOV(targetPos) )
		{
			FireGun();
		}
	}
}

void CNPCRoninTurret::DyingThink()
{
	m_hEnemy = nullptr;
	pev->enemy = nullptr;

	if ( m_TargetSelfDestructionTime <= gpGlobals->time )
	{
		pev->deadflag = DEAD_DEAD;
		Explode();
		return;
	}

	RotateTowardsIdealAngles(Vector(90, GetGunBarrelAngles()[YAW], 0), true);

	if ( m_NextDyingTick > gpGlobals->time )
	{
		return;
	}

	const float timeLeft = m_TargetSelfDestructionTime - gpGlobals->time;
	const bool isNearDeath = timeLeft <= NEAR_DEATH_DURATION;

	UTIL_Sparks(GetAttachmentPos(ATTACHMENT_GUN_BODY));

	float pitch = static_cast<float>(RANDOM_LONG(98, 102));

	if ( isNearDeath )
	{
		if ( !HasMemory(bits_MEMORY_CUSTOM1) )
		{
			CSoundEnt::InsertSound(bits_SOUND_DANGER, pev->origin, 400, 0.3f);
			Remember(bits_MEMORY_CUSTOM1);
		}

		// Ramp up the pitch as we get closer to exploding.
		pitch = 100.0f * (1.75f - ((timeLeft / NEAR_DEATH_DURATION) * 0.5f));
	}

	EMIT_SOUND_DYN(
		ENT(pev),
		CHAN_ITEM,
		SoundResources::ItemSounds.RandomResourcePath(ItemSoundId::Spark),
		isNearDeath ? 1.0f : 0.5f,
		ATTEN_SPARK,
		0,
		std::min<int>(static_cast<int>(pitch), 255)
	);

	m_NextDyingTick = gpGlobals->time + (isNearDeath ? NEAR_DEATH_SPARK_INTERVAL : DYING_SPARK_INTERVAL);
}

void CNPCRoninTurret::BeginDeploy()
{
	SetDeployState(DeployState::DEPLOYING);

	SetCurrentGunAngles(Vector());
	SetSequence(NPCRONIN_DEPLOY);
	pev->frame = 0;

	CommunicateStateToOwner();
	m_DeploySequenceEnd = gpGlobals->time + DEPLOY_DURATION;
}

void CNPCRoninTurret::DeployFinished()
{
	SetDeployState(DeployState::DEPLOYED);
	SetSequence(NPCRONIN_DEPLOY_IDLE);

	// Cache the gun barrel's Z delta from the origin,
	// so that we can use it for enemy search calculations.
	SetCurrentGunAngles(Vector());
	m_GunBarrelMinZOffset = (GetGunPosition() - Vector(pev->origin)).z;

	pev->takedamage = static_cast<float>(pev->max_health > 0.0f ? DAMAGE_AIM : DAMAGE_NO);
	m_hEnemy = nullptr;

	CommunicateStateToOwner();
}

void CNPCRoninTurret::BeginUndeploy()
{
	SetDeployState(DeployState::UNDEPLOYING);

	SetCurrentGunAngles(Vector());

	// We don't currently have an undeploy animation,
	// but blending back to the idle animation works
	// well enough for now.
	SetSequence(NPCRONIN_IDLE1);

	pev->takedamage = DAMAGE_NO;

	CommunicateStateToOwner();
	m_DeploySequenceEnd = gpGlobals->time + UNDEPLOY_DURATION;
}

void CNPCRoninTurret::UndeployFinished()
{
	SetDeployState(DeployState::UNDEPLOYED);
	SetSequence(NPCRONIN_IDLE1);
	CommunicateStateToOwner();
}

void CNPCRoninTurret::OnTouch(CBaseEntity* other)
{
	if ( !other || other->Classify() != CLASS_PLAYER )
	{
		return;
	}

	if ( CanTryToPickUp(other) &&
		 CWeaponRonin::AddRoninWeaponToPlayer(
			 dynamic_cast<CBasePlayer*>(other),
			 CWeaponRonin::TurretPickupType::ON_TOUCH
		 ) )
	{
		UTIL_Remove(this);
		return;
	}
}

void CNPCRoninTurret::SetSequence(NPCRoninTurretAnimations_e index)
{
	pev->sequence = index;
	ResetSequenceInfo();
}

void CNPCRoninTurret::SetCurrentGunAngles(const Vector& angles)
{
	m_CurrentGunAngles = angles;
	UpdateModelControllerValues();
}

void CNPCRoninTurret::UpdateModelControllerValues()
{
	SetBoneController(0, m_CurrentGunAngles[YAW]);
	SetBoneController(1, -m_CurrentGunAngles[PITCH]);
}

void CNPCRoninTurret::UpdateVelocity()
{
	static constexpr float REST_SPEED_THRESHOLD = 5.0f;

	if ( pev->movetype != MOVETYPE_IN_TOSS )
	{
		return;
	}

	if ( pev->flags & FL_ONGROUND )
	{
		// If we've stopped, reset to stable values.
		// Unfortunately pev->velocity is not updated to be accurate if we've
		// made contact with a wall, so we have to track our actual velocity manually.
		if ( m_LastTossedTime < gpGlobals->time )
		{
			const float timeDelta = gpGlobals->time - m_LastTossedTime;
			const float speed = (Vector(pev->origin) - m_LastTossedPos).Length() / timeDelta;

			if ( speed < REST_SPEED_THRESHOLD )
			{
				pev->movetype = MOVETYPE_NONE;
				pev->gravity = 1.0f;

				VectorCopy(vec3_origin, pev->velocity);
				VectorCopy(vec3_origin, pev->avelocity);

				m_LastTossedPos = Vector();
				m_LastTossedTime = 0.0f;

				return;
			}
		}

		// Manually apply some friction
		VectorScale(pev->velocity, sv_ronin_slide_friction.value, pev->velocity);
		VectorScale(pev->avelocity, sv_ronin_slide_friction.value * 0.9f, pev->avelocity);
	}

	m_LastTossedPos = pev->origin;
	m_LastTossedTime = gpGlobals->time;
}

bool CNPCRoninTurret::CanTryToPickUp(CBaseEntity* activator) const
{
	return m_AllowPickupWhenUndeployed && activator && activator->Classify() == CLASS_PLAYER && IsUndeployed() &&
		pev->movetype != MOVETYPE_IN_TOSS;
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
	const edict_t* thisObject = edict();

	for ( CBaseEntity* ent = UTIL_FindEntityInSphere(nullptr, pev->origin, radius); ent;
		  ent = UTIL_FindEntityInSphere(ent, pev->origin, radius) )
	{
		const edict_t* otherObject = ent->edict();

		// Quick checks before the more involved checks:
		if ( otherObject == thisObject ||  //
			 FBitSet(ent->pev->flags, FL_NOTARGET) ||  //
			 (pev->owner && otherObject == pev->owner) ||  //
			 !FInViewCone(ent) ||  //
			 !EnemyVisible(ent) )
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

void CNPCRoninTurret::RotateTowardsTarget(const Vector& targetPos)
{
	Vector gunPos = GetGunBarrelPos();
	Vector targetDir = (targetPos - gunPos).Normalize();

	Vector anglesToTarget;
	VectorAngles(targetDir, anglesToTarget);
	anglesToTarget[PITCH] *= -1.0f;

	RotateTowardsIdealAngles(anglesToTarget);
}

void CNPCRoninTurret::RotateTowardsIdealAngles(const Vector& angles, bool canPointDown)
{
	if ( std::isnan(m_LastActiveThink) )
	{
		return;
	}

	const float timeDelta = gpGlobals->time - m_LastActiveThink;

	if ( timeDelta <= 0.0f )
	{
		return;
	}

	Vector currentGlobalAngles = GetGunBarrelAngles();
	currentGlobalAngles[ROLL] = 0.0f;

	Vector globalAngleDelta = angles - currentGlobalAngles;
	NormalizeAngles(globalAngleDelta);

	// Turn angles past 180 degrees into negative angles.
	if ( globalAngleDelta[PITCH] > 180.0f )
	{
		globalAngleDelta[PITCH] -= 360.0f;
	}

	if ( globalAngleDelta[YAW] > 180.0f )
	{
		globalAngleDelta[YAW] -= 360.0f;
	}

	const float maxRotSpeed = GetRotationSpeed() * timeDelta;

	if ( globalAngleDelta[PITCH] > maxRotSpeed )
	{
		globalAngleDelta[PITCH] = maxRotSpeed;
	}
	else if ( globalAngleDelta[PITCH] < -maxRotSpeed )
	{
		globalAngleDelta[PITCH] = -maxRotSpeed;
	}

	if ( globalAngleDelta[YAW] > maxRotSpeed )
	{
		globalAngleDelta[YAW] = maxRotSpeed;
	}
	else if ( globalAngleDelta[YAW] < -maxRotSpeed )
	{
		globalAngleDelta[YAW] = -maxRotSpeed;
	}

	// The angle delta is computed from global to global angles.
	// Therefore we can just apply it to the gun angles - it doesn't
	// matter that the gun angles are relative to the entity angles.
	Vector newGunAngles = m_CurrentGunAngles + globalAngleDelta;
	NormalizeAngles(newGunAngles);

	if ( newGunAngles[PITCH] < -MAX_BARREL_UPWARD_PITCH )
	{
		newGunAngles[PITCH] = -MAX_BARREL_UPWARD_PITCH;
	}
	else
	{
		const float limit = canPointDown ? MAX_BARREL_DOWNWARD_PITCH : 0.0f;

		if ( newGunAngles[PITCH] > limit )
		{
			newGunAngles[PITCH] = limit;
		}
	}

	SetCurrentGunAngles(newGunAngles);
}

void CNPCRoninTurret::FireGun()
{
	CBasePlayer* playerOwner = pev->owner ? GetClassPtr<CBasePlayer>(VARS(pev->owner)) : nullptr;

	Vector gunPos = GetGunBarrelPos();
	const float spread = GetSpreadCone();

	Vector shootDir;
	Vector rightDir;
	Vector upDir;
	AngleVectors(GetGunBarrelAngles(), shootDir, rightDir, upDir);

	CHitscanAction hitscanAction;

	hitscanAction.SetGunPos(gunPos);
	hitscanAction.SetShootDir(shootDir);
	hitscanAction.SetInflictor(pev);
	hitscanAction.SetRandomSeed(playerOwner ? playerOwner->random_seed : DEFAULT_RANDOM_SEED);
	hitscanAction.SetRightDir(rightDir);
	hitscanAction.SetUpDir(upDir);
	hitscanAction.SetAttacker(pev->owner ? VARS(pev->owner) : pev);
	hitscanAction.SetBulletsPerShot(1);
	hitscanAction.SetBaseDamagePerShot(&skilldata_t::plrDmgRoninBullet);
	hitscanAction.SetSpread(Vector2D(spread, spread));
	hitscanAction.SetSendTracerMessage(true);

	hitscanAction.FireBullets();

	pev->effects = pev->effects | EF_MUZZLEFLASH;

	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, FIRE_SOUND, 1, ATTEN_BULLET, 0, RANDOM_LONG(94, 102));

	SetSequence(NPCRONIN_FIRE);
	pev->frame = 0;
}

void CNPCRoninTurret::UpdateBodyAndSkin()
{
	if ( m_DeployState != DeployState::DEPLOYED )
	{
		pev->body = NPCRONIN_BODY_NO_LIGHTS;
		pev->skin = NPCRONIN_SKIN_NO_LIGHTS;
		return;
	}

	if ( m_flFieldOfView < 0.0f )
	{
		pev->body = NPCRONIN_BODY_ALL_LIGHTS;
		pev->skin = NPCRONIN_SKIN_ORANGE_LIGHTS;
		return;
	}

	pev->body = NPCRONIN_BODY_FRONT_LIGHTS;
	pev->skin = NPCRONIN_SKIN_RED_LIGHTS;
}

void CNPCRoninTurret::SetDeployState(DeployState state)
{
	m_DeployState = state;

	if ( IsUndeployed() || IsFullyDeployed() )
	{
		m_DeploySequenceEnd = NAN;
	}

	UpdateBodyAndSkin();
}

bool CNPCRoninTurret::TargetIsInShootFOV(const Vector& targetPos) const
{
	Vector shootDir;
	AngleVectors(GetGunBarrelAngles(), shootDir, nullptr, nullptr);
	Vector2D shootDir2D = shootDir.Make2D().Normalize();

	Vector targetDelta = targetPos - Vector(pev->origin);
	Vector2D targetDir2D = targetDelta.Make2D().Normalize();

	float dotProduct = Vector2DotProduct(targetDir2D, shootDir2D);

	return dotProduct > m_ShootFOV;
}

bool CNPCRoninTurret::EnemyVisible(CBaseEntity* ent) const
{
	const TraceResult result = TraceSightToEnemy(ent);

	if ( result.flFraction < 1.0f && result.pHit != ent->edict() )
	{
		// Something was in the way
		return false;
	}

	const float top = ent->pev->origin[VEC3_Z] + ent->pev->maxs[VEC3_Z];

	if ( top < pev->origin[VEC3_Z] + m_GunBarrelMinZOffset )
	{
		// Target is too low.
		return false;
	}

	const Vector bottom = Vector(ent->pev->origin) + Vector(0, 0, ent->pev->mins[VEC3_Z]);
	const Vector gunBase = Vector(pev->origin) + Vector(0, 0, m_GunBarrelMinZOffset);
	const Vector delta = bottom - gunBase;

	Vector angles;
	VectorAngles(delta, angles);
	NormalizeAngles(angles);

	if ( angles[PITCH] > MAX_SEARCH_UPWARD_PITCH )
	{
		// Target is too high.
		return false;
	}

	return true;
}

TraceResult CNPCRoninTurret::TraceSightToEnemy(CBaseEntity* ent) const
{
	TraceResult result;
	memset(&result, 0, sizeof(result));

	if ( !ent || !ent->pev || FBitSet(ent->pev->flags, FL_NOTARGET) )
	{
		return result;
	}

	// Don't look through water
	if ( (pev->waterlevel != 3 && ent->pev->waterlevel == 3) || (pev->waterlevel == 3 && ent->pev->waterlevel == 0) )
	{
		return result;
	}

	Vector eyePos = GetEyePos();
	Vector targetPos = ent->BodyTarget(eyePos);
	UTIL_TraceLine(eyePos, targetPos, ignore_monsters, ignore_glass, ENT(pev), &result);

	return result;
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
	return GetAttachmentPos(ATTACHMENT_BARREL);
}

Vector CNPCRoninTurret::GetGunBodyPos() const
{
	return GetAttachmentPos(ATTACHMENT_GUN_BODY);
}

Vector CNPCRoninTurret::GetAttachmentPos(int attachment) const
{
	Vector out;
	Vector dummyAngles;
	GetAttachment(attachment, out, dummyAngles);
	return out;
}

Vector CNPCRoninTurret::GetGunBarrelAngles() const
{
	Vector currentAngles(pev->angles);
	currentAngles += m_CurrentGunAngles;
	NormalizeAngles(currentAngles);
	return currentAngles;
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
	static const float defaultValue = tanf(DEG2RADF(DEFAULT_BULLET_SPREAD_DEGREES));
	return !std::isnan(m_SpreadCone) ? m_SpreadCone : defaultValue;
}

float CNPCRoninTurret::GetRotationSpeed() const
{
	return !std::isnan(m_RotationSpeed) ? m_RotationSpeed : DEFAULT_ROT_DEGREES_PER_SECOND;
}

void CNPCRoninTurret::EmitDisablingSmoke() const
{
	const Vector pos = GetGunBodyPos();

	MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY);
	WRITE_BYTE(TE_SMOKE);
	WRITE_COORD(pos[VEC3_X]);
	WRITE_COORD(pos[VEC3_Y]);
	WRITE_COORD(pos[VEC3_Z]);
	WRITE_SHORT(g_sModelIndexSmoke);
	WRITE_BYTE(25);
	WRITE_BYTE(10);
	MESSAGE_END();
}

void CNPCRoninTurret::CommunicateStateToOwner()
{
	if ( !pev->owner )
	{
		return;
	}

	CBasePlayer* player = GetClassPtr<CBasePlayer>(VARS(pev->owner));

	if ( !player )
	{
		return;
	}

	CWeaponRonin* weapon = dynamic_cast<CWeaponRonin*>(
		player->GetNamedItem(WeaponAtts::StaticWeaponAttributes<CWeaponRonin>().Core.Classname)
	);

	if ( !weapon )
	{
		return;
	}

	weapon->RefreshStateFromTurret((pev->flags & FL_KILLME) ? nullptr : this);
}
