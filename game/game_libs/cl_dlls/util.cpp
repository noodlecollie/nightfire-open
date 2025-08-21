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
#include "EnginePublicAPI/triangleapi.h"

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

void RenderTextAtWorldLocation(const Vector& position, const char* text, uint32_t colour, int lineOffset)
{
	// This will be in the range (-1, -1) to (1, 1).
	Vector locationOnScreen;
	const bool isBehind = gEngfuncs.pTriAPI->WorldToScreen(position, locationOnScreen);

	if ( isBehind )
	{
		// No point drawing.
		return;
	}

	const Vector2D screenDim(static_cast<float>(gHUD.m_scrinfo.iWidth), static_cast<float>(gHUD.m_scrinfo.iHeight));
	const Vector2D halfScreenDim = screenDim / 2.0f;
	int x = static_cast<int>(halfScreenDim.x + (locationOnScreen.x * halfScreenDim.x));
	int y = static_cast<int>(halfScreenDim.y - (locationOnScreen.y * halfScreenDim.y));

	int lineWidth = 0;
	int lineHeight = 0;
	gEngfuncs.pfnDrawStringLen(text, &lineWidth, &lineHeight);

	x -= lineWidth / 2;
	y += lineOffset * lineHeight;

	gEngfuncs.pfnDrawString(x, y, text, ColChannel<0>(colour), ColChannel<1>(colour), ColChannel<2>(colour));
}
