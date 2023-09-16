#pragma once

#include "PlatformDefs/typedefs.h"
#include "MathLib/vec3.h"

#define bound(min, num, max) ((num) >= (min) ? ((num) < (max) ? (num) : (max)) : (min))

float V_CalcFov(float* fov_x, float width, float height);
void V_AdjustFov(float* fov_x, float* fov_y, float width, float height, qboolean lock_x);

void RoundUpHullSize(vec3_t size);
void ClearBounds(vec3_t mins, vec3_t maxs);
void AddPointToBounds(const vec3_t v, vec3_t mins, vec3_t maxs);
qboolean BoundsIntersect(const vec3_t mins1, const vec3_t maxs1, const vec3_t mins2, const vec3_t maxs2);
qboolean BoundsAndSphereIntersect(const vec3_t mins, const vec3_t maxs, const vec3_t origin, float radius);
qboolean
SphereIntersect(const vec3_t vSphereCenter, float fSphereRadiusSquared, const vec3_t vLinePt, const vec3_t vLineDir);
float RadiusFromBounds(const vec3_t mins, const vec3_t maxs);
void ExpandBounds(vec3_t mins, vec3_t maxs, float offset);
float RemapVal(float val, float A, float B, float C, float D);
float ApproachVal(float target, float value, float speed);
