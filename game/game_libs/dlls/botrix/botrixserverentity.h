#pragma once

#include "MathLib/vec3.h"

struct edict_s;

class CBaseEntity;
class CBasePlayerItem;

namespace Botrix
{
	class CBotrixServerEntity
	{
	public:
		explicit CBotrixServerEntity(struct edict_s* edict);

		CBaseEntity* GetBaseEntity() const;
		CBasePlayerItem* GetBasePlayerItem() const;
		struct edict_s* GetEdict() const;

		Vector GetOrigin() const;
		Vector GetMins() const;
		Vector GetMaxs() const;
		float GetCollisionRadiusSquared() const;
		float GetCollisionRadius() const;

	private:
		struct edict_s* m_pEdict = nullptr;
	};
}  // namespace Botrix
