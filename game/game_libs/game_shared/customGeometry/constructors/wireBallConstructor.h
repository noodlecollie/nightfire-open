#pragma once

#include "customGeometry/constructors/baseConstructor.h"

namespace CustomGeometry
{
	class CWireBallConstructor : public CBaseConstructor
	{
	public:
		bool IsValid() const override;
		GeometryItemPtr_t Construct() const override;

		Vector Origin() const;
		void SetOrigin(const Vector& origin);

		float Radius() const;
		void SetRadius(float radius);

		size_t NumDivisions() const;
		void SetNumDivisions(size_t divisions);

	private:
		Vector m_Origin;
		float m_Radius = 1.0f;
		size_t m_NumDivisions = 8;
	};
}  // namespace CustomGeometry
