#include "customGeometry/constructors/crosshair3DConstructor.h"

namespace CustomGeometry
{
	bool CCrosshair3DConstructor::IsValid() const
	{
		return m_Scale > 0.0f;
	}

	GeometryItemPtr_t CCrosshair3DConstructor::Construct() const
	{
		GeometryItemPtr_t geom(new CGeometryItem());
		geom->SetDrawType(DrawType::Lines);

		geom->AddPoint(m_Origin + Vector(-m_Scale, 0.0f, 0.0f));
		geom->AddPoint(m_Origin + Vector(m_Scale, 0.0f, 0.0f));
		geom->AddPoint(m_Origin + Vector(0.0f, -m_Scale, 0.0f));
		geom->AddPoint(m_Origin + Vector(0.0f, m_Scale, 0.0f));
		geom->AddPoint(m_Origin + Vector(0.0f, 0.0f, -m_Scale));
		geom->AddPoint(m_Origin + Vector(0.0f, 0.0f, m_Scale));

		geom->AddLineIndices(0, 1);
		geom->AddLineIndices(2, 3);
		geom->AddLineIndices(4, 5);

		return geom;
	}

	Vector CCrosshair3DConstructor::Origin() const
	{
		return m_Origin;
	}

	void CCrosshair3DConstructor::SetOrigin(const Vector& origin)
	{
		m_Origin = origin;
	}

	float CCrosshair3DConstructor::Scale() const
	{
		return m_Scale;
	}

	void CCrosshair3DConstructor::SetScale(float scale)
	{
		m_Scale = scale;
	}
}  // namespace CustomGeometry
