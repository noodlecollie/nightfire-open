#pragma once

#include "BuildPlatform/PlatformID.h"

#if XASH_WIN32()
#define OS_LIB_PREFIX ""
#define OS_LIB_EXT "dll"
#else
#define OS_LIB_PREFIX "lib"
#define OS_LIB_EXT "so"
#endif
