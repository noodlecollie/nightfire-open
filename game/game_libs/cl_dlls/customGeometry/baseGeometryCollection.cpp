#include <cmath>
#include "baseGeometryCollection.h"
#include "customGeometry/geometryRenderer.h"
#include "cl_dll.h"

namespace CustomGeometry
{
	size_t CBaseGeometryCollection::Count() const
	{
		return m_Items.Count();
	}

	GeometryItemPtr_t CBaseGeometryCollection::ItemAt(uint32_t index) const
	{
		return index < static_cast<uint32_t>(m_Items.Count()) ? m_Items[index] : GeometryItemPtr_t();
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

	void CBaseGeometryCollection::Render(RenderType renderType)
	{
		const float curTime = gEngfuncs.GetClientTime();

		FOR_EACH_VEC(m_Items, index)
		{
			GeometryItemPtr_t& item = m_Items[index];

			if ( !item )
			{
				continue;
			}

			const bool shouldRender = (renderType == RenderType::Geometry && item->GetDrawType() != DrawType::Text) ||
				(renderType == RenderType::Text && item->GetDrawType() == DrawType::Text);

			if ( !shouldRender )
			{
				continue;
			}

			CGeometryRenderer renderer;
			renderer.VidInit();
			renderer.Render(*item);

			if ( item->HasLifetime() )
			{
				float dieTime = item->GetLifetimeSecs();

				if ( dieTime <= curTime )
				{
					// Make pointer null. It will be removed on next pass.
					item.reset();
				}
			}
		}

		int index = 0;

		while ( index < m_Items.Count() )
		{
			if ( m_Items[index] )
			{
				++index;
				continue;
			}

			m_Items.Remove(index);
		}
	}

	bool CBaseGeometryCollection::IsEmpty() const
	{
		return m_Items.Count() < 1;
	}
}  // namespace CustomGeometry
