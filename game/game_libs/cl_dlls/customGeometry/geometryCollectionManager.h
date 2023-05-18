#pragma once

#include <memory>
#include <functional>
#include "customGeometry/baseGeometryCollection.h"

namespace CustomGeometry
{
	class CGeometryCollectionManager
	{
	public:
		typedef std::function<CBaseGeometryCollection*()> CollectionFactoryFunc;

		static CGeometryCollectionManager& StaticInstance();

		void SetFactoryFunction(Category category, CollectionFactoryFunc function);
		void Initialise();
		void VidInit();

		CBaseGeometryCollection* CollectionForCategory(Category category) const;

	private:
		struct CollectionEntry
		{
			std::unique_ptr<CBaseGeometryCollection> collection;
			CollectionFactoryFunc factory;
		};

		CollectionEntry m_Entries[CATEGORY_COUNT];
	};
}  // namespace CustomGeometry
