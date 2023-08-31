#pragma once

#include <stdint.h>
#include <stddef.h>

// This version catches errors where the macro is used
// on a pointer rather than an array.
#define SIZE_OF_ARRAY(a) \
  ((sizeof(a) / sizeof(*(a))) / \
  (size_t)(!(sizeof(a) % sizeof(*(a)))))

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

// Numerical types
typedef unsigned char byte;
typedef uint32_t uint;
typedef uint16_t ushort;

// Floats, vectors and other tuple types
typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t quat_t[4];
typedef byte rgba_t[4];  // unsigned byte colorpack
typedef byte rgb_t[3];  // unsigned byte colorpack
typedef vec_t matrix3x4[3][4];
typedef vec_t matrix4x4[4][4];

// Convenience aliases for standard types, used in various places in code:
typedef ushort word;
typedef uint dword;

// General-purpose string: a medium-sized array of chars
#define MAX_STRING 256
typedef char string[MAX_STRING];

typedef struct mplane_s
{
	vec3_t normal;
	vec_t dist;
	byte type;  // for fast side tests
	byte signbits;  // signx + (signy<<1) + (signz<<1)
	byte pad[2];
} mplane_t;
