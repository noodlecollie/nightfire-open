#pragma once

#include "BuildPlatform/Typedefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef vec_t matrix3x3[3][3];

extern const matrix3x3 m_matrix3x3_identity;

void Matrix3x3_Copy(matrix3x3 out, const matrix3x3 in);

static inline void Matrix3x3_LoadIdentity(matrix3x3 mat)
{
	Matrix3x3_Copy(mat, m_matrix3x3_identity);
}

#ifdef __cplusplus
}  // extern "C"
#endif
