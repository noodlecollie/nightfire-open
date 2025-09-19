#pragma once

#include "MathLib/vec3.h"
#include "utlvector.h"
#include "utllinkedlist.h"
#include <memory>

struct edict_s;

namespace Botrix
{
	class CEnemyEntity
	{
	public:
		explicit CEnemyEntity(struct edict_s* edict);
		CEnemyEntity(const CEnemyEntity& other);
		CEnemyEntity(CEnemyEntity&& other);
		~CEnemyEntity();

		CEnemyEntity& operator=(const CEnemyEntity& other);
		CEnemyEntity& operator=(CEnemyEntity&& other);

		struct edict_s* GetEdict() const;
		bool IsPlayerEdict() const;
		bool IsRoninEdict() const;

		bool IsValid() const;
		bool IsBot() const;
		bool IsAlive() const;
		int GetTeam() const;
		Vector GetHead() const;
		Vector GetCentre() const;
		Vector GetFoot() const;
		int GetHealth() const;

	private:
		struct IWrapper;
		struct PlayerWrapper;
		struct RoninWrapper;

		void CreateFrom(struct edict_s* edict);

		std::unique_ptr<IWrapper> m_Wrapper;
	};

	class CEnemyEntities
	{
	public:
		enum class EntityType
		{
			Invalid,
			Player,
			RoninTurret,
		};

		static EntityType GetType(struct edict_s* edict);

		bool Add(struct edict_s* edict);
		bool Remove(struct edict_s* edict);
		void Clear();

		const CEnemyEntity* Find(struct edict_s* edict) const;
		CEnemyEntity* Find(struct edict_s* edict);

	private:
		int FindIndex(struct edict_s* edict) const;

		CUtlLinkedList<CEnemyEntity> m_Entities;
	};
}  // namespace Botrix
