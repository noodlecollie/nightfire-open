#pragma once

#include "BuildPlatform/Typedefs.h"
#include "MathLib/vec3.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef vec_t matrix4x4[4][4];

extern const matrix4x4 m_matrix4x4_identity;

void Matrix4x4_Copy(matrix4x4 out, const matrix4x4 in);
void Matrix4x4_VectorTransform(const matrix4x4 in, const vec3_t v, vec3_t out);
void Matrix4x4_VectorITransform(const matrix4x4 in, const vec3_t v, vec3_t out);
void Matrix4x4_VectorRotate(const matrix4x4 in, const vec3_t v, vec3_t out);
void Matrix4x4_VectorIRotate(const matrix4x4 in, const vec3_t v, vec3_t out);
void Matrix4x4_ConcatTransforms(matrix4x4 out, const matrix4x4 in1, const matrix4x4 in2);
void Matrix4x4_CreateFromEntity(matrix4x4 out, const vec3_t angles, const vec3_t origin, float scale);
void Matrix4x4_TransformPositivePlane(const matrix4x4 in, const vec3_t normal, float d, vec3_t out, float* dist);
void Matrix4x4_ConvertToEntity(const matrix4x4 in, vec3_t angles, vec3_t origin);
void Matrix4x4_Invert_Simple(matrix4x4 out, const matrix4x4 in1);
qboolean Matrix4x4_Invert_Full(matrix4x4 out, const matrix4x4 in1);

static inline void Matrix4x4_LoadIdentity(matrix4x4 mat)
{
	Matrix4x4_Copy(mat, m_matrix4x4_identity);
}

#ifdef __cplusplus
}  // extern "C"
#endif
