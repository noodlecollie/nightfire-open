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
// util.cpp
//
// implementation of class-less helper functions
//

#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "hud.h"
#include "cl_util.h"
#include <string.h>
#include "PlatformLib/String.h"

HSPRITE LoadSprite(const char* pszName)
{
	int i;
	char sz[256];

	if ( ScreenWidth < 640 )
		i = 320;
	else
		i = 640;

	PlatformLib_SNPrintF(sz, sizeof(sz), pszName, i);

	return SPR_Load(sz);
}
