#pragma once

#include <assert.h>
#include "PlatformDefs/typedefs.h"
#include "MathLib/mathdefs.h"
#include "MathLib/mathlib.h"

typedef vec_t vec3_t[3];

// NFTODO: Move this to its own header, along with functions.
typedef struct mplane_s
{
	vec3_t normal;
	vec_t dist;
	byte type;  // for fast side tests
	byte signbits;  // signx + (signy<<1) + (signz<<1)
	byte pad[2];
} mplane_t;

extern const vec3_t vec3_origin;

static inline qboolean VectorIsNAN(const vec3_t v)
{
	return IS_NAN(v[0]) || IS_NAN(v[1]) || IS_NAN(v[2]);
}

static inline float DotProduct(const vec3_t x, const vec3_t y)
{
	return (x[0] * y[0]) + (x[1] * y[1]) + (x[2] * y[2]);
}

static inline int DotProductAbs(const vec3_t x, const vec3_t y)
{
	return abs((int)(x[0] * y[0])) + abs((int)(x[1] * y[1])) + abs((int)(x[2] * y[2]));
}

static inline float DotProductFabs(const vec3_t x, const vec3_t y)
{
	return fabsf(x[0] * y[0]) + fabsf(x[1] * y[1]) + fabsf(x[2] * y[2]);
};

static inline double DotProductPrecise(const vec3_t x, const vec3_t y)
{
	return ((double)(x[0]) * (double)(y[0])) + ((double)(x[1]) * (double)(y[1])) + ((double)(x[2]) * (double)(y[2]));
}

static inline void CrossProduct(const vec3_t a, const vec3_t b, vec3_t c)
{
	c[0] = (a[1] * b[2]) - (a[2] * b[1]);
	c[1] = (a[2] * b[0]) - (a[0] * b[2]);
	c[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

static inline void VectorAdd(const vec3_t a, const vec3_t b, vec3_t c)
{
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	c[2] = a[2] + b[2];
}

static inline void VectorAddScalar(const vec3_t a, vec_t b, vec3_t c)
{
	c[0] = a[0] + b;
	c[1] = a[1] + b;
	c[2] = a[2] + b;
}

static inline void VectorSubtract(const vec3_t a, const vec3_t b, vec3_t c)
{
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
}

static inline void VectorCopy(const vec3_t a, vec3_t b)
{
	b[0] = a[0];
	b[1] = a[1];
	b[2] = a[2];
}

static inline void VectorScale(const vec3_t in, float scale, vec3_t out)
{
	out[0] = in[0] * scale;
	out[1] = in[1] * scale;
	out[2] = in[2] * scale;
}

static inline qboolean VectorCompare(const vec3_t v1, const vec3_t v2)
{
	return v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2];
}

static inline void VectorDivide(const vec3_t in, float d, vec3_t out)
{
	assert(d != 0.0f);
	VectorScale(in, (1.0f / d), out);
}

static inline vec_t VectorMax(const vec3_t a)
{
	return Q_max(a[0], Q_max(a[1], a[2]));
}

static inline float VectorAvg(const vec3_t a)
{
	return (a[0] + a[1] + a[2]) / 3.0f;
}

static inline float VectorLengthSquared(const vec3_t a)
{
	return DotProduct(a, a);
}

static inline float VectorLength(const vec3_t a)
{
	return sqrtf(VectorLengthSquared(a));
}

static inline float VectorDistanceSquared(const vec3_t a, const vec3_t b)
{
	return ((a[0] - b[0]) * (a[0] - b[0])) + ((a[1] - b[1]) * (a[1] - b[1])) + ((a[2] - b[2]) * (a[2] - b[2]));
}

static inline float VectorDistance(const vec3_t a, const vec3_t b)
{
	return sqrtf(VectorDistanceSquared(a, b));
}

static inline void VectorAverage(const vec3_t a, const vec3_t b, vec3_t o)
{
	o[0] = (a[0] + b[0]) * 0.5f;
	o[1] = (a[1] + b[1]) * 0.5f;
	o[2] = (a[2] + b[2]) * 0.5f;
}

static inline void VectorSet(vec3_t v, vec_t x, vec_t y, vec_t z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

static inline void VectorClear(vec3_t x)
{
	VectorSet(x, 0.0f, 0.0f, 0.0f);
}

static inline void VectorLerp(const vec3_t v1, float lerp, const vec3_t v2, vec3_t c)
{
	c[0] = v1[0] + (lerp * (v2[0] - v1[0]));
	c[1] = v1[1] + (lerp * (v2[1] - v1[1]));
	c[2] = v1[2] + (lerp * (v2[2] - v1[2]));
}

static inline float VectorNormalize(vec3_t v)
{
	const float length = VectorLength(v);
	const float iLength = length != 0.0f ? (1.0f / length) : 0.0f;

	v[0] *= iLength;
	v[1] *= iLength;
	v[2] *= iLength;

	return length;
}

static inline void VectorNormalize2(const vec3_t v, vec3_t dest)
{
	float ilength = VectorLength(v);

	if ( ilength )
	{
		ilength = 1.0f / ilength;
	}

	dest[0] = v[0] * ilength;
	dest[1] = v[1] * ilength;
	dest[2] = v[2] * ilength;
}

static inline void VectorNormalizeFast(vec3_t v)
{
	const float ilength = rsqrt(VectorLengthSquared(v));

	v[0] *= ilength;
	v[1] *= ilength;
	v[2] *= ilength;
}

static inline void VectorNegate(const vec3_t x, vec3_t y)
{
	y[0] = -(x[0]);
	y[1] = -(x[1]);
	y[2] = -(x[2]);
}

static inline void VectorM(float scale1, const vec3_t b1, vec3_t c)
{
	c[0] = (scale1 * b1[0]);
	c[1] = (scale1 * b1[1]);
	c[2] = (scale1 * b1[2]);
}

static inline void VectorMA(const vec3_t a, float scale, const vec3_t b, vec3_t c)
{
	c[0] = a[0] + (scale * b[0]);
	c[1] = a[1] + (scale * b[1]);
	c[2] = a[2] + (scale * b[2]);
}

static inline void VectorMAM(float scale1, const vec3_t b1, float scale2, const vec3_t b2, vec3_t c)
{
	c[0] = (scale1 * b1[0]) + (scale2 * b2[0]);
	c[1] = (scale1 * b1[1]) + (scale2 * b2[1]);
	c[2] = (scale1 * b1[2]) + (scale2 * b2[2]);
}

static inline void
VectorMAMAM(float scale1, const vec3_t b1, float scale2, const vec3_t b2, float scale3, const vec3_t b3, vec3_t c)
{
	c[0] = (scale1 * b1[0]) + (scale2 * b2[0]) + (scale3 * b3[0]);
	c[1] = (scale1 * b1[1]) + (scale2 * b2[1]) + (scale3 * b3[1]);
	c[2] = (scale1 * b1[2]) + (scale2 * b2[2]) + (scale3 * b3[2]);
}

static inline qboolean VectorIsNull(const vec3_t v)
{
	return v[0] == 0.0f && v[1] == 0.0f && v[2] == 0.0f;
}

float VectorNormalizeLength(const vec3_t v, vec3_t out);
qboolean VectorCompareEpsilon(const vec3_t vec1, const vec3_t vec2, vec_t epsilon);
void VectorVectors(const vec3_t forward, vec3_t right, vec3_t up);
void VectorAngles(const float* forward, float* angles);
void AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
void VectorsAngles(const vec3_t forward, const vec3_t right, const vec3_t up, vec3_t angles);
void RoundUpHullSize(vec3_t size);
int SignbitsForPlane(const vec3_t normal);
int PlaneTypeForNormal(const vec3_t normal);
void PlaneIntersect(const mplane_t* plane, const vec3_t p0, const vec3_t p1, vec3_t out);
void ClearBounds(vec3_t mins, vec3_t maxs);
void AddPointToBounds(const vec3_t v, vec3_t mins, vec3_t maxs);
qboolean BoundsIntersect(const vec3_t mins1, const vec3_t maxs1, const vec3_t mins2, const vec3_t maxs2);
qboolean BoundsAndSphereIntersect(const vec3_t mins, const vec3_t maxs, const vec3_t origin, float radius);
qboolean
SphereIntersect(const vec3_t vSphereCenter, float fSphereRadiusSquared, const vec3_t vLinePt, const vec3_t vLineDir);
float RadiusFromBounds(const vec3_t mins, const vec3_t maxs);
void ExpandBounds(vec3_t mins, vec3_t maxs, float offset);

int BoxOnPlaneSide(const vec3_t emins, const vec3_t emaxs, const mplane_t* p);
#define BOX_ON_PLANE_SIDE(emins, emaxs, p) \
	(((p)->type < 3) ? (((p)->dist <= (emins)[(p)->type]) ? 1 : (((p)->dist >= (emaxs)[(p)->type]) ? 2 : 3)) \
					 : BoxOnPlaneSide((emins), (emaxs), (p)))

extern const int boxpnt[6][4];
