#pragma once

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4127) // Conditional expression is constant
#pragma warning(disable : 4244) // Conversion with possible loss of data
#pragma warning(disable : 4116) // Unnamed type definition in parentheses
#pragma warning(disable : 4701) // Potentially uninitialised local variable used
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include "nuklear/nuklear.h"

#ifdef _MSC_VER
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif
