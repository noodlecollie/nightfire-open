#pragma once

#include "PlatformDefs/typedefs.h"

typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t quat_t[4];
typedef vec_t matrix3x4[3][4];
typedef vec_t matrix4x4[4][4];

typedef struct mplane_s
{
	vec3_t normal;
	vec_t dist;
	byte type;  // for fast side tests
	byte signbits;  // signx + (signy<<1) + (signz<<1)
	byte pad[2];
} mplane_t;

typedef enum
{
	PITCH = 0,
	YAW,
	ROLL
} EulerAngleAxis_e;

// 0 - 2 are axial planes
// 3 needs alternate calc
typedef enum
{
	PLANE_X = 0,
	PLANE_Y,
	PLANE_Z,
	PLANE_NONAXIAL
} PlaneAxis_e;
