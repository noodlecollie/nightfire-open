#pragma once

#include "BuildPlatform/Typedefs.h"
#include "MathLib/quaternion.h"
#include "MathLib/vec3.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef vec_t matrix3x4[3][4];

extern const matrix3x4 m_matrix3x4_identity;

// NFTODO: Switch all of these these so that output matrix is last, otherwise it's way too confusing
void Matrix3x4_Copy(matrix3x4 out, const matrix3x4 in);
void Matrix3x4_ConcatTransforms(matrix3x4 out, const matrix3x4 in1, const matrix3x4 in2);
void Matrix3x4_FromOriginQuat(matrix3x4 out, const quat_t quaternion, const vec3_t origin);
void Matrix3x4_FromQuat(matrix3x4 out, const quat_t quaternion);
void Matrix3x4_CreateFromEntityScale3f(matrix3x4 out, const vec3_t angles, const vec3_t origin, const vec3_t scale);
void Matrix3x4_TransformAABB(const matrix3x4 world, const vec3_t mins, const vec3_t maxs, vec3_t absmin, vec3_t absmax);
void Matrix3x4_GetOrientationVectors(const matrix3x4 matrix, vec3_t forward, vec3_t right, vec3_t up);

void Matrix3x4_SetOrigin(matrix3x4 out, float x, float y, float z);
void Matrix3x4_OriginFromMatrix(const matrix3x4 in, vec3_t out);

void Matrix3x4_VectorTransform(const matrix3x4 in, const vec3_t v, vec3_t out);
void Matrix3x4_VectorITransform(const matrix3x4 in, const vec3_t v, vec3_t out);
void Matrix3x4_VectorRotate(const matrix3x4 in, const vec3_t v, vec3_t out);
void Matrix3x4_VectorIRotate(const matrix3x4 in, const vec3_t v, vec3_t out);

vec_t Matrix3x4_CalcSign(const matrix3x4 in);

static inline void Matrix3x4_LoadIdentity(matrix3x4 mat)
{
	Matrix3x4_Copy(mat, m_matrix3x4_identity);
}

static inline void Matrix3x4_CreateFromEntity(matrix3x4 out, const vec3_t angles, const vec3_t origin, float scale)
{
	const vec3_t scaleVec = { scale, scale, scale };
	Matrix3x4_CreateFromEntityScale3f(out, angles, origin, scaleVec);
}

#ifdef __cplusplus
}  // extern "C"
#endif
