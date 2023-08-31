#pragma once

#include "BuildDefs/build.h"

#ifndef XASH_WIN32
typedef void* HANDLE;
typedef void* HINSTANCE;

typedef struct tagPOINT
{
	int x, y;
} POINT;
#endif // XASH_WIN32
