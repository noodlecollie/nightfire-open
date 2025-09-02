#pragma once

#include <limits>
#include "MathLib/vec3.h"

namespace CustomGeometry
{
	// Must fit in a uint8_t.
	// "None" is used for sending a clear message.
	enum class PrimitiveType
	{
		None = 0,
		WireBall,
	};

	struct BasePrimitive
	{
		virtual bool IsValid() const = 0;
	};

	struct WireBallPrimitive : public BasePrimitive
	{
		static constexpr PrimitiveType TYPE = PrimitiveType::WireBall;

		Vector origin;
		uint16_t radius = 8;
		uint8_t numDivisions = 8;

		bool IsValid() const override
		{
			return radius > 0 && numDivisions >= 3;
		}
	};
}  // namespace CustomGeometry
