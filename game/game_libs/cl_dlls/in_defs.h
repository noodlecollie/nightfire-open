//========= Copyright (c) 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#pragma once

#ifdef _WIN32
#define HSPRITE HSPRITE_win32
#include <windows.h>
#undef HSPRITE
#else
typedef struct point_s
{
	int x;
	int y;
} POINT;
#define GetCursorPos(x)
#define SetCursorPos(x, y)
#endif
