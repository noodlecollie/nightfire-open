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

// This version catches errors where the macro is used
// on a pointer rather than an array.
#define SIZE_OF_ARRAY(a) \
  ((sizeof(a) / sizeof(*(a))) / \
  (size_t)(!(sizeof(a) % sizeof(*(a)))))
