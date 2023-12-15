#pragma once

#include <ostream>
#include <iomanip>
#include "Common/Math.h"

static inline void WriteFloat(std::ostream& stream, float value)
{
	const std::streamsize defaultPrecision = stream.precision();
	stream << std::setprecision(6) << (std::isnan(value) ? 0.0f : value) << std::setprecision(defaultPrecision);
}

static inline void WriteVec3D(std::ostream& stream, const Vec3D value)
{
	WriteFloat(stream, value.x);
	stream << " ";
	WriteFloat(stream, value.y);
	stream << " ";
	WriteFloat(stream, value.z);
}
