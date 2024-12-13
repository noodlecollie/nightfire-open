#pragma once

#include "customGeometry/constructors/baseConstructor.h"

namespace CustomGeometry
{
	class CCrosshair3DConstructor : public CBaseConstructor
	{
	public:
		bool IsValid() const override;
		GeometryItemPtr_t Construct() const override;

		Vector Origin() const;
		void SetOrigin(const Vector& origin);

		// Scale is the length of each prong from the origin.
		float Scale() const;
		void SetScale(float scale);

	private:
		Vector m_Origin;
		float m_Scale = 8.0f;
	};
}  // namespace CustomGeometry
