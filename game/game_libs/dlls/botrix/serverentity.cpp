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

		if ( !VectorIsNull(m_pEdict->v.mins) )
		{
			return m_pEdict->v.mins;
		}

		// Some things like weapons only set the abs bounding box.
		// It's unclear why - I need to look into this.
		if ( GetClassPtr<CBasePlayerItem>(&m_pEdict->v) )
		{
			return Vector(m_pEdict->v.absmin) - Vector(m_pEdict->v.origin);
		}

		return Vector();
	}

	Vector ServerEntity::GetMaxs() const
	{
		if ( !m_pEdict )
		{
			return Vector();
		}

		if ( !VectorIsNull(m_pEdict->v.maxs) )
		{
			return m_pEdict->v.maxs;
		}

		// Some things like weapons only set the abs bounding box.
		// It's unclear why - I need to look into this.
		if ( GetClassPtr<CBasePlayerItem>(&m_pEdict->v) )
		{
			return Vector(m_pEdict->v.absmax) - Vector(m_pEdict->v.origin);
		}

		return Vector();
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
