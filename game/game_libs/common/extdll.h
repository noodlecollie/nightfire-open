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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Shared engine/DLL constants
#include "EnginePublicAPI/const.h"
#include "EnginePublicAPI/progdefs.h"
#include "EnginePublicAPI/edict.h"

// Shared header describing protocol between engine and DLLs
#include "EnginePublicAPI/eiface.h"

// Shared header between the client DLL and the game DLLs
#include "cdll_dll.h"
