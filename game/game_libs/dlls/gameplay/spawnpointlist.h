#pragma once

#include <cstdint>
#include "standard_includes.h"
#include "utlvector.h"
#include "utlstring.h"

class CSpawnPointList
{
public:
	CSpawnPointList();

	void Initialise(const CUtlString& className);

	// Controls the sequence of indices used when calling GetNextSpawnPoint().
	// Sequential means spawn points are returned in the order they were originally found.
	// Randomise means they will be returned in a random order.
	void MakeIndicesSequential();
	void RandomiseIndices();

	// Returns the next spawn point and increments the index.
	CBaseEntity* GetNextSpawnPoint();

	// The index here is directly into the spawn point list,
	// not indirected via the indices list.
	uint32_t NextSpawnPointIndex() const;
	CBaseEntity* SpawnPointAtIndex(uint32_t index) const;

	size_t SpawnPointCount() const;

private:
	void FindAllSpawnPoints();

	CUtlString m_SpawnPointClassName;
	CUtlVector<EHANDLE> m_SpawnPoints;
	CUtlVector<uint32_t> m_Indices;
	uint32_t m_iNextSpawnPointIndex;
};
