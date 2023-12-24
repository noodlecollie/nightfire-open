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

#include "BuildPlatform/PlatformID.h"
#include "BuildPlatform/Typedefs.h"
#include "MathLib/mathdefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern const int boxpnt[6][4];

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
word FloatToHalf(float v);
float HalfToFloat(word h);
int NearestPOW(int value, qboolean roundDown);
void SinCos(float radians, float* sine, float* cosine);
int Q_log2(int val);

#ifdef __cplusplus
}  // extern "C"
#endif
