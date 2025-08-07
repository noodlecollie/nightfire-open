#include "hitscanweapondebugging.h"
#include "customGeometry/geometryCollectionManager.h"

namespace
{
	static constexpr uint32_t HITSCAN_LINE_COLOUR = 0x84E2FFFF;

	static CustomGeometry::GeometryItemPtr_t CurrentBatch;

	static inline CustomGeometry::CBaseGeometryCollection* GetCollection()
	{
		using namespace CustomGeometry;

		CGeometryCollectionManager& manager = CGeometryCollectionManager::StaticInstance();
		return manager.CollectionForCategory(Category::HitscanEvents);
	}
}  // namespace

namespace HitscanWeaponDebugging
{
	void Clear()
	{
		using namespace CustomGeometry;

		CBaseGeometryCollection* collection = GetCollection();

		if ( collection && !collection->IsEmpty() )
		{
			collection->Clear();
		}

		CurrentBatch.reset();
	}

	void BeginBatch()
	{
		using namespace CustomGeometry;

		CurrentBatch.reset(new CGeometryItem());

		CurrentBatch->SetColour(HITSCAN_LINE_COLOUR);
		CurrentBatch->SetDrawType(DrawType::Lines);
	}

	void AddTraceToBatch(const Vector& begin, const Vector& end)
	{
		using namespace CustomGeometry;

		if ( !CurrentBatch )
		{
			return;
		}

		CurrentBatch->AddLine(begin, end);
	}

	void EndBatch()
	{
		using namespace CustomGeometry;

		CBaseGeometryCollection* collection = GetCollection();

		if ( collection && CurrentBatch && !CurrentBatch->IsEmpty() )
		{
			collection->AddItem(CurrentBatch);
		}

		CurrentBatch.reset();
	}
}  // namespace HitscanWeaponDebugging
