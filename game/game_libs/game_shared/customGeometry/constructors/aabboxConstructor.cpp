#include "customGeometry/constructors/aabboxConstructor.h"

namespace CustomGeometry
{
	bool CAABBoxConstructor::IsValid() const
	{
		return GetDegenerateAxes() != Degenerate_AllAxes;
	}

	Vector CAABBoxConstructor::Min() const
	{
		return m_vecMin;
	}

	Vector CAABBoxConstructor::Max() const
	{
		return m_vecMax;
	}

	void CAABBoxConstructor::SetBounds(const Vector& min, const Vector& max)
	{
		m_vecMin = min;
		m_vecMax = max;
		NormaliseBounds();
	}

	GeometryItemPtr_t CAABBoxConstructor::Construct() const
	{
		GeometryItemPtr_t geom(new CGeometryItem());
		geom->SetDrawType(DrawType::Lines);

		switch ( GetDegenerateAxes() )
		{
			case Degenerate_AllAxes:
			{
				// Nothing to construct.
				break;
			}

			case Degenerate_XAxis | Degenerate_YAxis:
			{
				ConstructDegenerateOnTwoAxes(geom, 2);
				break;
			}

			case Degenerate_XAxis | Degenerate_ZAxis:
			{
				ConstructDegenerateOnTwoAxes(geom, 1);
				break;
			}

			case Degenerate_YAxis | Degenerate_ZAxis:
			{
				ConstructDegenerateOnTwoAxes(geom, 0);
				break;
			}

			case Degenerate_XAxis:
			{
				ConstructDegenerateOnOneAxis(geom, 0);
				break;
			}

			case Degenerate_YAxis:
			{
				ConstructDegenerateOnOneAxis(geom, 1);
				break;
			}

			case Degenerate_ZAxis:
			{
				ConstructDegenerateOnOneAxis(geom, 2);
				break;
			}

			default:
			{
				ConstructWithAllAxesValid(geom);
				break;
			}
		}

		return geom;
	}

	void CAABBoxConstructor::ConstructWithAllAxesValid(GeometryItemPtr_t& geom) const
	{
		const Vector& min = m_vecMin;
		const Vector& max = m_vecMax;

		// Bottom rect points
		geom->AddPoint(Vector(min[0], min[1], min[2]));
		geom->AddPoint(Vector(max[0], min[1], min[2]));
		geom->AddPoint(Vector(max[0], max[1], min[2]));
		geom->AddPoint(Vector(min[0], max[1], min[2]));

		// Top rect points
		geom->AddPoint(Vector(min[0], min[1], max[2]));
		geom->AddPoint(Vector(max[0], min[1], max[2]));
		geom->AddPoint(Vector(max[0], max[1], max[2]));
		geom->AddPoint(Vector(min[0], max[1], max[2]));

		// Bottom rect
		geom->AddLineIndices(0, 1);
		geom->AddLineIndices(1, 2);
		geom->AddLineIndices(2, 3);
		geom->AddLineIndices(3, 0);

		// Top rect
		geom->AddLineIndices(4, 5);
		geom->AddLineIndices(5, 6);
		geom->AddLineIndices(6, 7);
		geom->AddLineIndices(7, 4);

		// Connecting lines
		geom->AddLineIndices(0, 4);
		geom->AddLineIndices(1, 5);
		geom->AddLineIndices(2, 6);
		geom->AddLineIndices(3, 7);
	}

	void CAABBoxConstructor::ConstructDegenerateOnOneAxis(GeometryItemPtr_t& geom, uint32_t degenerateAxis) const
	{
		static constexpr size_t NUM_POINTS = 4;

		const Vector& min = m_vecMin;
		const Vector& max = m_vecMax;
		Vector points[NUM_POINTS];

		if ( degenerateAxis == 0 )
		{
			points[0] = Vector(min[0], min[1], min[2]);
			points[1] = Vector(min[0], max[1], min[2]);
			points[2] = Vector(min[0], max[1], max[2]);
			points[3] = Vector(min[0], min[1], max[2]);
		}
		else if ( degenerateAxis == 1 )
		{
			points[0] = Vector(min[0], min[1], min[2]);
			points[1] = Vector(max[0], min[1], min[2]);
			points[2] = Vector(max[0], min[1], max[2]);
			points[3] = Vector(min[0], min[1], max[2]);
		}
		else
		{
			points[0] = Vector(min[0], min[1], min[2]);
			points[1] = Vector(max[0], min[1], min[2]);
			points[2] = Vector(max[0], max[1], min[2]);
			points[3] = Vector(min[0], max[1], min[2]);
		}

		size_t baseIndex = geom->GetPointCount();

		for ( size_t index = 0; index < NUM_POINTS; ++index )
		{
			geom->AddPoint(points[index]);
		}

		for ( size_t index = 0; index < NUM_POINTS; ++index )
		{
			geom->AddLineIndices(baseIndex + index, baseIndex + ((index + 1) % NUM_POINTS));
		}
	}

	void CAABBoxConstructor::ConstructDegenerateOnTwoAxes(GeometryItemPtr_t& geom, uint32_t validAxis) const
	{
		Vector maxPoint = m_vecMin;
		maxPoint[validAxis] = m_vecMax[validAxis];

		size_t baseIndex = geom->GetPointCount();

		geom->AddPoint(m_vecMin);
		geom->AddPoint(maxPoint);
		geom->AddLineIndices(baseIndex, baseIndex + 1);
	}

	uint32_t CAABBoxConstructor::GetDegenerateAxes() const
	{
		static constexpr float EQUALITY_TOLERANCE = 0.01f;

		uint32_t degenerateAxes = 0;

		for ( uint32_t axis = 0; axis < 3; ++axis )
		{
			if ( m_vecMax[axis] - m_vecMin[axis] < EQUALITY_TOLERANCE )
			{
				degenerateAxes |= (1 << axis);
			}
		}

		return degenerateAxes;
	}

	void CAABBoxConstructor::NormaliseBounds()
	{
		for ( uint32_t axis = 0; axis < 3; ++axis )
		{
			if ( m_vecMin[axis] > m_vecMax[axis] )
			{
				float temp = m_vecMin[axis];
				m_vecMin[axis] = m_vecMax[axis];
				m_vecMax[axis] = temp;
			}
		}
	}
}  // namespace CustomGeometry
