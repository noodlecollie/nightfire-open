#pragma once

#include <cmath>
#include "miniutl.h"

namespace ExtraMath
{
	static inline float Clamp(float min, float val, float max)
	{
		return Max(min, Min(val, max));
	}

	// FUNC should be a callback that accepts a float in range [0 1] and
	// returns a corresponding float in range [0 1]. If clampInput is false,
	// the input float may be outside the [0 1] range.
	template<typename FUNC>
	static inline float
	RemapParametric(float value, float a, float b, float x, float y, const FUNC& parametricFunc, bool clampInput = true)
	{
		if ( fabs(a - b) < 0.001f )
		{
			// No domain to remap from.
			return x;
		}

		const float interpolant = (value - a) / (b - a);
		return x + (parametricFunc(clampInput ? Clamp(0.0f, interpolant, 1.0f) : interpolant) * (y - x));
	}

	// Linearly remaps value from [a b] range to [x y] range.
	static inline float RemapLinear(float value, float a, float b, float x, float y, bool clampInput = true)
	{
		if ( fabs(a - b) < 0.001f )
		{
			// No domain to remap from.
			return x;
		}

		const float interpolant = (value - a) / (b - a);

		// Optimisation: don't use a custom parametric function, this is probably quicker.
		return x + ((clampInput ? Clamp(0.0f, interpolant, 1.0f) : interpolant) * (y - x));
	}

	static inline float RemapSqrt(float value, float a, float b, float x, float y, bool clampInput = true)
	{
		return RemapParametric(
			value,
			a,
			b,
			x,
			y,
			[](float input)
			{
				// y = sqrt(x) goes from y = 0 at x = 0 to y = 2 at x = 4.
				// We want to multiply the input by 4, do a sqrt, and then
				// divide the output by 2.

				if ( input >= 0.0f )
				{
					return sqrtf(4.0f * input) / 2.0f;
				}
				else
				{
					// Just rotate the graph 180 degrees.
					return sqrtf(-4.0f * input) / -2.0f;
				}
			},
			clampInput);
	}
}  // namespace ExtraMath
