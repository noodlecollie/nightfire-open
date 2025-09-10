#pragma once

#include "customGeometry/constructors/baseConstructor.h"
#include "customGeometry/primitiveDefs.h"

namespace CustomGeometry
{
	class CWireBallConstructor : public CBaseConstructor
	{
	public:
		bool IsValid() const override;
		GeometryItemPtr_t Construct() const override;

		const WireBallPrimitive& Primitive() const;
		void SetPrimitive(const WireBallPrimitive& primitive);

	private:
		WireBallPrimitive m_Primitive {};
	};
}  // namespace CustomGeometry
