#include "customGeometry/constructors/circle2DConstructor.h"
#include "customGeometry/constructors/geometryUtils.h"

namespace CustomGeometry
{
	bool CCircle2DConstructor::IsValid() const
	{
		return m_Radius > 0.0f;
	}

	GeometryItemPtr_t CCircle2DConstructor::Construct() const
	{
		if ( !IsValid() )
		{
			return GeometryItemPtr_t();
		}

		GeometryItemPtr_t geometry(new CustomGeometry::CGeometryItem());

		geometry->SetDrawType(DrawType::TriangleStrip);

		float innerRadius = m_Radius - m_Width;

		if ( innerRadius < 0.0f )
		{
			innerRadius = 0.0f;
		}

		for ( size_t index = 0; index < m_NumDivisions; ++index )
		{
			float outerX = 0.0f;
			float outerY = 0.0f;
			float innerX = 0.0f;
			float innerY = 0.0f;

			GetCirclePointCoOrds(index, m_NumDivisions, m_Radius, outerX, outerY);
			GetCirclePointCoOrds(index, m_NumDivisions, innerRadius, innerX, innerY);

			geometry->AddPointAndIndex(Vector(m_Centre.x + outerX, m_Centre.y + outerY, 0));
			geometry->AddPointAndIndex(Vector(m_Centre.x + innerX, m_Centre.y + innerY, 0));
		}

		// Add the first two points again to complete the triangle strip.
		geometry->AddIndex(0);
		geometry->AddIndex(1);

		return geometry;
	}

	Vector2D CCircle2DConstructor::Centre() const
	{
		return m_Centre;
	}

	void CCircle2DConstructor::SetCentre(const Vector2D& centre)
	{
		m_Centre = centre;
	}

	size_t CCircle2DConstructor::NumDivisions() const
	{
		return m_NumDivisions;
	}

	void CCircle2DConstructor::SetNumDivisions(size_t divisions)
	{
		m_NumDivisions = divisions;
	}

	float CCircle2DConstructor::Radius() const
	{
		return m_Radius;
	}

	void CCircle2DConstructor::SetRadius(float radius)
	{
		m_Radius = radius;
	}

	float CCircle2DConstructor::Width() const
	{
		return m_Width;
	}

	void CCircle2DConstructor::SetWidth(float width)
	{
		m_Width = width;
	}
}  // namespace CustomGeometry
