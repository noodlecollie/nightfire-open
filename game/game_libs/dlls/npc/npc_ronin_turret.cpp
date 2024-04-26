#include "npc/npc_ronin_turret.h"
#include "monsters.h"

static constexpr const char* const RONIN_MODEL = "models/weapon_ronin/w_ronin.mdl";

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
	pev->takedamage = static_cast<float>(pev->health > 0.0f ? DAMAGE_AIM : DAMAGE_NO);

	SetBits(pev->flags, FL_MONSTER);
	SetUse(&CNPCRoninTurret::RoninUse);

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
	SetThink(nullptr);
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
