/*
defaults.h - set up default configuration
Copyright (C) 2016 Mittorn

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#pragma once

#include "BuildPlatform/PlatformID.h"
#include "common/engine_builddefs.h"

/*
===================================================================

SETUP BACKENDS DEFINITIONS

===================================================================
*/

// video backends (XASH_VIDEO)
#define VIDEO_NULL 0
#define VIDEO_SDL 1
#define VIDEO_FBDEV 2

// audio backends (XASH_SOUND)
#define SOUND_NULL 0
#define SOUND_SDL 1
#define SOUND_OPENSLES 2
#define SOUND_ALSA 3

// input (XASH_INPUT)
#define INPUT_NULL 0
#define INPUT_SDL 1
#define INPUT_EVDEV 2

// timer (XASH_TIMER)
#define TIMER_NULL 0  // not used
#define TIMER_SDL 1
#define TIMER_POSIX 2
#define TIMER_WIN32 3

// messageboxes (XASH_MESSAGEBOX)
#define MSGBOX_STDERR 0
#define MSGBOX_SDL 1
#define MSGBOX_WIN32 2

// library loading (XASH_LIB)
#define LIB_NULL 0
#define LIB_POSIX 1
#define LIB_WIN32 2
#define LIB_STATIC 3

#if !XASH_DEDICATED()
#if XASH_SDL
// we are building using libSDL
#ifndef XASH_VIDEO
#define XASH_VIDEO VIDEO_SDL
#endif  // XASH_VIDEO

#ifndef XASH_INPUT
#define XASH_INPUT INPUT_SDL
#endif  // XASH_INPUT

#ifndef XASH_SOUND
#define XASH_SOUND SOUND_SDL
#endif  // XASH_SOUND

#if XASH_SDL == 2
#ifndef XASH_TIMER
#define XASH_TIMER TIMER_SDL
#endif  // XASH_TIMER

#ifndef XASH_MESSAGEBOX
#define XASH_MESSAGEBOX MSGBOX_SDL
#endif  // XASH_MESSAGEBOX
#endif
#elif XASH_LINUX()
// we are building for Linux without SDL2, can draw only to framebuffer yet
#ifndef XASH_VIDEO
#define XASH_VIDEO VIDEO_FBDEV
#endif  // XASH_VIDEO

#ifndef XASH_INPUT
#define XASH_INPUT INPUT_EVDEV
#endif  // XASH_INPUT

#ifndef XASH_SOUND
#define XASH_SOUND SOUND_ALSA
#endif  // XASH_SOUND

#define XASH_USE_EVDEV 1
#endif

#endif  // XASH_DEDICATED()

//
// select messagebox implementation
//
#ifndef XASH_MESSAGEBOX
#if XASH_WIN32()
#define XASH_MESSAGEBOX MSGBOX_WIN32
#else  // !XASH_WIN32()
#define XASH_MESSAGEBOX MSGBOX_STDERR
#endif  // !XASH_WIN32()
#endif  // XASH_MESSAGEBOX

//
// no timer - no xash
//
#ifndef XASH_TIMER
#if XASH_WIN32()
#define XASH_TIMER TIMER_WIN32
#else  // !XASH_WIN32()
#define XASH_TIMER TIMER_POSIX
#endif  // !XASH_WIN32()
#endif

#ifdef XASH_STATIC_LIBS
#define XASH_LIB LIB_STATIC
#define XASH_INTERNAL_GAMELIBS
#define XASH_ALLOW_SAVERESTORE_OFFSETS
#elif XASH_WIN32()
#define XASH_LIB LIB_WIN32
#elif XASH_POSIX()
#define XASH_LIB LIB_POSIX
#endif

//
// fallback to NULL
//
#ifndef XASH_VIDEO
#define XASH_VIDEO VIDEO_NULL
#endif  // XASH_VIDEO

#ifndef XASH_SOUND
#define XASH_SOUND SOUND_NULL
#endif  // XASH_SOUND

#ifndef XASH_INPUT
#define XASH_INPUT INPUT_NULL
#endif  // XASH_INPUT

/*
=========================================================================

Default build-depended cvar and constant values

=========================================================================
*/

// Defaults
#ifndef DEFAULT_TOUCH_ENABLE
#define DEFAULT_TOUCH_ENABLE "0"
#endif  // DEFAULT_TOUCH_ENABLE

#ifndef DEFAULT_M_IGNORE
#define DEFAULT_M_IGNORE "0"
#endif  // DEFAULT_M_IGNORE

#ifndef DEFAULT_JOY_DEADZONE
#define DEFAULT_JOY_DEADZONE "4096"
#endif  // DEFAULT_JOY_DEADZONE

#ifndef DEFAULT_DEV
#define DEFAULT_DEV 0
#endif  // DEFAULT_DEV

#ifndef DEFAULT_ALLOWCONSOLE
#define DEFAULT_ALLOWCONSOLE 0
#endif  // DEFAULT_ALLOWCONSOLE

#ifndef DEFAULT_FULLSCREEN
#define DEFAULT_FULLSCREEN 1
#endif  // DEFAULT_FULLSCREEN
