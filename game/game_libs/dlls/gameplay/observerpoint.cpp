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
	pev->movetype = MOVETYPE_NOCLIP;
	pev->solid = SOLID_NOT;
	pev->effects |= EF_NODRAW;

	// Always network this entity, so that clients can rely on
	// our positions and angles without needing to see us.
	pev->flags |= FL_ALWAYSPACK;
}

int CObserverPoint::Index() const
{
	return m_index;
}
