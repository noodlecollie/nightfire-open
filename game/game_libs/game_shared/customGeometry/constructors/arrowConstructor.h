#pragma once

#include "customGeometry/constructors/baseConstructor.h"

namespace CustomGeometry
{
	class CArrowConstructor : public CBaseConstructor
	{
	public:
		bool IsValid() const override;
		GeometryItemPtr_t Construct() const override;

		Vector Base() const;
		void SetBase(const Vector& base);

		Vector Head() const;
		void SetHead(const Vector& head);

		float Length() const;
		void SetLength(float length);

		float HeadScale() const;
		void SetHeadScale(float scale);

	private:
		Vector m_Base;
		Vector m_Head;
		float m_Length = 64.0f;
		float m_HeadScale = 8.0f;
	};
}  // namespace CustomGeometry
