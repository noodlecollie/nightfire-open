// basic typedefs
#ifndef XASH_TYPES_H
#define XASH_TYPES_H

#include "BuildDefs/build.h"
#include "CommonUtil/typedefs.h"
#include "CommonUtil/bitdefs.h"

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

#undef true
#undef false

typedef uint64_t longtime_t;

#define MAX_INFO_STRING 256  // infostrings are transmitted across network
#define MAX_SERVERINFO_STRING 512  // server handles too many settings. expand to 1024?
#define MAX_LOCALINFO_STRING 32768  // localinfo used on server and not sended to the clients
#define MAX_SYSPATH 1024  // system filepath
#define MAX_VA_STRING 1024  // string length returned by va()
#define MAX_PRINT_MSG 8192  // how many symbols can handle single call of Con_Printf or Con_DPrintf
#define MAX_TOKEN 2048  // parse token length
#define MAX_MODS 512  // environment games that engine can keep visible
#define MAX_USERMSG_LENGTH 2048  // don't modify it's relies on a client-side definitions

#define GAMMA (2.2f)  // Valve Software gamma
#define INVGAMMA (1.0f / 2.2f)  // back to 1.0
#define TEXGAMMA (0.9f)  // compensate dim textures

#ifndef __cplusplus
#ifdef NULL
#undef NULL
#endif

#define NULL ((void*)0)
#endif

#if defined(static_assert)  // C11 static_assert
#define STATIC_ASSERT static_assert
#else
#define STATIC_ASSERT(x, y) extern int _static_assert_##__LINE__[(x) ? 1 : -1]
#endif

typedef struct file_s file_t;  // normal file
typedef struct stream_s stream_t;  // sound stream for background music playing
typedef off_t fs_offset_t;
#if XASH_WIN32
typedef int fs_size_t;  // return type of _read, _write funcs
#else /* !XASH_WIN32 */
typedef ssize_t fs_size_t;
#endif /* !XASH_WIN32 */

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

// config strings are a general means of communication from
// the server to all connected clients.
// each config string can be at most CS_SIZE characters.

#define MAX_QPATH 64  // max length of a game pathname
#define MAX_OSPATH 260  // max length of a filesystem pathname
#define CS_SIZE 64  // size of one config string
#define CS_TIME 16  // size of time string

#endif  // XASH_TYPES_H
