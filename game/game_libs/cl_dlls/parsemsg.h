/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/
//
//  parsemsg.h
//
#pragma once
#ifndef PARSEMSG_H
#define PARSEMSG_H

#include <cstdint>
#include "MathLib/vec3.h"

void BEGIN_READ(void* buf, int size);
int READ_CHAR(void);
int READ_BYTE(void);
int READ_SHORT(void);
int READ_WORD(void);
int READ_LONG(void);
float READ_FLOAT(void);
char* READ_STRING(void);
float READ_COORD(void);
float READ_ANGLE(void);
float READ_HIRESANGLE(void);

template<typename T>
static inline void READ_BYTES(T* const out)
{
	for ( uint32_t index = 0; index < sizeof(*out); ++index )
	{
		reinterpret_cast<uint8_t*>(out)[index] = static_cast<uint8_t>(READ_BYTE());
	}
}

static inline void READ_VEC_PRECISE(Vector& vec)
{
	for ( uint32_t axis = 0; axis < 3; ++axis )
	{
		float value = 0;
		READ_BYTES(&value);
		vec[axis] = value;
	}
}

#endif  // PARSEMSG_H
