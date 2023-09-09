#pragma once

// This file caters for definitions that should be provided by CMake,
// and translates them to more easily debuggable internal definitions.

#ifdef BUILDFLAG_IS_DEDICATED_SERVER
#define XASH_DEDICATED() 1
#else
#define XASH_DEDICATED() 0
#endif

#ifdef BUILDFLAG_BUILD_ENGINE_TESTS
#define XASH_ENGINE_TESTS() 1
#else
#define XASH_ENGINE_TESTS() 0
#endif
