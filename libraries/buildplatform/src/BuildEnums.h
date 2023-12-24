/*
build.h - compile-time build information
Copyright (C) 2023 Alibek Omarov

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

// This header defines the enumeration values that can be passed to Q_build*
// functions and get current value through XASH_PLATFORM, XASH_ARCHITECTURE and
// XASH_ARCHITECTURE_ABI defines

//================================================================
//
//           OPERATING SYSTEM DEFINES
//
//================================================================

#define PLATFORM_WIN32 1
#define PLATFORM_LINUX 2
#define PLATFORM_LINUX_UNKNOWN 3

#if XASH_WIN32()
#define XASH_PLATFORM() PLATFORM_WIN32
#elif XASH_LINUX_UNKNOWN()
#define XASH_PLATFORM() PLATFORM_LINUX_UNKNOWN
#elif XASH_LINUX()
#define XASH_PLATFORM() PLATFORM_LINUX
#else
#error Unsupported platform!
#endif

//================================================================
//
//           CPU ARCHITECTURE DEFINES
//
//================================================================

#define ARCHITECTURE_AMD64 1
#define ARCHITECTURE_X86 2
#define ARCHITECTURE_ARM 3
#define ARCHITECTURE_MIPS 4
#define ARCHITECTURE_E2K 5
#define ARCHITECTURE_RISCV 6

#if XASH_AMD64()
#define XASH_ARCHITECTURE() ARCHITECTURE_AMD64
#elif XASH_X86()
#define XASH_ARCHITECTURE() ARCHITECTURE_X86
#elif XASH_ARM()
#define XASH_ARCHITECTURE() ARCHITECTURE_ARM
#elif XASH_MIPS()
#define XASH_ARCHITECTURE() ARCHITECTURE_MIPS
#elif XASH_E2K()
#define XASH_ARCHITECTURE() ARCHITECTURE_E2K
#elif XASH_RISCV()
#define XASH_ARCHITECTURE() ARCHITECTURE_RISCV
#else
#error Unknown architecture!
#endif

//================================================================
//
//           ENDIANNESS DEFINES
//
//================================================================

#define ENDIANNESS_LITTLE 1
#define ENDIANNESS_BIG 2

#if XASH_LITTLE_ENDIAN()
#define XASH_ENDIANNESS() ENDIANNESS_LITTLE
#elif XASH_BIG_ENDIAN()
#define XASH_ENDIANNESS() ENDIANNESS_BIG
#else
#error Unknown endianness!
#endif

//================================================================
//
//           APPLICATION BINARY INTERFACE
//
//================================================================

#define ARCHITECTURE_ARM_VER_MASK ((1 << 5) - 1)
#define ARCHITECTURE_ARM_VER_SHIFT 0
#define ARCHITECTURE_ARM_HARDFP (1 << 5)

#define ARCHITECTURE_RISCV_FP_SOFT 0
#define ARCHITECTURE_RISCV_FP_SINGLE 1
#define ARCHITECTURE_RISCV_FP_DOUBLE 2

#if XASH_ARCHITECTURE() == ARCHITECTURE_ARM
#if XASH_ARM_HARDFP()
#define XASH_ARCHITECTURE_ABI() (ARCHITECTURE_ARM_HARDFP | XASH_ARM())
#else
#define XASH_ARCHITECTURE_ABI() (XASH_ARM())
#endif
#elif XASH_ARCHITECTURE() == ARCHITECTURE_RISCV
#if XASH_RISCV_SOFTFP()
#define XASH_ARCHITECTURE_ABI() ARCHITECTURE_RISCV_FP_SOFT
#elif XASH_RISCV_SINGLEFP()
#define XASH_ARCHITECTURE_ABI() ARCHITECTURE_RISCV_FP_SINGLE
#elif XASH_RISCV_DOUBLEFP()
#define XASH_ARCHITECTURE_ABI() ARCHITECTURE_RISCV_FP_DOUBLE
#else
#error Unknown RISCV architecture!
#endif
#else
#define XASH_ARCHITECTURE_ABI() 0  // unused
#endif
