#pragma once

#include "standard_includes.h"
#include "weapons/ronin_common_defs.h"

extern cvar_t sv_ronin_slide_friction;
extern cvar_t sv_ronin_death_delay;

enum NPCRoninTurretAnimations_e
{
	NPCRONIN_IDLE1,
	NPCRONIN_TOSS_SPIN,
	NPCRONIN_DEPLOY,
	NPCRONIN_FIRE,
	NPCRONIN_DEPLOY_IDLE,
};

class CNPCRoninTurret : public CBaseMonster
{
public:
	enum class DeployState
	{
		TOSSED,  // Thrown but not deployed yet
		DEPLOYING,
		DEPLOYED,
		UNDEPLOYING,
		UNDEPLOYED  // Was previously deployed but has now been put away
	};

	// Bullets per second
	static constexpr float DEFAULT_FIRE_RATE = 8.0f;
	static constexpr float MAX_FIRE_RATE = 20.0f;

	static void RegisterCvars();

	int BloodColor(void) override;
	void GibMonster(void) override;
	int Classify(void) override;
	int ObjectCaps(void) override;
	void KeyValue(KeyValueData* data) override;
	void Killed(entvars_t* pevAttacker, int iGib) override;

	void Spawn(void) override;
	void Precache(void) override;

	bool AllowsPickupWhenUndeployed() const;
	void SetAllowsPickupWhenUndeployed(bool allow);

	bool AllowUndeployWithUseKey() const;
	void SetAllowUndeployWithUseKey(bool allow);

	bool Explode();
	void SetExplosionDamage(int damage);

	static TYPEDESCRIPTION m_SaveData[];

	// These functions do nothing if the Ronin is not
	// in the correct state for the action to begin.
	void ToggleDeploy();
	void DeployNow();
	void UndeployNow();

	DeployState GetDeployState() const;
	bool IsUndeployed() const;
	bool IsFullyDeployed() const;
	Vector GetCentre() const;

	void StartToss(const Vector& velocity, const Vector& angularVelocity);
	void StartToss(const Vector& origin, const Vector& velocity, const Vector& angularVelocity);
	void Place(const Vector& origin, const Vector& angles, float fov);

private:
	static constexpr int ATTACHMENT_BARREL = 0;
	static constexpr int ATTACHMENT_GUN_BODY = 1;

	// Deploy/undeploy
	static constexpr float DEPLOY_DURATION = 1.8f;
	static constexpr float UNDEPLOY_DURATION = 0.5f;

	// Not allowed to go all the way to 90 degrees spread,
	// since the spread is calculated using tan().
	static constexpr float MAX_HALF_BULLET_SPREAD_DEGREES = 89.0f;

	// Think frequency
	static constexpr float THINK_INTERVAL = 0.1f;

	// Death params
	static constexpr float DYING_SPARK_INTERVAL = 0.8f;
	static constexpr float NEAR_DEATH_SPARK_INTERVAL = 0.1f;
	static constexpr float NEAR_DEATH_DURATION = 1.5f;

	// Maximum amount of pitch that can be applied to the gun barrel.
	static constexpr float MAX_BARREL_UPWARD_PITCH = 30.0f;

	// This only applies when the Ronin is dying. Usually, the gun
	// cannot look lower than the horizon.
	static constexpr float MAX_BARREL_DOWNWARD_PITCH = 70.0f;

	// Maximum amount of pitch that will be used to track enemies.
	// This is different to the barrel's max pitch, because it looks
	// more natural if the Ronin still fires at targets that are
	// slightly too high for it to actually reach.
	static constexpr float MAX_SEARCH_UPWARD_PITCH = 60.0f;

	// Default parameter values:
	static constexpr float DEFAULT_SEARCH_RANGE = 500.0f;
	static constexpr float DEFAULT_BULLET_SPREAD_DEGREES = 1.0f;
	static constexpr float DEFAULT_ROT_DEGREES_PER_SECOND = 180.0f;
	static constexpr float DEFAULT_SIGHT_FOV = 150.0f;
	static constexpr float DEFAULT_SHOOT_FOV = 20.0f;
	static constexpr int DEFAULT_EXPLOSION_DAMAGE = 120;

	// A value less than any other dot product we should encounter.
	static constexpr float FOV_SEARCH_ANYWHERE = -2.0f;

	// Movement
	static constexpr int MOVETYPE_IN_TOSS = MOVETYPE_BOUNCE;
	static constexpr float DEFAULT_FRICTION = 0.9f;

	// Spawn flags
	static constexpr int SF_ALLOW_PICKUP = (1 << 0);
	static constexpr int SF_START_DEPLOYED = (1 << 1);
	static constexpr int SF_UNDEPLOY_WITH_USE_KEY = (1 << 2);

	void EXPORT RoninUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

	void MainThink();
	void DeployingThink();
	void UndeployingThink();
	void ActiveThink();
	void AttackThink();
	void DyingThink();

	void BeginDeploy();
	void DeployFinished();
	void BeginUndeploy();
	void UndeployFinished();

	void OnTouch(CBaseEntity* other);

	void SetSequence(NPCRoninTurretAnimations_e index);
	void SetCurrentGunAngles(const Vector& angles);
	void UpdateModelControllerValues();
	void UpdateVelocity();
	bool CanTryToPickUp(CBaseEntity* activator) const;
	CBaseEntity* FindBestTarget();
	void RotateTowardsTarget(const Vector& targetPos);
	void RotateTowardsIdealAngles(const Vector& angles, bool canPointDown = false);
	void FireGun();
	void UpdateBodyAndSkin();
	void SetDeployState(DeployState state);
	bool TargetIsInShootFOV(const Vector& targetPos) const;
	bool EnemyVisible(CBaseEntity* ent) const;
	TraceResult TraceSightToEnemy(CBaseEntity* ent) const;
	Vector GetBestTargetPosition(float minUnitsDevFromTarget, float maxUnitsDevFromTarget) const;
	Vector GetEyePos() const;
	Vector GetGunBarrelPos() const;
	Vector GetAttachmentPos(int attachment) const;
	Vector GetGunBodyPos() const;
	Vector GetGunBarrelAngles() const;
	float GetSearchRange() const;
	float GetFireInterval() const;
	float GetSpreadCone() const;
	float GetRotationSpeed() const;
	void EmitDisablingSmoke() const;
	void CommunicateStateToOwner();

	// Exists separately from normal FOV member, so that we can tell
	// if we parsed a KV value or not.
	float m_KVSightFOV = NAN;

	float m_ShootFOV = NAN;
	float m_SearchRange = NAN;
	float m_FireInterval = NAN;
	float m_SpreadCone = NAN;
	float m_RotationSpeed = NAN;
	int m_ExplosionDamage = DEFAULT_EXPLOSION_DAMAGE;

	DeployState m_DeployState = DeployState::UNDEPLOYED;
	float m_DeploySequenceEnd = NAN;

	// Gun angles, relative to the overall entity angles
	Vector m_CurrentGunAngles;
	float m_LastActiveThink = NAN;
	float m_GunBarrelMinZOffset = 0.0f;

	Vector m_LastTossedPos;
	float m_LastTossedTime = 0.0f;
	float m_NextDyingTick = 0.0f;
	float m_TargetSelfDestructionTime = 0.0f;

	bool m_AllowPickupWhenUndeployed = false;
	bool m_AllowUndeployWithUseKey = false;
};
