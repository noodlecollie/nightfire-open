/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/
#pragma once
#ifndef EXTDLL_H
#define EXTDLL_H

//
// Global header file for extension DLLs
//

// Allow "DEBUG" in addition to default "_DEBUG"
#ifdef _DEBUG
#define DEBUG 1
#endif

// Prevent tons of unused windows definitions
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOWINRES
#define NOSERVICE
#define NOMCX
#define NOIME
#define HSPRITE HSPRITE_win32
#include "windows.h"
#undef HSPRITE
#else  // _WIN32
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif
typedef unsigned int ULONG;
typedef unsigned char BYTE;
typedef int BOOL;
#define MAX_PATH PATH_MAX
#include <limits.h>
#include <stdarg.h>
#endif  //_WIN32

// Misc C-runtime library headers
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

// NFTODO: Put this somewhere canonical
#if defined(__LP64__) || defined(__LLP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__)) || \
	defined(_M_X64) || defined(__ia64) || defined(_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
#define GAME_64BIT() 1
#else
#define GAME_64BIT() 0
#endif

// Header file containing definition of globalvars_t and entvars_t
typedef unsigned int func_t;
typedef int string_t;  // from engine's pr_comp.h;

#include "vector_classes.h"

// Shared engine/DLL constants
#include "XashDefs/const.h"
#include "XashDefs/progdefs.h"
#include "XashDefs/edict.h"

// Shared header describing protocol between engine and DLLs
#include "XashDefs/eiface.h"

// Shared header between the client DLL and the game DLLs
#include "cdll_dll.h"
#ifndef Q_min
#define Q_min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef Q_max
#define Q_max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#endif  // EXTDLL_H
