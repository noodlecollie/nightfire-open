#pragma once

#include <math.h>
#include "MathLib/vec3.h"
#include "CompileTools/bspfile.h"

static inline int AxisFromNormal(const vec3_t v)
{
	return (fabsf(v[0]) < NORMAL_EPSILON) + (fabsf(v[1]) < NORMAL_EPSILON) + (fabsf(v[2]) < NORMAL_EPSILON);
}
