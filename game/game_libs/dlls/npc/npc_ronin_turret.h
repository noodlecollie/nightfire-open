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

	static constexpr float THINK_INTERVAL_DEPLOYING = 0.25f;
	static constexpr float DEPLOY_DURATION = 1.0f;
	static constexpr float UNDEPLOY_DURATION = 0.5f;

	void EXPORT RoninUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

	void BeginDeploy();
	void DeployFinished();
	void BeginUndeploy();
	void UndeployFinished();

	void SetSequence(NPCRoninTurretAnimations_e index);

	DeployState m_DeployState = DeployState::NOT_DEPLOYED;
};
