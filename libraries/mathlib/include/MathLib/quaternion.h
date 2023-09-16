#pragma once

#include "PlatformDefs/typedefs.h"
#include "MathLib/vec3.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef vec_t quat_t[4];

void AngleQuaternion(const vec3_t angles, quat_t q, qboolean studio);
void QuaternionAngle(const quat_t q, vec3_t angles);
void QuaternionSlerp(const quat_t p, const quat_t q, float t, quat_t qt);

// NFTODO: Move this somewhere better
void R_StudioSlerpBones(int numbones, quat_t q1[], float pos1[][3], const quat_t q2[], const float pos2[][3], float s);

#ifdef __cplusplus
}  // extern "C"
#endif
