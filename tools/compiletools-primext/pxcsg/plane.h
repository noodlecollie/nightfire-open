#pragma once

#include "BuildPlatform/Decorators.h"
#include "csg.h"

static inline float PlaneDiff2(const vec3_t point, const plane_t* plane)
{
	if ( unlikely(!plane) )
	{
		return 0.0f;
	}

	return DotProduct(point, plane->normal) - plane->dist;
}
