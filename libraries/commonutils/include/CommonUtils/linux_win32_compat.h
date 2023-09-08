#pragma once

#include "PlatformDefs/platformid.h"

#if XASH_WIN32()
#define HSPRITE WINAPI_HSPRITE
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef HSPRITE
#else
typedef void* HANDLE;
typedef void* HINSTANCE;

typedef struct tagPOINT
{
	int x, y;
} POINT;
#endif // XASH_WIN32()
