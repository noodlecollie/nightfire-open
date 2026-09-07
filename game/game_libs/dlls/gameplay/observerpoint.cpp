#include "gameplay/observerpoint.h"

LINK_ENTITY_TO_CLASS(info_observer_point, CObserverPoint)

TYPEDESCRIPTION CObserverPoint::m_SaveData[] = {
	DEFINE_FIELD(CObserverPoint, m_index, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CObserverPoint, CPointEntity)

void CObserverPoint::Precache()
{
	CPointEntity::Precache();
}

void CObserverPoint::KeyValue(KeyValueData* pkvd)
{
	if ( FStrEq(pkvd->szKeyName, "index") )
	{
		m_index = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
	{
		CPointEntity::KeyValue(pkvd);
	}
}

void CObserverPoint::Spawn(void)
{
	// MENUMAP TODO: Need to set a flag here to include this entity
	// in delta packets. Currently, if the entity has the NODRAW
	// effect, does not have a model, or is not in the client's
	// PVS, the client will not receive the entity's origin/angles.
	pev->movetype = MOVETYPE_NOCLIP;
	pev->solid = SOLID_NOT;
	pev->effects |= EF_NODRAW;
}

int CObserverPoint::Index() const
{
	return m_index;
}
