#pragma once

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
