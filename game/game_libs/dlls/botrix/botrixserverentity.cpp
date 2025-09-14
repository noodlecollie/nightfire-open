#include "botrix/botrixserverentity.h"
#include "standard_includes.h"
#include "weapons.h"
#include "MathLib/mathlib.h"
#include <algorithm>

namespace Botrix
{
	CBotrixServerEntity::CBotrixServerEntity(struct edict_s* edict) :
		m_pEdict(edict)
	{
	}

	CBaseEntity* CBotrixServerEntity::GetBaseEntity() const
	{
		return CBaseEntity::Instance(m_pEdict);
	}

	CBasePlayerItem* CBotrixServerEntity::GetBasePlayerItem() const
	{
		return dynamic_cast<CBasePlayerItem*>(GetBaseEntity());
	}

	struct edict_s* CBotrixServerEntity::GetEdict() const
	{
		return m_pEdict;
	}

	Vector CBotrixServerEntity::GetOrigin() const
	{
		if ( !m_pEdict )
		{
			return Vector();
		}

		return m_pEdict->v.origin;
	}

	Vector CBotrixServerEntity::GetMins() const
	{
		if ( !m_pEdict )
		{
			return Vector();
		}

		return m_pEdict->v.mins;
	}

	Vector CBotrixServerEntity::GetMaxs() const
	{
		if ( !m_pEdict )
		{
			return Vector();
		}

		return m_pEdict->v.maxs;
	}

	float CBotrixServerEntity::GetCollisionRadiusSquared() const
	{
		if ( !m_pEdict )
		{
			return 0.0f;
		}

		return std::max<float>(GetMaxs().LengthSquared(), GetMins().LengthSquared());
	}

	float CBotrixServerEntity::GetCollisionRadius() const
	{
		if ( !m_pEdict )
		{
			return 0.0f;
		}

		return rsqrt(GetCollisionRadiusSquared());
	}
}  // namespace Botrix
