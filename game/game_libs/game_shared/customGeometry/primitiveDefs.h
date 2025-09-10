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
		AABBox,
		WaypointMarker,
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

	struct AABBoxPrimitive : public BasePrimitive
	{
		static constexpr PrimitiveType TYPE = PrimitiveType::AABBox;

		Vector mins;
		Vector maxs;

		bool IsValid() const override
		{
			return !VectorCompare(mins, maxs);
		}
	};

	struct WaypointMarkerPrimitive : public BasePrimitive
	{
		static constexpr PrimitiveType TYPE = PrimitiveType::WaypointMarker;

		// At player eye position, not ground
		Vector location;

		bool IsValid() const override
		{
			return true;
		}
	};
}  // namespace CustomGeometry
