#pragma once

#include "BuildPlatform/Typedefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef vec_t vec4_t[4];

static inline void Vector4Copy(const vec4_t a, vec4_t b)
{
	b[0] = a[0];
	b[1] = a[1];
	b[2] = a[2];
	b[3] = a[3];
}

static inline void Vector4Set(vec4_t v, vec_t a, vec_t b, vec_t c, vec_t d)
{
	v[0] = a;
	v[1] = b;
	v[2] = c;
	v[3] = d;
}

#ifdef __cplusplus
}  // extern "C"
#endif
