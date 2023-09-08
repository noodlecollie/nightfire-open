// basic typedefs
#ifndef XASH_TYPES_H
#define XASH_TYPES_H

#include "PlatformDefs/platformid.h"
#include "PlatformDefs/typedefs.h"
#include "CommonUtils/bitdefs.h"

#if XASH_WIN32
#include <wchar.h>  // off_t
#endif  // _WIN32

#include <sys/types.h>  // off_t
#include <stdint.h>
#include <assert.h>

typedef int sound_t;

#if XASH_64BIT
typedef uint32_t poolhandle_t;
#else
typedef void* poolhandle_t;
#endif

typedef uint64_t longtime_t;

#define GAMMA (2.2f)  // Valve Software gamma
#define INVGAMMA (1.0f / 2.2f)  // back to 1.0
#define TEXGAMMA (0.9f)  // compensate dim textures

#if defined(static_assert)  // C11 static_assert
#define STATIC_ASSERT static_assert
#else
#define STATIC_ASSERT(x, y) extern int _static_assert_##__LINE__[(x) ? 1 : -1]
#endif

typedef struct stream_s stream_t;  // sound stream for background music playing

typedef struct dllfunc_s
{
	const char* name;
	void** func;
} dllfunc_t;

typedef struct dll_info_s
{
	const char* name;  // name of library
	const dllfunc_t* fcts;  // list of dll exports
	qboolean crash;  // crash if dll not found
	void* link;  // hinstance of loading library
} dll_info_t;

typedef void (*setpair_t)(const char* key, const void* value, const void* buffer, void* numpairs);

#endif  // XASH_TYPES_H
