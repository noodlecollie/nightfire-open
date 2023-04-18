#include "baseGeometryCollection.h"
#include "customGeometry/geometryRenderer.h"

namespace CustomGeometry
{
	size_t CBaseGeometryCollection::Count() const
	{
		return m_Items.Count();
	}

	GeometryItemPtr_t CBaseGeometryCollection::ItemAt(uint32_t index) const
	{
		return index < m_Items.Count() ? m_Items[index] : GeometryItemPtr_t();
	}

	void CBaseGeometryCollection::AddItem(const GeometryItemPtr_t& item)
	{
		if ( !item || item->GetDrawType() == DrawType::None )
		{
			return;
		}

		m_Items.AddToTail(item);
	}

	void CBaseGeometryCollection::VidInit()
	{
	}

	void CBaseGeometryCollection::Clear()
	{
		m_Items.Purge();
	}

	void CBaseGeometryCollection::Render()
	{
		FOR_EACH_VEC(m_Items, index)
		{
			GeometryItemPtr_t item = m_Items[index];

			if ( !item )
			{
				continue;
			}

			CGeometryRenderer renderer;
			renderer.VidInit();
			renderer.Render(*item);
		}
	}

	bool CBaseGeometryCollection::IsEmpty() const
	{
		return m_Items.Count() < 1;
	}
}
