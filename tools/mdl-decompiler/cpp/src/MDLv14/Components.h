#pragma once

#include <string>
#include <cstdint>

namespace MDLv14
{
	struct Vec3D
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};

	struct BoundingBox
	{
		Vec3D min;
		Vec3D max;
	};

	struct EyePosition
	{
		Vec3D pos;
	};

	struct Header
	{
		std::string identifier;
		int32_t version = 0;
		std::string name;
		int32_t length = 0;
		EyePosition eyePosition;
		BoundingBox boundingBox;
		BoundingBox clippingBox;
		int32_t typeFlags;
	};
}  // namespace MDLv14
