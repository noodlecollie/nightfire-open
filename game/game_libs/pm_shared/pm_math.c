/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/
// pm_math.c -- math primitives

#include "EnginePublicAPI/const.h"
#include "MathLib/vec3.h"
#include "MathLib/angles.h"

/*
===================
InterpolateAngles

Interpolate Euler angles.
FIXME:  Use Quaternions to avoid discontinuities
Frac is 0.0 to 1.0 ( i.e., should probably be clamped, but doesn't have to be )
===================
*/


/*
===================
AngleBetweenVectors

===================
*/
float AngleBetweenVectors(const vec3_t v1, const vec3_t v2)
{
	float angle;
	float l1 = Length(v1);
	float l2 = Length(v2);

	if ( !l1 || !l2 )
		return 0.0f;

	angle = acosf(DotProduct(v1, v2) / (l1 * l2));
	angle = (angle * 180.0f) / (float)M_PI;

	return angle;
}

void VectorTransform(const vec3_t in1, float in2[3][4], vec3_t out)
{
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}

void VectorMatrix(vec3_t forward, vec3_t right, vec3_t up)
{
	vec3_t tmp;

	if ( forward[0] == 0 && forward[1] == 0 )
	{
		right[0] = 1;
		right[1] = 0;
		right[2] = 0;
		up[0] = -forward[2];
		up[1] = 0;
		up[2] = 0;
		return;
	}

	tmp[0] = 0;
	tmp[1] = 0;
	tmp[2] = 1.0;
	CrossProduct(forward, tmp, right);
	VectorNormalize(right);
	CrossProduct(right, forward, up);
	VectorNormalize(up);
}
