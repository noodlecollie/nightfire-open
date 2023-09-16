/*
mathlib.c - internal mathlib
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "MathLib/mathlib.h"
#include "MathLib/vec3.h"
#include "MathLib/vec4.h"
#include "MathLib/mat3x4.h"
#include "MathLib/quaternion.h"
#include "MathLib/plane.h"
#include "MathLib/utils.h"
#include "PlatformDefs/utils.h"

#define NUM_HULL_ROUNDS SIZE_OF_ARRAY(hull_table)
#define HULL_PRECISION 4

const vec3_t vec3_origin = {0, 0, 0};

static word hull_table[] = {
	2, 4, 6, 8, 12, 16, 18, 24, 28, 32, 36, 40, 48, 54, 56, 60, 64, 72, 80, 112, 120, 128, 140, 176,
};

const int boxpnt[6][4] = {
	{0, 4, 6, 2},  // +X
	{0, 1, 5, 4},  // +Y
	{0, 2, 3, 1},  // +Z
	{7, 5, 1, 3},  // -X
	{7, 3, 2, 6},  // -Y
	{7, 6, 4, 5},  // -Z
};

/*
=================
anglemod
=================
*/
float anglemod(float a)
{
	a = (360.0f / 65536) * ((int)(a * (65536 / 360.0f)) & 65535);
	return a;
}

word FloatToHalf(float v)
{
	unsigned int i = FloatAsUint(v);
	unsigned int e = (i >> 23) & 0x00ff;
	unsigned int m = i & 0x007fffff;
	unsigned short h;

	if ( e <= 127 - 15 )
	{
		h = (unsigned short)(((m | 0x00800000) >> (127 - 14 - e)) >> 13);
	}
	else
	{
		h = (i >> 13) & 0x3fff;
	}

	h |= (i >> 16) & 0xc000;

	return h;
}

float HalfToFloat(word h)
{
	unsigned int f = (h << 16) & 0x80000000;
	unsigned int em = h & 0x7fff;

	if ( em > 0x03ff )
	{
		f |= (em << 13) + ((127 - 15) << 23);
	}
	else
	{
		unsigned int m = em & 0x03ff;

		if ( m != 0 )
		{
			unsigned int e = (em >> 10) & 0x1f;

			while ( (m & 0x0400) == 0 )
			{
				m <<= 1;
				e--;
			}

			m &= 0x3ff;
			f |= ((e + (127 - 14)) << 23) | (m << 13);
		}
	}

	return UintAsFloat(f);
}

/*
=================
RoundUpHullSize

round the hullsize to nearest 'right' value
=================
*/
void RoundUpHullSize(vec3_t size)
{
	size_t i;
	size_t j;

	for ( i = 0; i < 3; i++ )
	{
		qboolean negative = false;
		float result, value;

		value = size[i];
		if ( value < 0.0f )
			negative = true;
		value = Q_ceil(fabs(value));
		result = Q_ceil(size[i]);

		// lookup hull table to find nearest supposed value
		for ( j = 0; j < NUM_HULL_ROUNDS; j++ )
		{
			if ( value > hull_table[j] )
				continue;  // ceil only

			if ( negative )
			{
				result = (value - hull_table[j]);
				if ( result <= HULL_PRECISION )
				{
					result = (float)(-hull_table[j]);
					break;
				}
			}
			else
			{
				result = (value - hull_table[j]);
				if ( result <= HULL_PRECISION )
				{
					result = hull_table[j];
					break;
				}
			}
		}

		size[i] = result;
	}
}

/*
=================
SignbitsForPlane

fast box on planeside test
=================
*/
int SignbitsForPlane(const vec3_t normal)
{
	int bits, i;

	for ( bits = i = 0; i < 3; i++ )
		if ( normal[i] < 0.0f )
			bits |= 1 << i;
	return bits;
}

/*
=================
PlaneTypeForNormal
=================
*/
int PlaneTypeForNormal(const vec3_t normal)
{
	if ( normal[0] == 1.0f )
		return PLANE_X;
	if ( normal[1] == 1.0f )
		return PLANE_Y;
	if ( normal[2] == 1.0f )
		return PLANE_Z;
	return PLANE_NONAXIAL;
}

/*
=================
NearestPOW
=================
*/
int NearestPOW(int value, qboolean roundDown)
{
	int n = 1;

	if ( value <= 0 )
		return 1;
	while ( n < value )
		n <<= 1;

	if ( roundDown )
	{
		if ( n > value )
			n >>= 1;
	}
	return n;
}

// remap a value in the range [A,B] to [C,D].
float RemapVal(float val, float A, float B, float C, float D)
{
	return C + (D - C) * (val - A) / (B - A);
}

float ApproachVal(float target, float value, float speed)
{
	float delta = target - value;

	if ( delta > speed )
		value += speed;
	else if ( delta < -speed )
		value -= speed;
	else
		value = target;

	return value;
}

/*
=================
rsqrt
=================
*/
float rsqrt(float number)
{
	int i;
	float x, y;

	if ( number == 0.0f )
		return 0.0f;

	x = number * 0.5f;
	i = FloatAsInt(number);  // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);  // what the fuck?
	y = IntAsFloat(i);
	y = y * (1.5f - (x * y * y));  // first iteration

	return y;
}

/*
=================
SinCos
=================
*/
void SinCos(float radians, float* sine, float* cosine)
{
#if _MSC_VER == 1200
	_asm
		{
		fld	dword ptr [radians]
		fsincos

		mov edx, dword ptr [cosine]
		mov eax, dword ptr [sine]

		fstp dword ptr [edx]
		fstp dword ptr [eax]
		}
#else
	*sine = sinf(radians);
	*cosine = cosf(radians);
#endif
}

//
// bounds operations
//
/*
=================
ClearBounds
=================
*/
void ClearBounds(vec3_t mins, vec3_t maxs)
{
	// make bogus range
	mins[0] = mins[1] = mins[2] = 999999.0f;
	maxs[0] = maxs[1] = maxs[2] = -999999.0f;
}

/*
=================
AddPointToBounds
=================
*/
void AddPointToBounds(const vec3_t v, vec3_t mins, vec3_t maxs)
{
	float val;
	int i;

	for ( i = 0; i < 3; i++ )
	{
		val = v[i];
		if ( val < mins[i] )
			mins[i] = val;
		if ( val > maxs[i] )
			maxs[i] = val;
	}
}

/*
=================
ExpandBounds
=================
*/
void ExpandBounds(vec3_t mins, vec3_t maxs, float offset)
{
	mins[0] -= offset;
	mins[1] -= offset;
	mins[2] -= offset;
	maxs[0] += offset;
	maxs[1] += offset;
	maxs[2] += offset;
}

/*
=================
BoundsIntersect
=================
*/
qboolean BoundsIntersect(const vec3_t mins1, const vec3_t maxs1, const vec3_t mins2, const vec3_t maxs2)
{
	if ( mins1[0] > maxs2[0] || mins1[1] > maxs2[1] || mins1[2] > maxs2[2] )
		return false;
	if ( maxs1[0] < mins2[0] || maxs1[1] < mins2[1] || maxs1[2] < mins2[2] )
		return false;
	return true;
}

/*
=================
BoundsAndSphereIntersect
=================
*/
qboolean BoundsAndSphereIntersect(const vec3_t mins, const vec3_t maxs, const vec3_t origin, float radius)
{
	if ( mins[0] > origin[0] + radius || mins[1] > origin[1] + radius || mins[2] > origin[2] + radius )
		return false;
	if ( maxs[0] < origin[0] - radius || maxs[1] < origin[1] - radius || maxs[2] < origin[2] - radius )
		return false;
	return true;
}

/*
=================
SphereIntersect
=================
*/
qboolean
SphereIntersect(const vec3_t vSphereCenter, float fSphereRadiusSquared, const vec3_t vLinePt, const vec3_t vLineDir)
{
	float a, b, c, insideSqr;
	vec3_t p;

	// translate sphere to origin.
	VectorSubtract(vLinePt, vSphereCenter, p);

	a = DotProduct(vLineDir, vLineDir);
	b = 2.0f * DotProduct(p, vLineDir);
	c = DotProduct(p, p) - fSphereRadiusSquared;

	insideSqr = b * b - 4.0f * a * c;
	if ( insideSqr <= 0.000001f )
		return false;
	return true;
}

/*
=================
PlaneIntersect

find point where ray
was intersect with plane
=================
*/
void PlaneIntersect(const mplane_t* plane, const vec3_t p0, const vec3_t p1, vec3_t out)
{
	float distToPlane = PlaneDiff(p0, plane);
	float planeDotRay = DotProduct(plane->normal, p1);
	float sect = -(distToPlane) / planeDotRay;

	VectorMA(p0, sect, p1, out);
}

/*
=================
RadiusFromBounds
=================
*/
float RadiusFromBounds(const vec3_t mins, const vec3_t maxs)
{
	vec3_t corner;
	int i;

	for ( i = 0; i < 3; i++ )
	{
		corner[i] = fabsf(mins[i]) > fabsf(maxs[i]) ? fabsf(mins[i]) : fabsf(maxs[i]);
	}

	return (float)VectorLength(corner);
}

float V_CalcFov(float* fov_x, float width, float height)
{
	float x, half_fov_y;

	if ( *fov_x < 1.0f || *fov_x > 179.0f )
	{
		*fov_x = 90.0f;  // default value
	}

	x = width / tanf(DEG2RADF(*fov_x) * 0.5f);
	half_fov_y = atanf(height / x);

	return RAD2DEGF(half_fov_y) * 2;
}

/*
====================
V_AdjustFov
====================
*/
void V_AdjustFov(float* fov_x, float* fov_y, float width, float height, qboolean lock_x)
{
	float x, y;

	if ( width * 3 == 4 * height || width * 4 == height * 5 )
	{
		// 4:3 or 5:4 ratio
		return;
	}

	if ( lock_x )
	{
		*fov_y = 2 * atanf((width * 3) / (height * 4) * tanf(*fov_y * M_PI_F / 360.0f * 0.5f)) * 360 / M_PI_F;
		return;
	}

	y = V_CalcFov(fov_x, 640, 480);
	x = *fov_x;

	*fov_x = V_CalcFov(&y, height, width);
	if ( *fov_x < x )
		*fov_x = x;
	else
		*fov_y = y;
}

/*
==================
BoxOnPlaneSideNonAxial

Returns 1, 2, or 1 + 2
==================
*/
static int BoxOnPlaneSideNonAxial(const vec3_t emins, const vec3_t emaxs, const mplane_t* p)
{
	float dist1 = 0.0f;
	float dist2 = 0.0f;
	int sides = 0;

	// general case
	switch ( p->signbits )
	{
		case 0:
			dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
			dist2 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
			break;
		case 1:
			dist1 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
			dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
			break;
		case 2:
			dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
			dist2 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
			break;
		case 3:
			dist1 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
			dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
			break;
		case 4:
			dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
			dist2 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
			break;
		case 5:
			dist1 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emins[2];
			dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emaxs[2];
			break;
		case 6:
			dist1 = p->normal[0] * emaxs[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
			dist2 = p->normal[0] * emins[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
			break;
		case 7:
			dist1 = p->normal[0] * emins[0] + p->normal[1] * emins[1] + p->normal[2] * emins[2];
			dist2 = p->normal[0] * emaxs[0] + p->normal[1] * emaxs[1] + p->normal[2] * emaxs[2];
			break;
		default:
			break;
	}

	if ( dist1 >= p->dist )
	{
		sides = 1;
	}

	if ( dist2 < p->dist )
	{
		sides |= 2;
	}

	return sides;
}

int BoxOnPlaneSide(const vec3_t emins, const vec3_t emaxs, const mplane_t* p)
{
	if ( !p )
	{
		return 0;
	}

	if ( p->type >= PLANE_NONAXIAL )
	{
		return BoxOnPlaneSideNonAxial(emins, emaxs, p);
	}

	if ( p->dist <= emins[p->type] )
	{
		return 1;
	}
	else if ( p->dist >= emaxs[p->type] )
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

/*
====================
StudioSlerpBones

====================
*/
void R_StudioSlerpBones(int numbones, vec4_t q1[], float pos1[][3], const vec4_t q2[], const float pos2[][3], float s)
{
	int i;

	s = bound(0.0f, s, 1.0f);

	for ( i = 0; i < numbones; i++ )
	{
		QuaternionSlerp(q1[i], q2[i], s, q1[i]);
		VectorLerp(pos1[i], s, pos2[i], pos1[i]);
	}
}
