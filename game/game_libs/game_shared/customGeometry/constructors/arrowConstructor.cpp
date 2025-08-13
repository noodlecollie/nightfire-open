#include "customGeometry/constructors/arrowConstructor.h"

namespace CustomGeometry
{
	bool CArrowConstructor::IsValid() const
	{
		return m_Length > 0.0f && m_HeadScale > 0.0f && !VectorCompareEpsilon(m_Base, m_Head, 0.001f);
	}

	GeometryItemPtr_t CArrowConstructor::Construct() const
	{
		GeometryItemPtr_t item(new CGeometryItem());
		item->SetDrawType(DrawType::Lines);

		item->AddLine(m_Base, m_Head);

		const Vector dir = (m_Head - m_Base).Normalize();
		Vector perpendicular = CrossProduct(dir, Vector(0.0f, 0.0f, 1.0f));

		item->AddLine(m_Head, m_Head + ((m_HeadScale / 2.0f) * perpendicular) - (m_HeadScale * dir));
		item->AddLine(m_Head, m_Head - ((m_HeadScale / 2.0f) * perpendicular) - (m_HeadScale * dir));

		return item;
	}

	Vector CArrowConstructor::Base() const
	{
		return m_Base;
	}

	void CArrowConstructor::SetBase(const Vector& base)
	{
		m_Base = base;
	}

	Vector CArrowConstructor::Head() const
	{
		return m_Head;
	}

	void CArrowConstructor::SetHead(const Vector& head)
	{
		m_Head = head;
	}

	float CArrowConstructor::Length() const
	{
		return m_Length;
	}

	void CArrowConstructor::SetLength(float length)
	{
		m_Length = length;
	}

	float CArrowConstructor::HeadScale() const
	{
		return m_HeadScale;
	}

	void CArrowConstructor::SetHeadScale(float scale)
	{
		m_HeadScale = scale;
	}
}  // namespace CustomGeometry
