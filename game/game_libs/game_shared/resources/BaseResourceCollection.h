#pragma once

#include <cstdint>
#include <cstddef>
#include "utlvector.h"
#include "utlstring.h"
#include "projectInterface/IProjectInterface.h"
#include "IterableResourceCollection.h"

template<typename ENUM, size_t N>
class CBaseResourceCollection : public IIterableResourceCollection
{
public:
	using ResourceId = ENUM;
	static constexpr size_t NUM_ENTRIES = N;

	CBaseResourceCollection() :
		IIterableResourceCollection(m_ResourcePaths, NUM_ENTRIES)
	{
	}

	inline size_t ResourceCount(ResourceId id) const
	{
		const uint32_t intId = static_cast<uint32_t>(id);
		return intId < NUM_ENTRIES ? m_ResourcePaths[intId].Count() : 0;
	}

	const char* ResourcePath(ResourceId id, uint32_t index = 0) const
	{
		const uint32_t intId = static_cast<uint32_t>(id);

		if ( intId >= NUM_ENTRIES )
		{
			return nullptr;
		}

		const CUtlVector<CUtlString>& list = m_ResourcePaths[intId];

		if ( index >= list.Count() )
		{
			return nullptr;
		}

		return list[index].String();
	}

	const char* RandomResourcePath(ResourceId id) const
	{
		const uint32_t intId = static_cast<uint32_t>(id);

		if ( intId >= NUM_ENTRIES )
		{
			return nullptr;
		}

		const CUtlVector<CUtlString>& list = m_ResourcePaths[intId];

		if ( list.Count() < 1 )
		{
			return nullptr;
		}

		IProjectInterface* ifc = IProjectInterface::ProjectInterfaceImpl();
		int32_t index = ifc->RNG().GetRandomInt(0, list.Count() - 1);
		return list[index].String();
	}

protected:
	template<size_t LEN>
	inline void InitialiseResources(ResourceId id, const char* const (&array)[LEN])
	{
		InitialiseResources(id, array, LEN);
	}

	inline void InitialiseResource(ResourceId id, const char* const path)
	{
		InitialiseResources(id, &path, 1);
	}

	void InitialiseResources(ResourceId id, const char* const* paths, size_t pathsLength)
	{
		const uint32_t intId = static_cast<uint32_t>(id);

		CUtlVector<CUtlString>& list = m_ResourcePaths[intId];
		list.Purge();

		for ( uint32_t index = 0; index < pathsLength; ++index )
		{
			list.AddToTail(CUtlString(paths[index]));
		}
	}

	CUtlVector<CUtlString> m_ResourcePaths[NUM_ENTRIES];
};
