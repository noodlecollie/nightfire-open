#pragma once

#include "MathLib/vec3.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	PITCH = 0,
	YAW,
	ROLL
} EulerAngleAxis_e;

static inline vec_t anglemod(vec_t a)
{
	return (360.0f / 65536) * ((int)(a * (65536 / 360.0f)) & 65535);
}

void NormalizeAngles(vec3_t angles);
void NormalizeAnglesOut(const vec3_t in, vec3_t out);
void InterpolateAngles(const vec3_t start, const vec3_t end, vec3_t output, float frac);

#ifdef __cplusplus
}  // extern "C"
#endif
