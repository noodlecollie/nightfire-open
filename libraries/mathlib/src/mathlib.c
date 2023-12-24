/*
mathlib.c - internal mathlib
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "MathLib/mathlib.h"
#include "MathLib/vec3.h"
#include "MathLib/vec4.h"
#include "MathLib/angles.h"
#include "MathLib/mat3x4.h"
#include "MathLib/quaternion.h"
#include "MathLib/plane.h"
#include "MathLib/utils.h"
#include "BuildPlatform/Utils.h"

const int boxpnt[6][4] = {
	{0, 4, 6, 2},  // +X
	{0, 1, 5, 4},  // +Y
	{0, 2, 3, 1},  // +Z
	{7, 5, 1, 3},  // -X
	{7, 3, 2, 6},  // -Y
	{7, 6, 4, 5},  // -Z
};

word FloatToHalf(float v)
{
	unsigned int i = FloatAsUint(v);
	unsigned int e = (i >> 23) & 0x00ff;
	unsigned int m = i & 0x007fffff;
	unsigned short h;

	if ( e <= 127 - 15 )
	{
		h = (unsigned short)(((m | 0x00800000) >> (127 - 14 - e)) >> 13);
	}
	else
	{
		h = (i >> 13) & 0x3fff;
	}

	h |= (i >> 16) & 0xc000;

	return h;
}

float HalfToFloat(word h)
{
	unsigned int f = (h << 16) & 0x80000000;
	unsigned int em = h & 0x7fff;

	if ( em > 0x03ff )
	{
		f |= (em << 13) + ((127 - 15) << 23);
	}
	else
	{
		unsigned int m = em & 0x03ff;

		if ( m != 0 )
		{
			unsigned int e = (em >> 10) & 0x1f;

			while ( (m & 0x0400) == 0 )
			{
				m <<= 1;
				e--;
			}

			m &= 0x3ff;
			f |= ((e + (127 - 14)) << 23) | (m << 13);
		}
	}

	return UintAsFloat(f);
}


int NearestPOW(int value, qboolean roundDown)
{
	int n = 1;

	if ( value <= 0 )
		return 1;
	while ( n < value )
		n <<= 1;

	if ( roundDown )
	{
		if ( n > value )
			n >>= 1;
	}
	return n;
}

float rsqrt(float number)
{
	int i;
	float x, y;

	if ( number == 0.0f )
		return 0.0f;

	x = number * 0.5f;
	i = FloatAsInt(number);  // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);  // what the fuck?
	y = IntAsFloat(i);
	y = y * (1.5f - (x * y * y));  // first iteration

	return y;
}

void SinCos(float radians, float* sine, float* cosine)
{
#if _MSC_VER == 1200
	_asm
		{
		fld	dword ptr [radians]
		fsincos

		mov edx, dword ptr [cosine]
		mov eax, dword ptr [sine]

		fstp dword ptr [edx]
		fstp dword ptr [eax]
		}
#else
	*sine = sinf(radians);
	*cosine = cosf(radians);
#endif
}

int Q_log2(int val)
{
	int answer = 0;

	while ( val >>= 1 )
	{
		answer++;
	}

	return answer;
}
