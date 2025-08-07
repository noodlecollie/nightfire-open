#pragma once

#include "customGeometry/constructors/baseConstructor.h"
#include "MathLib/vec3.h"

namespace CustomGeometry
{
	class CCircle2DConstructor : public CBaseConstructor
	{
	public:
		bool IsValid() const override;
		GeometryItemPtr_t Construct() const override;

		Vector2D Centre() const;
		void SetCentre(const Vector2D& centre);

		size_t NumDivisions() const;
		void SetNumDivisions(size_t divisions);

		float Radius() const;
		void SetRadius(float radius);

		// Measured inwards from the radius.
		float Width() const;
		void SetWidth(float width);

	private:
		Vector2D m_Centre;
		size_t m_NumDivisions = 8;
		float m_Radius = 1.0f;
		float m_Width = 1.0f;
	};
}  // namespace CustomGeometry
