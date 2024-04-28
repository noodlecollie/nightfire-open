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
	static constexpr float ACTIVE_THINK_INTERVAL = 0.1f;

	void EXPORT RoninUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

	void ActiveThink();

	void BeginDeploy();
	void DeployFinished();
	void BeginUndeploy();
	void UndeployFinished();

	void SetSequence(NPCRoninTurretAnimations_e index);
	CBaseEntity* FindBestTarget();
	void AttackTarget();
	bool EnemyVisible(CBaseEntity* ent) const;
	Vector GetBestTargetPosition(float minUnitsDevFromTarget, float maxUnitsDevFromTarget) const;
	Vector GetEyePos() const;

	static float GetSearchRange();

	// Exists separately from normal FOV member, so that we can tell
	// if we parsed a KV value or not.
	float m_KVSightFOV = NAN;

	float m_ShootFOV = NAN;

	DeployState m_DeployState = DeployState::NOT_DEPLOYED;
	EHANDLE m_hEnemy;
};
