#pragma once

#include "standard_includes.h"
#include "utlhashmap.h"
#include "utlvector.h"

class CObserverPointList
{
public:
	void Initialise();

	edict_t* GetNextObserverPointWithIndex(int32_t index);
	edict_t* GetObserverPoint(int32_t index, int32_t subIndex) const;
	size_t GetNumObserverPointsWithIndex(int32_t index) const;

private:
	using HashMap = CUtlHashMap<int32_t, CUtlVector<EHANDLE>>;

	HashMap m_ObserverPointsByIndex;
	int32_t m_lastQueriedObserverPointIndex = 0;
	size_t m_nextListIndex = 0;
};
