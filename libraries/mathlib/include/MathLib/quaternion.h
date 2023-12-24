#pragma once

#include "BuildPlatform/Typedefs.h"
#include "MathLib/vec3.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef vec_t quat_t[4];

void QuaternionAlign(const quat_t p, const quat_t q, quat_t qt);
void QuaternionSlerp(const quat_t p, const quat_t q, float t, quat_t qt);
void QuaternionSlerpNoAlign(const quat_t p, const quat_t q, float t, quat_t qt);

#ifdef __cplusplus
}  // extern "C"
#endif
