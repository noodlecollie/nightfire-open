#include "customGeometry/geometryCollectionManager.h"

namespace CustomGeometry
{
	CGeometryCollectionManager& CGeometryCollectionManager::StaticInstance()
	{
		static CGeometryCollectionManager manager;
		return manager;
	}

	void CGeometryCollectionManager::SetFactoryFunction(Category category, CollectionFactoryFunc function)
	{
		if ( category > Category::None && category < Category::CategoryCount )
		{
			m_Entries[static_cast<uint32_t>(category)].factory = function;
		}
	}

	void CGeometryCollectionManager::Initialise()
	{
		for ( uint32_t index = 1; index < CATEGORY_COUNT; ++index )
		{
			CollectionEntry& entry = m_Entries[index];

			if ( entry.factory )
			{
				entry.collection.reset(entry.factory());
			}
			else
			{
				entry.collection.reset(new CBaseGeometryCollection());
			}
		}
	}

	void CGeometryCollectionManager::VidInit()
	{
		for ( uint32_t index = 1; index < CATEGORY_COUNT; ++index )
		{
			CollectionEntry& entry = m_Entries[index];

			if ( entry.collection )
			{
				entry.collection->VidInit();
			}
		}
	}

	CBaseGeometryCollection* CGeometryCollectionManager::CollectionForCategory(Category category) const
	{
		if ( category <= Category::None || category >= Category::CategoryCount )
		{
			return nullptr;
		}

		return m_Entries[static_cast<uint32_t>(category)].collection.get();
	}
}  // namespace CustomGeometry
