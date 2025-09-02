#include "customGeometry/constructors/geometryUtils.h"
#include "MathLib/mathlib.h"

namespace CustomGeometry
{
	void GetCirclePointCoOrds(size_t point, size_t segmentCount, float radius, float& x, float& y)
	{
		static constexpr float TWO_PI = 2.0f * static_cast<float>(M_PI);

		const float theta = (TWO_PI * static_cast<float>(point)) / static_cast<float>(segmentCount);
		x = radius * cosf(theta);
		y = radius * sinf(theta);
	}
}  // namespace CustomGeometry
