#pragma once

#include "MathLib/vec3.h"
#include "bsp5.h"

static inline float PlaneDiff(const vec3_t point, const plane_t* plane)
{
	return (plane->type < 3 ? point[plane->type] : DotProduct(point, plane->normal)) - plane->dist;
}
