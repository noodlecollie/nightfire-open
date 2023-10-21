#pragma once

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
};
