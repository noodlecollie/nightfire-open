#pragma once

#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus) || defined(__bool_true_false_are_defined)
// C++ already has bool. Typedef qboolean to be an int,
// so that it's the same width as the C enum below.
typedef int qboolean;
#else
typedef enum
{
	false,
	true
} qboolean;
#endif

// TODO: Deprecate some of these types in favour of stdint names, to reduce confusion.

// Numerical types
typedef unsigned char byte;
typedef uint32_t uint;
typedef uint16_t ushort;

typedef float vec_t;
typedef byte rgba_t[4];  // unsigned byte colorpack
typedef byte rgb_t[3];  // unsigned byte colorpack

// Convenience aliases for standard types, used in various places in code:
typedef ushort word;
typedef uint dword;

// General-purpose string: a medium-sized array of chars
#define MAX_STRING 256
typedef char string[MAX_STRING];
