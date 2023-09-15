#pragma once

#include "MathLib/mathdefs.h"
#include "MathLib/mathtypes.h"
#include "Mathlib/vec3.h"

extern const matrix3x4 m_matrix3x4_identity;
extern const matrix4x4 m_matrix4x4_identity;

#define Matrix3x4_LoadIdentity(mat) Matrix3x4_Copy(mat, m_matrix3x4_identity)
#define Matrix3x4_Copy(out, in) memcpy(out, in, sizeof(matrix3x4))

void Matrix3x4_VectorTransform(const matrix3x4 in, const float v[3], float out[3]);
void Matrix3x4_VectorITransform(const matrix3x4 in, const float v[3], float out[3]);
void Matrix3x4_VectorRotate(const matrix3x4 in, const float v[3], float out[3]);
void Matrix3x4_VectorIRotate(const matrix3x4 in, const float v[3], float out[3]);
void Matrix3x4_ConcatTransforms(matrix3x4 out, const matrix3x4 in1, const matrix3x4 in2);
void Matrix3x4_FromOriginQuat(matrix3x4 out, const vec4_t quaternion, const vec3_t origin);
void Matrix3x4_CreateFromEntity(matrix3x4 out, const vec3_t angles, const vec3_t origin, float scale);
void Matrix3x4_TransformAABB(const matrix3x4 world, const vec3_t mins, const vec3_t maxs, vec3_t absmin, vec3_t absmax);
void Matrix3x4_SetOrigin(matrix3x4 out, float x, float y, float z);
void Matrix3x4_OriginFromMatrix(const matrix3x4 in, float* out);
void Matrix3x4_AnglesFromMatrix(const matrix3x4 in, vec3_t out);

#define Matrix4x4_LoadIdentity(mat) Matrix4x4_Copy(mat, m_matrix4x4_identity)
#define Matrix4x4_Copy(out, in) memcpy(out, in, sizeof(matrix4x4))

void Matrix4x4_VectorTransform(const matrix4x4 in, const float v[3], float out[3]);
void Matrix4x4_VectorITransform(const matrix4x4 in, const float v[3], float out[3]);
void Matrix4x4_VectorRotate(const matrix4x4 in, const float v[3], float out[3]);
void Matrix4x4_VectorIRotate(const matrix4x4 in, const float v[3], float out[3]);
void Matrix4x4_ConcatTransforms(matrix4x4 out, const matrix4x4 in1, const matrix4x4 in2);
void Matrix4x4_CreateFromEntity(matrix4x4 out, const vec3_t angles, const vec3_t origin, float scale);
void Matrix4x4_TransformPositivePlane(const matrix4x4 in, const vec3_t normal, float d, vec3_t out, float* dist);
void Matrix4x4_ConvertToEntity(const matrix4x4 in, vec3_t angles, vec3_t origin);
void Matrix4x4_Invert_Simple(matrix4x4 out, const matrix4x4 in1);
qboolean Matrix4x4_Invert_Full(matrix4x4 out, const matrix4x4 in1);
