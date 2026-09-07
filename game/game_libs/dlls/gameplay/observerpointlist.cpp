#include "gameplay/observerpointlist.h"
#include "gameplay/observerpoint.h"

void CObserverPointList::Initialise()
{
	m_ObserverPointsByIndex.Purge();
	m_lastQueriedObserverPointIndex = 0;
	m_nextListIndex = 0;

	for ( CObserverPoint* observerPoint =
			  dynamic_cast<CObserverPoint*>(UTIL_FindEntityByClassname(nullptr, CObserverPoint::CLASSNAME));
		  !FNullEnt(observerPoint);
		  observerPoint =
			  dynamic_cast<CObserverPoint*>(UTIL_FindEntityByClassname(observerPoint, CObserverPoint::CLASSNAME)) )
	{
		EHANDLE handle;
		handle = observerPoint;

		const int hashIndex = m_ObserverPointsByIndex.FindOrInsert(observerPoint->Index());
		CUtlVector<EHANDLE>& list = m_ObserverPointsByIndex.Element(hashIndex);
		list.AddToTail(handle);
	}
}

edict_t* CObserverPointList::GetNextObserverPointWithIndex(int32_t index)
{
	const int hashIndex = m_ObserverPointsByIndex.Find(index);

	if ( hashIndex == HashMap::InvalidIndex() )
	{
		return nullptr;
	}

	const CUtlVector<EHANDLE>& list = m_ObserverPointsByIndex.Element(hashIndex);

	if ( list.IsEmpty() )
	{
		ASSERT(false);
		return nullptr;
	}

	if ( index != m_lastQueriedObserverPointIndex )
	{
		m_lastQueriedObserverPointIndex = index;
		m_nextListIndex = 0;
	}

	if ( m_nextListIndex >= static_cast<size_t>(list.Count()) )
	{
		m_nextListIndex = 0;
	}

	return list[static_cast<int32_t>(m_nextListIndex)].Get();
}

edict_t* CObserverPointList::GetObserverPoint(int32_t index, int32_t subIndex) const
{
	const int hashIndex = m_ObserverPointsByIndex.Find(index);

	if ( hashIndex == HashMap::InvalidIndex() )
	{
		return nullptr;
	}

	const CUtlVector<EHANDLE>& list = m_ObserverPointsByIndex.Element(hashIndex);

	if ( subIndex < 0 || subIndex >= list.Count() )
	{
		return nullptr;
	}

	return list[subIndex].Get();
}
