#pragma once

#include "standard_includes.h"

enum NPCRoninTurretAnimations_e
{
	NPCRONIN_IDLE1,
	NPCRONIN_SPIN,
	NPCRONIN_DEPLOY,
	NPCRONIN_FIRE,
	NPCRONIN_DEPLOY_IDLE,
};

class CNPCRoninTurret : public CBaseMonster
{
public:
	static constexpr float DEFAULT_SEARCH_RANGE = 300.0f;
	static constexpr float DEFAULT_BULLET_SPREAD_DEGREES = 10.0f;
	static constexpr float DEFAULT_ROT_DEGREES_PER_SECOND = 360.0f;

	// Bullets per second
	static constexpr float DEFAULT_FIRE_RATE = 8.0f;
	static constexpr float MAX_FIRE_RATE = 20.0f;

	int BloodColor(void) override;
	void GibMonster(void) override;
	int Classify(void) override;
	int ObjectCaps(void) override;
	void KeyValue(KeyValueData* data) override;

	void Spawn(void) override;
	void Precache(void) override;

	static TYPEDESCRIPTION m_SaveData[];

	// These functions do nothing if the Ronin is not
	// in the correct state for the action to begin.
	void ToggleDeploy();
	void DeployNow();
	void UndeployNow();

private:
	enum class DeployState
	{
		NOT_DEPLOYED,
		DEPLOYING,
		DEPLOYED,
		UNDEPLOYING
	};

	static constexpr float DEPLOY_DURATION = 1.0f;
	static constexpr float UNDEPLOY_DURATION = 0.5f;

	// Not allowed to go all the way to 90 degrees spread,
	// since the spread is calculated using tan().
	static constexpr float MAX_HALF_BULLET_SPREAD_DEGREES = 89.0f;

	// Don't allow us to think less frequently than this,
	// or enemy tracking would suffer.
	static constexpr float MAX_ACTIVE_THINK_INTERVAL = 0.1f;

	// Maximum amount of pitch that can be applied to the gun barrel.
	static constexpr float MAX_PITCH_DEVIATION = 15.0f;

	void EXPORT RoninUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

	void ActiveThink();

	void BeginDeploy();
	void DeployFinished();
	void BeginUndeploy();
	void UndeployFinished();

	void SetSequence(NPCRoninTurretAnimations_e index);
	void UpdateModelControllerValues();
	CBaseEntity* FindBestTarget();
	void RotateTowardsTarget();
	void AttackTarget();
	bool EnemyVisible(CBaseEntity* ent) const;
	Vector GetBestTargetPosition(float minUnitsDevFromTarget, float maxUnitsDevFromTarget) const;
	Vector GetEyePos() const;
	Vector GetGunBarrelPos() const;
	float GetBestThinkInterval() const;
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

	// Gun angles, relative to the overall entity angles
	Vector m_CurrentGunAngles;
	float m_LastAngleUpdate = NAN;
};
