#include "geometryItem.h"

namespace CustomGeometry
{
	void CGeometryItem::Reset()
	{
		ClearGeometry();

		m_Colour = 0xFFFFFFFF;
		m_DrawType = DrawType::None;
	}

	void CGeometryItem::ClearGeometry()
	{
		m_Points.Purge();
		m_Indices.Purge();
	}

	bool CGeometryItem::IsEmpty() const
	{
		return m_Points.Count() < 1 && m_Indices.Count() < 1;
	}

	DrawType CGeometryItem::GetDrawType() const
	{
		return m_DrawType;
	}

	void CGeometryItem::SetDrawType(DrawType type)
	{
		m_DrawType = type;
	}

	uint32_t CGeometryItem::GetColour() const
	{
		return m_Colour;
	}

	void CGeometryItem::SetColour(uint32_t col)
	{
		m_Colour = col;
	}

	float CGeometryItem::GetScale() const
	{
		return m_Scale;
	}

	void CGeometryItem::SetScale(float scale)
	{
		m_Scale = scale;
	}

	const CUtlVector<Vector>& CGeometryItem::GetPoints() const
	{
		return m_Points;
	}

	const CUtlVector<uint8_t>& CGeometryItem::GetIndices() const
	{
		return m_Indices;
	}

	bool CGeometryItem::AddPoint(const Vector& p0)
	{
		if ( m_Points.Count() + 1 > MAX_POINTS_PER_MSG )
		{
			return false;
		}

		m_Points.AddToTail(p0);
		return true;
	}

	bool CGeometryItem::AddLine(const Vector& p0, const Vector& p1)
	{
		if ( m_Points.Count() + 2 > MAX_POINTS_PER_MSG || m_Indices.Count() + 2 > MAX_INDICES_PER_MSG )
		{
			return false;
		}

		uint8_t baseIndex = CurrentBaseIndex();

		m_Points.AddToTail(p0);
		m_Points.AddToTail(p1);

		m_Indices.AddToTail(baseIndex);
		m_Indices.AddToTail(baseIndex + 1);

		return true;
	}

	bool CGeometryItem::AddPointAndIndex(const Vector& p0)
	{
		if ( m_Points.Count() + 1 > MAX_POINTS_PER_MSG || m_Indices.Count() + 1 > MAX_INDICES_PER_MSG )
		{
			return false;
		}

		uint8_t baseIndex = CurrentBaseIndex();

		m_Points.AddToTail(p0);
		m_Indices.AddToTail(baseIndex);

		return true;
	}

	bool CGeometryItem::AddTriangleQuad(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3)
	{
		if ( m_Points.Count() + 4 > MAX_POINTS_PER_MSG || m_Indices.Count() + 6 > MAX_INDICES_PER_MSG )
		{
			return false;
		}

		uint8_t baseIndex = CurrentBaseIndex();

		m_Points.AddToTail(p0);
		m_Points.AddToTail(p1);
		m_Points.AddToTail(p2);
		m_Points.AddToTail(p3);

		m_Indices.AddToTail(baseIndex + 0);
		m_Indices.AddToTail(baseIndex + 1);
		m_Indices.AddToTail(baseIndex + 2);

		m_Indices.AddToTail(baseIndex + 0);
		m_Indices.AddToTail(baseIndex + 2);
		m_Indices.AddToTail(baseIndex + 3);

		return true;
	}

	bool CGeometryItem::AddIndex(uint8_t index)
	{
		if ( m_Indices.Count() + 1 > MAX_INDICES_PER_MSG )
		{
			return false;
		}

		m_Indices.AddToTail(index);
		return true;
	}

	bool CGeometryItem::AddLineIndices(uint8_t i0, uint8_t i1)
	{
		if ( m_Indices.Count() + 2 > MAX_INDICES_PER_MSG )
		{
			return false;
		}

		m_Indices.AddToTail(i0);
		m_Indices.AddToTail(i1);
		return true;
	}

	size_t CGeometryItem::GetPointCount() const
	{
		return m_Points.Count();
	}

	uint8_t CGeometryItem::CurrentBaseIndex() const
	{
		return static_cast<uint8_t>(m_Points.Count());
	}

	Vector& CGeometryItem::GetPoint(uint8_t index)
	{
		static Vector dummy;
		return index < m_Points.Count() ? m_Points[index] : dummy;
	}

	const Vector& CGeometryItem::GetPoint(uint8_t index) const
	{
		static Vector dummy;
		return index < m_Points.Count() ? m_Points[index] : dummy;
	}

	uint8_t& CGeometryItem::GetIndex(uint8_t i)
	{
		static uint8_t dummy;
		return i < m_Indices.Count() ? m_Indices[i] : dummy;
	}

	const uint8_t& CGeometryItem::GetIndex(uint8_t i) const
	{
		static uint8_t dummy;
		return i < m_Indices.Count() ? m_Indices[i] : dummy;
	}
}
