#include "botrix/serverentity.h"
#include "standard_includes.h"
#include "weapons.h"
#include "MathLib/mathlib.h"
#include <algorithm>

namespace Botrix
{
	ServerEntity::ServerEntity(struct edict_s* edict) :
		m_pEdict(edict)
	{
	}

	CBaseEntity* ServerEntity::GetBaseEntity() const
	{
		return CBaseEntity::Instance(m_pEdict);
	}

	CBasePlayerItem* ServerEntity::GetBasePlayerItem() const
	{
		return dynamic_cast<CBasePlayerItem*>(GetBaseEntity());
	}

	struct edict_s* ServerEntity::GetEdict() const
	{
		return m_pEdict;
	}

	Vector ServerEntity::GetOrigin() const
	{
		if ( !m_pEdict )
		{
			return Vector();
		}

		return m_pEdict->v.origin;
	}

	Vector ServerEntity::GetMins() const
	{
		if ( !m_pEdict )
		{
			return Vector();
		}

		return m_pEdict->v.mins;
	}

	Vector ServerEntity::GetMaxs() const
	{
		if ( !m_pEdict )
		{
			return Vector();
		}

		return m_pEdict->v.maxs;
	}

	float ServerEntity::GetCollisionRadiusSquared() const
	{
		if ( !m_pEdict )
		{
			return 0.0f;
		}

		return std::max<float>(GetMaxs().LengthSquared(), GetMins().LengthSquared());
	}

	float ServerEntity::GetCollisionRadius() const
	{
		if ( !m_pEdict )
		{
			return 0.0f;
		}

		return rsqrt(GetCollisionRadiusSquared());
	}
}  // namespace Botrix
