#pragma once

#include "standard_includes.h"

extern cvar_t sv_ronin_slide_friction;

enum NPCRoninTurretAnimations_e
{
	NPCRONIN_IDLE1,
	NPCRONIN_TOSS_SPIN,
	NPCRONIN_DEPLOY,
	NPCRONIN_FIRE,
	NPCRONIN_DEPLOY_IDLE,
};

enum NPCRoninTurretBody_e
{
	NPCRONIN_BODY_ALL_LIGHTS,
	NPCRONIN_BODY_REAR_LIGHTS,
	NPCRONIN_BODY_FRONT_LIGHTS,
	NPCRONIN_BODY_NO_LIGHTS,
};

enum NPCRoninTurretSkin_e
{
	NPCRONIN_SKIN_RED_LIGHTS,
	NPCRONIN_SKIN_ORANGE_LIGHTS,
	NPCRONIN_SKIN_NO_LIGHTS,
};

class CNPCRoninTurret : public CBaseMonster
{
public:
	static constexpr float DEFAULT_SEARCH_RANGE = 300.0f;
	static constexpr float DEFAULT_BULLET_SPREAD_DEGREES = 10.0f;
	static constexpr float DEFAULT_ROT_DEGREES_PER_SECOND = 130.0f;

	// Bullets per second
	static constexpr float DEFAULT_FIRE_RATE = 8.0f;
	static constexpr float MAX_FIRE_RATE = 20.0f;

	static void RegisterCvars();

	int BloodColor(void) override;
	void GibMonster(void) override;
	int Classify(void) override;
	int ObjectCaps(void) override;
	void KeyValue(KeyValueData* data) override;

	void Spawn(void) override;
	void Precache(void) override;

	bool AllowsPickupWhenUndeployed() const;
	void SetAllowsPickupWhenUndeployed(bool allow);

	static TYPEDESCRIPTION m_SaveData[];

	// These functions do nothing if the Ronin is not
	// in the correct state for the action to begin.
	void ToggleDeploy();
	void DeployNow();
	void UndeployNow();

	bool IsUndeployed() const;

	void StartToss(const Vector& velocity, const Vector& angularVelocity);
	void StartToss(const Vector& origin, const Vector& velocity, const Vector& angularVelocity);
	void Place(const Vector& origin, const Vector& angles, float fov);

private:
	enum class DeployState
	{
		NOT_DEPLOYED,
		DEPLOYING,
		DEPLOYED,
		UNDEPLOYING,
	};

	// Deploy/undeploy
	static constexpr float DEPLOY_DURATION = 1.8f;
	static constexpr float UNDEPLOY_DURATION = 0.5f;

	// Not allowed to go all the way to 90 degrees spread,
	// since the spread is calculated using tan().
	static constexpr float MAX_HALF_BULLET_SPREAD_DEGREES = 89.0f;

	// Think frequency
	static constexpr float THINK_INTERVAL = 0.1f;

	// Maximum amount of pitch that can be applied to the gun barrel.
	static constexpr float MAX_BARREL_UPWARD_PITCH = 30.0f;

	// Maximum amount of pitch that will be used to track enemies.
	// This is different to the barrel's max pitch, because it looks
	// more natural if the Ronin still fires at targets that are
	// slightly too high for it to actually reach.
	static constexpr float MAX_SEARCH_UPWARD_PITCH = 60.0f;

	static constexpr float DEFAULT_SIGHT_FOV = 150.0f;
	static constexpr float DEFAULT_SHOOT_FOV = 20.0f;
	static constexpr float DEFAULT_SPREAD_CONE = 1.0f;

	// A value less than any other dot product we should encounter.
	static constexpr float FOV_SEARCH_ANYWHERE = -2.0f;

	// Movement
	static constexpr int MOVETYPE_IN_TOSS = MOVETYPE_BOUNCE;
	static constexpr float DEFAULT_FRICTION = 0.9f;

	// Spawn flags
	static constexpr int SF_ALLOW_PICKUP = (1 << 0);

	void EXPORT RoninUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

	void MainThink();
	void DeployingThink();
	void UndeployingThink();
	void ActiveThink();

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
	bool AddRoninToPlayer(CBasePlayer* player, bool isPickupFromUse);
	bool AddRoninWeaponToPlayer(CBasePlayer* player);
	CBaseEntity* FindBestTarget();
	void RotateTowardsTarget(const Vector& targetPos);
	void FireGun();
	void UpdateBodyAndSkin();
	void SetDeployState(DeployState state);
	bool TargetIsInShootFOV(const Vector& targetPos) const;
	bool EnemyVisible(CBaseEntity* ent) const;
	TraceResult TraceSightToEnemy(CBaseEntity* ent) const;
	Vector GetBestTargetPosition(float minUnitsDevFromTarget, float maxUnitsDevFromTarget) const;
	Vector GetEyePos() const;
	Vector GetGunBarrelPos() const;
	Vector GetGunBarrelAngles() const;
	float GetSearchRange() const;
	float GetFireInterval() const;
	float GetSpreadCone() const;
	float GetRotationSpeed() const;

	// Exists separately from normal FOV member, so that we can tell
	// if we parsed a KV value or not.
	float m_KVSightFOV = NAN;

	float m_ShootFOV = NAN;
	float m_SearchRange = NAN;
	float m_FireInterval = NAN;
	float m_SpreadCone = NAN;
	float m_RotationSpeed = NAN;

	DeployState m_DeployState = DeployState::NOT_DEPLOYED;
	float m_DeploySequenceEnd = NAN;

	// Gun angles, relative to the overall entity angles
	Vector m_CurrentGunAngles;
	float m_LastAngleUpdate = NAN;
	float m_GunBarrelMinZOffset = 0.0f;

	Vector m_LastTossedPos;
	float m_LastTossedTime = 0.0f;
	bool m_AllowPickupWhenUndeployed = false;
};
