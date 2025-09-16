#include "botrix/enemyentity.h"
#include "standard_includes.h"
#include "npc/npc_ronin_turret.h"
#include "botrix/playerinfo.h"
#include "botrix/parameter_vars.h"

namespace Botrix
{
	struct CEnemyEntity::IWrapper
	{
		virtual ~IWrapper() = default;

		virtual edict_t* GetEdict() const = 0;
		virtual bool IsBot() const = 0;
		virtual bool IsAlive() const = 0;
		virtual int GetTeam() const = 0;
		virtual Vector GetHead() const = 0;
		virtual Vector GetCentre() const = 0;
		virtual Vector GetFoot() const = 0;
		virtual int GetHealth() const = 0;
	};

	struct CEnemyEntity::PlayerWrapper : public CEnemyEntity::IWrapper
	{
		CBasePlayer* m_Player = nullptr;

		explicit PlayerWrapper(CBasePlayer* player) :
			m_Player(player)
		{
		}

		virtual ~PlayerWrapper() = default;

		edict_t* GetEdict() const override
		{
			return m_Player ? m_Player->edict() : nullptr;
		}

		bool IsBot() const override
		{
			return m_Player ? m_Player->IsFakeClient() : false;
		}

		bool IsAlive() const override
		{
			return m_Player ? m_Player->IsAlive() : false;
		}

		int GetTeam() const override
		{
			return m_Player ? CPlayerInfo(m_Player->edict()).GetTeamIndex() : -1;
		}

		Vector GetHead() const override
		{
			if ( !m_Player )
			{
				return Vector();
			}

			Vector pos = m_Player->pev->origin;

			// Use parameter vars here rather than the view offset, since we
			// want to shoot at where we expect the player's head hitbox to be,
			// rather than where the engine has currently placed their camera.

			if ( IsDucking() )
			{
				pos.z += CBotrixParameterVars::PLAYER_EYE_CROUCHED_FROM_ORIGIN;
			}
			else
			{
				pos.z += CBotrixParameterVars::PLAYER_EYE_FROM_ORIGIN;
			}

			return pos;
		}

		Vector GetCentre() const override
		{
			return m_Player ? Vector(m_Player->pev->origin) : Vector();
		}

		Vector GetFoot() const override
		{
			return m_Player
				? (Vector(m_Player->pev->origin) - Vector(0.0f, 0.0f, CBotrixParameterVars::PLAYER_HEIGHT / 2.0f))
				: Vector();
		}

		int GetHealth() const override
		{
			return m_Player ? CPlayerInfo(m_Player->edict()).GetHealth() : 0;
		}

	private:
		bool IsDucking() const
		{
			if ( !m_Player )
			{
				return false;
			}

			return FBitSet(m_Player->pev->flags, FL_DUCKING) || (m_Player->m_afPhysicsFlags & PFLAG_DUCKING);
		}
	};

	struct CEnemyEntity::RoninWrapper : public CEnemyEntity::IWrapper
	{
		CNPCRoninTurret* m_Ronin = nullptr;

		explicit RoninWrapper(CNPCRoninTurret* ronin) :
			m_Ronin(ronin)
		{
		}

		~RoninWrapper() = default;

		edict_t* GetEdict() const override
		{
			return m_Ronin ? m_Ronin->edict() : nullptr;
		}

		bool IsBot() const override
		{
			return false;
		}

		bool IsAlive() const override
		{
			return m_Ronin ? (m_Ronin->GetDeployState() != CNPCRoninTurret::DeployState::DEPLOYED) : false;
		}

		int GetTeam() const override
		{
			if ( !m_Ronin )
			{
				return -1;
			}

			edict_t* owner = m_Ronin->pev->owner;

			if ( !owner )
			{
				return -1;
			}

			return CPlayerInfo(owner).GetTeamIndex();
		}

		Vector GetHead() const override
		{
			return GetCentre();
		}

		Vector GetCentre() const override
		{
			return m_Ronin ? m_Ronin->GetCentre() : Vector();
		}

		Vector GetFoot() const override
		{
			return m_Ronin ? Vector(m_Ronin->pev->origin) : Vector();
		}

		int GetHealth() const override
		{
			return m_Ronin ? static_cast<int>(m_Ronin->pev->health) : 0;
		}
	};

	CEnemyEntity::CEnemyEntity(struct edict_s* edict)
	{
		if ( !edict )
		{
			return;
		}

		CBaseEntity* baseEnt = CBaseEntity::Instance(edict);

		if ( baseEnt->Classify() == CLASS_PLAYER )
		{
			CBasePlayer* player = dynamic_cast<CBasePlayer*>(baseEnt);
			ASSERT(player);

			m_Wrapper.reset(new PlayerWrapper(player));
		}
		else if ( FStrEq(STRING(edict->v.classname), "npc_ronin_turret") )
		{
			CNPCRoninTurret* ronin = dynamic_cast<CNPCRoninTurret*>(baseEnt);
			ASSERT(ronin);

			m_Wrapper.reset(new RoninWrapper(ronin));
		}
		else
		{
			ASSERT(false);
		}
	}

	CEnemyEntity::~CEnemyEntity()
	{
	}

	struct edict_s* CEnemyEntity::GetEdict() const
	{
		return m_Wrapper ? m_Wrapper->GetEdict() : nullptr;
	}

	bool CEnemyEntity::IsBot() const
	{
		return m_Wrapper ? m_Wrapper->IsBot() : false;
	}

	bool CEnemyEntity::IsAlive() const
	{
		return m_Wrapper ? m_Wrapper->IsAlive() : false;
	}

	int CEnemyEntity::GetTeam() const
	{
		return m_Wrapper ? m_Wrapper->GetTeam() : -1;
	}

	Vector CEnemyEntity::GetHead() const
	{
		return m_Wrapper ? m_Wrapper->GetHead() : Vector();
	}

	Vector CEnemyEntity::GetCentre() const
	{
		return m_Wrapper ? m_Wrapper->GetCentre() : Vector();
	}

	Vector CEnemyEntity::GetFoot() const
	{
		return m_Wrapper ? m_Wrapper->GetFoot() : Vector();
	}

	int CEnemyEntity::GetHealth() const
	{
		return m_Wrapper ? m_Wrapper->GetHealth() : 0;
	}
}  // namespace Botrix
