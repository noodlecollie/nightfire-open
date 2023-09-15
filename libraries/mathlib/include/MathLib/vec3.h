#pragma once

#include "MathLib/mathdefs.h"
#include "MathLib/mathtypes.h"

extern const vec3_t vec3_origin;

#define VectorIsNAN(v) (IS_NAN(v[0]) || IS_NAN(v[1]) || IS_NAN(v[2]))
#define DotProduct(x, y) ((x)[0] * (y)[0] + (x)[1] * (y)[1] + (x)[2] * (y)[2])
#define DotProductAbs(x, y) (abs((int)((x)[0] * (y)[0])) + abs((int)((x)[1] * (y)[1])) + abs((int)((x)[2] * (y)[2])))
#define DotProductFabs(x, y) (fabs((x)[0] * (y)[0]) + fabs((x)[1] * (y)[1]) + fabs((x)[2] * (y)[2]))
#define DotProductPrecise(x, y) \
	((double)(x)[0] * (double)(y)[0] + (double)(x)[1] * (double)(y)[1] + (double)(x)[2] * (double)(y)[2])
#define CrossProduct(a, b, c) \
	((c)[0] = (a)[1] * (b)[2] - (a)[2] * (b)[1], \
	 (c)[1] = (a)[2] * (b)[0] - (a)[0] * (b)[2], \
	 (c)[2] = (a)[0] * (b)[1] - (a)[1] * (b)[0])
#define Vector2Subtract(a, b, c) ((c)[0] = (a)[0] - (b)[0], (c)[1] = (a)[1] - (b)[1])
#define VectorSubtract(a, b, c) ((c)[0] = (a)[0] - (b)[0], (c)[1] = (a)[1] - (b)[1], (c)[2] = (a)[2] - (b)[2])
#define Vector2Add(a, b, c) ((c)[0] = (a)[0] + (b)[0], (c)[1] = (a)[1] + (b)[1])
#define VectorAdd(a, b, c) ((c)[0] = (a)[0] + (b)[0], (c)[1] = (a)[1] + (b)[1], (c)[2] = (a)[2] + (b)[2])
#define VectorAddScalar(a, b, c) ((c)[0] = (a)[0] + (b), (c)[1] = (a)[1] + (b), (c)[2] = (a)[2] + (b))
#define Vector2Copy(a, b) ((b)[0] = (a)[0], (b)[1] = (a)[1])
#define VectorCopy(a, b) ((b)[0] = (a)[0], (b)[1] = (a)[1], (b)[2] = (a)[2])
#define Vector4Copy(a, b) ((b)[0] = (a)[0], (b)[1] = (a)[1], (b)[2] = (a)[2], (b)[3] = (a)[3])
#define VectorScale(in, scale, out) \
	((out)[0] = (in)[0] * (scale), (out)[1] = (in)[1] * (scale), (out)[2] = (in)[2] * (scale))
#define VectorCompare(v1, v2) ((v1)[0] == (v2)[0] && (v1)[1] == (v2)[1] && (v1)[2] == (v2)[2])
#define VectorDivide(in, d, out) VectorScale(in, (1.0f / (d)), out)
#define VectorMax(a) (Q_max((a)[0], Q_max((a)[1], (a)[2])))
#define VectorAvg(a) (((a)[0] + (a)[1] + (a)[2]) / 3)
#define VectorLength(a) (sqrt(DotProduct(a, a)))
#define VectorLength2(a) (DotProduct(a, a))
#define VectorDistance(a, b) (sqrt(VectorDistance2(a, b)))
#define VectorDistance2(a, b) \
	(((a)[0] - (b)[0]) * ((a)[0] - (b)[0]) + ((a)[1] - (b)[1]) * ((a)[1] - (b)[1]) + \
	 ((a)[2] - (b)[2]) * ((a)[2] - (b)[2]))
#define Vector2Average(a, b, o) ((o)[0] = ((a)[0] + (b)[0]) * 0.5f, (o)[1] = ((a)[1] + (b)[1]) * 0.5f)
#define VectorAverage(a, b, o) \
	((o)[0] = ((a)[0] + (b)[0]) * 0.5f, (o)[1] = ((a)[1] + (b)[1]) * 0.5f, (o)[2] = ((a)[2] + (b)[2]) * 0.5f)
#define Vector2Set(v, x, y) ((v)[0] = (x), (v)[1] = (y))
#define VectorSet(v, x, y, z) ((v)[0] = (x), (v)[1] = (y), (v)[2] = (z))
#define Vector4Set(v, a, b, c, d) ((v)[0] = (a), (v)[1] = (b), (v)[2] = (c), (v)[3] = (d))
#define VectorClear(x) ((x)[0] = (x)[1] = (x)[2] = 0)
#define Vector2Lerp(v1, lerp, v2, c) \
	((c)[0] = (v1)[0] + (lerp) * ((v2)[0] - (v1)[0]), (c)[1] = (v1)[1] + (lerp) * ((v2)[1] - (v1)[1]))
#define VectorLerp(v1, lerp, v2, c) \
	((c)[0] = (v1)[0] + (lerp) * ((v2)[0] - (v1)[0]), \
	 (c)[1] = (v1)[1] + (lerp) * ((v2)[1] - (v1)[1]), \
	 (c)[2] = (v1)[2] + (lerp) * ((v2)[2] - (v1)[2]))
#define VectorNormalize(v) \
	{ \
		float ilength = sqrtf(DotProduct(v, v)); \
		if ( ilength ) \
			ilength = 1.0f / ilength; \
		v[0] *= ilength; \
		v[1] *= ilength; \
		v[2] *= ilength; \
	}
#define VectorNormalize2(v, dest) \
	{ \
		float ilength = sqrtf(DotProduct(v, v)); \
		if ( ilength ) \
			ilength = 1.0f / ilength; \
		dest[0] = v[0] * ilength; \
		dest[1] = v[1] * ilength; \
		dest[2] = v[2] * ilength; \
	}
#define VectorNormalizeFast(v) \
	{ \
		float ilength = rsqrt(DotProduct(v, v)); \
		v[0] *= ilength; \
		v[1] *= ilength; \
		v[2] *= ilength; \
	}
#define VectorNormalizeLength(v) VectorNormalizeLength2((v), (v))
#define VectorNegate(x, y) ((y)[0] = -(x)[0], (y)[1] = -(x)[1], (y)[2] = -(x)[2])
#define VectorM(scale1, b1, c) ((c)[0] = (scale1) * (b1)[0], (c)[1] = (scale1) * (b1)[1], (c)[2] = (scale1) * (b1)[2])
#define VectorMA(a, scale, b, c) \
	((c)[0] = (a)[0] + (scale) * (b)[0], (c)[1] = (a)[1] + (scale) * (b)[1], (c)[2] = (a)[2] + (scale) * (b)[2])
#define VectorMAM(scale1, b1, scale2, b2, c) \
	((c)[0] = (scale1) * (b1)[0] + (scale2) * (b2)[0], \
	 (c)[1] = (scale1) * (b1)[1] + (scale2) * (b2)[1], \
	 (c)[2] = (scale1) * (b1)[2] + (scale2) * (b2)[2])
#define VectorMAMAM(scale1, b1, scale2, b2, scale3, b3, c) \
	((c)[0] = (scale1) * (b1)[0] + (scale2) * (b2)[0] + (scale3) * (b3)[0], \
	 (c)[1] = (scale1) * (b1)[1] + (scale2) * (b2)[1] + (scale3) * (b3)[1], \
	 (c)[2] = (scale1) * (b1)[2] + (scale2) * (b2)[2] + (scale3) * (b3)[2])
#define VectorIsNull(v) ((v)[0] == 0.0f && (v)[1] == 0.0f && (v)[2] == 0.0f)

float VectorNormalizeLength2(const vec3_t v, vec3_t out);
qboolean VectorCompareEpsilon(const vec3_t vec1, const vec3_t vec2, vec_t epsilon);
void VectorVectors(const vec3_t forward, vec3_t right, vec3_t up);
void VectorAngles(const float* forward, float* angles);
void AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
void VectorsAngles(const vec3_t forward, const vec3_t right, const vec3_t up, vec3_t angles);
