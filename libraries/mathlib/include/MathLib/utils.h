#pragma once

#include "BuildPlatform/Typedefs.h"
#include "MathLib/vec3.h"
#include "MathLib/mat3x4.h"
#include "MathLib/quaternion.h"
#include "MathLib/angles.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define bound(min, num, max) ((num) >= (min) ? ((num) < (max) ? (num) : (max)) : (min))

// remap a value in the range [A,B] to [C,D].
static inline float RemapVal(float val, float A, float B, float C, float D)
{
	return C + (D - C) * (val - A) / (B - A);
}

static inline float RemapValClamped(float val, float A, float B, float C, float D)
{
	if ( A == B )
	{
		return val >= B ? D : C;
	}

	float cVal = (val - A) / (B - A);
	cVal = bound(0.0f, cVal, 1.0f);

	return C + (D - C) * cVal;
}

void VectorAngles(const vec3_t forward, vec3_t angles);
void AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
void AngleVectorsTranspose(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
void VectorsAngles(const vec3_t forward, const vec3_t right, const vec3_t up, vec3_t angles);
float AngleBetweenVectors(const vec3_t v1, const vec3_t v2);
float VectorToYaw(const vec3_t src);
void GenerateBasisVectors(const vec3_t forward, vec3_t right, vec3_t up);

void Matrix3x4_AnglesFromMatrix(const matrix3x4 in, vec3_t out);
void Matrix3x4_AnglesToMatrix(const vec3_t angles, matrix3x4 matrix);
void Matrix3x4_AnglesToIMatrix(const vec3_t angles, matrix3x4 matrix);

void AngleQuaternion(const vec3_t angles, quat_t q, qboolean studio);
void QuaternionAngle(const quat_t q, vec3_t angles);

float V_CalcFov(float* fov_x, float width, float height);
void V_AdjustFov(float* fov_x, float* fov_y, float width, float height, qboolean lock_x);

void RoundUpHullSize(vec3_t size);
void ClearBounds(vec3_t mins, vec3_t maxs);
qboolean BoundsAreCleared(vec3_t mins, vec3_t maxs);
void AddPointToBounds(const vec3_t v, vec3_t mins, vec3_t maxs);

qboolean BoundsIntersect(const vec3_t mins1, const vec3_t maxs1, const vec3_t mins2, const vec3_t maxs2);
qboolean BoundsAndSphereIntersect(const vec3_t mins, const vec3_t maxs, const vec3_t origin, float radius);
qboolean
SphereIntersect(const vec3_t vSphereCenter, float fSphereRadiusSquared, const vec3_t vLinePt, const vec3_t vLineDir);

float RadiusFromBounds(const vec3_t mins, const vec3_t maxs);
void ExpandBounds(vec3_t mins, vec3_t maxs, float offset);
float ApproachVal(float target, float value, float speed);

// NFTODO: Move this somewhere better
void R_StudioSlerpBones(int numbones, quat_t q1[], float pos1[][3], const quat_t q2[], const float pos2[][3], float s);

#ifdef __cplusplus
}  // extern "C"
#endif
