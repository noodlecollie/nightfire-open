/*
extdll.h - must be included into the all ui files
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#ifndef EXTDLL_H
#define EXTDLL_H

#ifndef EXPORT
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define EXPORT __attribute__((visibility("default")))
#else
#define EXPORT
#endif
#endif

// disable c++11 on old msvc
#if defined(_MSC_VER) && _MSC_VER < 1800 && !defined CXX11_NOT_PRESENT
#define CXX11_NOT_PRESENT
#endif

// Misc C-runtime library headers
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#ifdef CXX11_NOT_PRESENT
// C++11 keywords
#define final
#define constexpr
#define override
#define nullptr NULL
#endif

#ifdef bound
#undef bound
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

typedef int (*cmpfunc)(const void* a, const void* b);
typedef int BOOL;
typedef int qboolean;
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned int uint32;  //!!!

#include "EngineInternalAPI/menu_int.h"

#endif  // EXTDLL_H
