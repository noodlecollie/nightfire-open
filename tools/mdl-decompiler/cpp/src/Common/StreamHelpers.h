#pragma once

#include <ostream>
#include <iomanip>
#include "Common/Math.h"

static inline void WriteFixedFloat(std::ostream& stream, float value)
{
	const std::streamsize defaultPrecision = stream.precision();
	stream << std::setprecision(6) << std::fixed << (std::isnan(value) ? 0.0f : value)
		   << std::setprecision(defaultPrecision) << std::defaultfloat;
}

static inline void WriteVec3D(std::ostream& stream, const Vec3D value)
{
	WriteFixedFloat(stream, value.x);
	stream << " ";
	WriteFixedFloat(stream, value.y);
	stream << " ";
	WriteFixedFloat(stream, value.z);
}
