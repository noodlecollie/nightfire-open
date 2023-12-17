#pragma once

#include <cmath>

struct Vec3D
{
	float x = 0;
	float y = 0;
	float z = 0;

	Vec3D() = default;

	Vec3D(float inX, float inY, float inZ) :
		x(inX),
		y(inY),
		z(inZ)
	{
	}

	bool IsNAN() const
	{
		return std::isnan(x) && std::isnan(y) && std::isnan(z);
	}

	void SetNAN()
	{
		x = NAN;
		y = NAN;
		z = NAN;
	}
};

static inline constexpr float DegreesToRadians(float degrees)
{
	return (degrees * static_cast<float>(M_PI)) / 180.0f;
}
