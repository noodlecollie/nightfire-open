#pragma once

#include "MathLib/vec3.h"
#include <memory>

struct edict_s;

namespace Botrix
{
	class CEnemyEntity
	{
	public:
		explicit CEnemyEntity(struct edict_s* edict);
		~CEnemyEntity();

		struct edict_s* GetEdict() const;

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

		std::unique_ptr<IWrapper> m_Wrapper;
	};
}  // namespace Botrix
