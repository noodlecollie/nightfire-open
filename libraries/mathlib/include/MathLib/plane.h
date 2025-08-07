#pragma once

#include "BuildPlatform/Typedefs.h"
#include "BuildPlatform/Decorators.h"
#include "MathLib/vec3.h"

#ifdef __cplusplus
extern "C"
{
#endif

// 0 - 2 are axial planes
// 3 needs alternate calc
typedef enum
{
	PLANE_X = 0,
	PLANE_Y,
	PLANE_Z,
	PLANE_NONAXIAL
} PlaneAxis_e;

typedef struct mplane_s
{
	vec3_t normal;
	vec_t dist;
	byte type;  // for fast side tests
	byte signbits;  // signx + (signy<<1) + (signz<<1)
	byte pad[2];
} mplane_t;

static inline float PlaneDist(const vec3_t point, const mplane_t* plane)
{
	if ( unlikely(!plane) )
	{
		return 0.0f;
	}

	return plane->type < 3 ? point[plane->type] : DotProduct(point, plane->normal);
}

static inline float PlaneDiff(const vec3_t point, const mplane_t* plane)
{
	if ( unlikely(!plane) )
	{
		return 0.0f;
	}

	return PlaneDist(point, plane) - plane->dist;
}

int SignbitsForPlane(const vec3_t normal);
int PlaneTypeForNormal(const vec3_t normal);
void PlaneIntersect(const mplane_t* plane, const vec3_t p0, const vec3_t p1, vec3_t out);
int BoxOnPlaneSide(const vec3_t emins, const vec3_t emaxs, const mplane_t* p);

#ifdef __cplusplus
}  // extern "C"
#endif
