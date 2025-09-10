#include "customGeometry/constructors/wireBallConstructor.h"
#include "customGeometry/constructors/geometryUtils.h"

namespace CustomGeometry
{
	bool CWireBallConstructor::IsValid() const
	{
		return m_Primitive.IsValid();
	}

	GeometryItemPtr_t CWireBallConstructor::Construct() const
	{
		if ( !m_Primitive.IsValid() )
		{
			return nullptr;
		}

		GeometryItemPtr_t geometry(new CGeometryItem());

		geometry->SetDrawType(DrawType::Lines);

		for ( size_t axis = 0; axis < 3; ++axis )
		{
			for ( size_t index = 0; index < m_Primitive.numDivisions; ++index )
			{
				float x1 = 0.0f;
				float y1 = 0.0f;
				float x2 = 0.0f;
				float y2 = 0.0f;

				GetCirclePointCoOrds(index, m_Primitive.numDivisions, m_Primitive.radius, x1, y1);
				GetCirclePointCoOrds(
					(index + 1) % m_Primitive.numDivisions,
					m_Primitive.numDivisions,
					m_Primitive.radius,
					x2,
					y2
				);

				const size_t axisForX = (axis + 1) % 3;
				const size_t axisFory = (axis + 2) % 3;

				Vector begin;
				begin[axisForX] = x1;
				begin[axisFory] = y1;

				Vector end;
				end[axisForX] = x2;
				end[axisFory] = y2;

				geometry->AddLine(m_Primitive.origin + begin, m_Primitive.origin + end);
			}
		}

		return geometry;
	}

	const WireBallPrimitive& CWireBallConstructor::Primitive() const
	{
		return m_Primitive;
	}

	void CWireBallConstructor::SetPrimitive(const WireBallPrimitive& primitive)
	{
		m_Primitive = primitive;
	}
}  // namespace CustomGeometry
