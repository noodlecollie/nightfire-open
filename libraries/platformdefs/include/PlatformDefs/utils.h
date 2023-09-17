#pragma once

#include <stddef.h>

#define PLAT_STRINGIFY_TOKEN(x) #x
#define PLAT_STRINGIFY_VALUE(x) PLAT_STRINGIFY_TOKEN(x)

#ifndef offsetof
#ifdef __GNUC__
#define offsetof(s, m) __builtin_offsetof(s, m)
#else
#define offsetof(s, m) (size_t) & (((s*)0)->m)
#endif
#endif

#define ALIGN(x, a) (((size_t)(x) + ((size_t)(a)-1)) & ~((size_t)(a)-1))

#ifdef __cplusplus
template<typename T, size_t SIZE>
constexpr size_t SIZE_OF_ARRAY(T (&)[SIZE])
{
	return SIZE;
}
#else
// This version catches errors where the macro is used
// on a pointer rather than an array.
#define SIZE_OF_ARRAY(a) ((sizeof(a) / sizeof(*(a))) / (size_t)(!(sizeof(a) % sizeof(*(a)))))
#endif
