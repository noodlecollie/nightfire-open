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
