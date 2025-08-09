#include "spawnpointlist.h"

CSpawnPointList::CSpawnPointList() :
	m_SpawnPointClassName(),
	m_SpawnPoints(),
	m_iNextSpawnPointIndex(0)
{
}

uint32_t CSpawnPointList::NextSpawnPointIndex() const
{
	return m_Indices.Count() > 0 ? m_Indices[m_iNextSpawnPointIndex] : 0;
}

size_t CSpawnPointList::SpawnPointCount() const
{
	return m_SpawnPoints.Count();
}

CBaseEntity* CSpawnPointList::SpawnPointAtIndex(uint32_t index) const
{
	return index < static_cast<uint32_t>(m_SpawnPoints.Count()) ? (CBaseEntity*)m_SpawnPoints[index] : nullptr;
}

CBaseEntity* CSpawnPointList::IndirectSpawnPointAtIndex(uint32_t index) const
{
	return index < static_cast<uint32_t>(m_Indices.Count()) ? (CBaseEntity*)m_SpawnPoints[m_Indices[index]] : nullptr;
}

void CSpawnPointList::Initialise(const CUtlString& className)
{
	m_SpawnPointClassName = className;

	FindAllSpawnPoints();
	MakeIndicesSequential();
}

CBaseEntity* CSpawnPointList::GetNextSpawnPoint()
{
	if ( m_Indices.Count() < 1 )
	{
		return nullptr;
	}

	uint32_t index = m_iNextSpawnPointIndex;
	m_iNextSpawnPointIndex = (m_iNextSpawnPointIndex + 1) % m_Indices.Count();

	return m_SpawnPoints[m_Indices[index]];
}

void CSpawnPointList::FindAllSpawnPoints()
{
	m_SpawnPoints.Purge();

	for ( CBaseEntity* spawnPoint = UTIL_FindEntityByClassname(nullptr, m_SpawnPointClassName.String());
		  !FNullEnt(spawnPoint);
		  spawnPoint = UTIL_FindEntityByClassname(spawnPoint, m_SpawnPointClassName.String()) )
	{
		EHANDLE handle;
		handle = spawnPoint;
		m_SpawnPoints.AddToTail(handle);
	}

	m_iNextSpawnPointIndex = 0;
}

void CSpawnPointList::MakeIndicesSequential()
{
	m_Indices.Purge();

	FOR_EACH_VEC(m_SpawnPoints, index)
	{
		m_Indices.AddToTail(static_cast<uint32_t>(index));
	}

	m_iNextSpawnPointIndex = 0;
}

void CSpawnPointList::RandomiseIndices()
{
	if ( m_SpawnPoints.Count() < 1 )
	{
		return;
	}

	if ( m_Indices.Count() < 1 )
	{
		MakeIndicesSequential();
	}

	CUtlVector<uint32_t> temp;

	while ( m_Indices.Count() > 0 )
	{
		int index = RANDOM_LONG(0, m_Indices.Count() - 1);
		temp.AddToTail(m_Indices[index]);
		m_Indices.Remove(index);
	}

	m_Indices.EnsureCapacity(temp.Count());

	FOR_EACH_VEC(temp, index)
	{
		m_Indices.AddToTail(temp[index]);
	}

	m_iNextSpawnPointIndex = 0;
}

const CUtlVector<uint32_t>& CSpawnPointList::Indices() const
{
	return m_Indices;
}
