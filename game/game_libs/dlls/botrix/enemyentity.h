#pragma once

#include "MathLib/vec3.h"
#include "utlvector.h"
#include "utllinkedlist.h"
#include <memory>
#include "botrix/types.h"

struct edict_s;

namespace Botrix
{
	class CEnemyEntity
	{
	public:
		enum class EntityType
		{
			Invalid,
			Player,
			RoninTurret,
		};

		static EntityType GetType(struct edict_s* edict);

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

	class CEnemyEntityContainer
	{
	public:
		bool Add(struct edict_s* edict);
		bool Remove(struct edict_s* edict);
		void Clear();
		int Count() const;

		const CEnemyEntity* Get(int index) const;
		CEnemyEntity* Get(int index);

		const CEnemyEntity* Find(struct edict_s* edict) const;
		CEnemyEntity* Find(struct edict_s* edict);
		int FindIndex(struct edict_s* edict) const;
		int GetNextIndex(int index) const;
		int GetPreviousIndex(int index) const;
		bool IsValidIndex(int index) const;

		bool ContainsPlayer(TPlayerIndex playerIndex) const;

	private:
		CUtlLinkedList<CEnemyEntity> m_Entities;
	};
}  // namespace Botrix
