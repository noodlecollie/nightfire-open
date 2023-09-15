/*
mathlib.h - base math functions
Copyright (C) 2007 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#pragma once

#include <stdlib.h>
#include <string.h>

#if HAVE_TGMATH_H
#include <tgmath.h>
#endif

#include "PlatformDefs/platformid.h"
#include "PlatformDefs/typedefs.h"
#include "MathLib/mathdefs.h"
#include "MathLib/mathtypes.h"

#define MakeRGBA(out, x, y, z, w) Vector4Set(out, (byte)x, (byte)y, (byte)z, (byte)w)
#define PlaneDist(point, plane) ((plane)->type < 3 ? (point)[(plane)->type] : DotProduct((point), (plane)->normal))
#define PlaneDiff(point, plane) \
	(((plane)->type < 3 ? (point)[(plane)->type] : DotProduct((point), (plane)->normal)) - (plane)->dist)
#define bound(min, num, max) ((num) >= (min) ? ((num) < (max) ? (num) : (max)) : (min))

// horrible cast but helps not breaking strict aliasing in mathlib
// as union type punning should be fine in C but not in C++
// so don't carry over this to C++ code
typedef union
{
	float fl;
	uint32_t u;
	int32_t i;
} float_bits_t;

static inline uint32_t FloatAsUint(float v)
{
	float_bits_t bits = {v};
	return bits.u;
}

static inline int32_t FloatAsInt(float v)
{
	float_bits_t bits = {v};
	return bits.i;
}

static inline float IntAsFloat(int32_t i)
{
	float_bits_t bits;
	bits.i = i;
	return bits.fl;
}

static inline float UintAsFloat(uint32_t u)
{
	float_bits_t bits;
	bits.u = u;
	return bits.fl;
}

float rsqrt(float number);
float anglemod(float a);
word FloatToHalf(float v);
float HalfToFloat(word h);
int NearestPOW(int value, qboolean roundDown);
void SinCos(float radians, float* sine, float* cosine);
float RemapVal(float val, float A, float B, float C, float D);
float ApproachVal(float target, float value, float speed);

float V_CalcFov(float* fov_x, float width, float height);
void V_AdjustFov(float* fov_x, float* fov_y, float width, float height, qboolean lock_x);
