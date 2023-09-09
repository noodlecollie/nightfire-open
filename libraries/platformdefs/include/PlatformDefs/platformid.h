/*
build.h - compile-time build information

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/

#pragma once

// Xash had its own conventions for these preprocessor macros,
// but I didn't like them. Treating the macros as function invocations
// means that compilation fails if the header is not included, instead
// of #if or #ifdef silently failing.

#define XASH_WIN32() 0
#define XASH_POSIX() 0
#define XASH_LINUX() 0
#define XASH_LINUX_UNKNOWN() 0

#define XASH_LITTLE_ENDIAN() 0
#define XASH_BIG_ENDIAN() 0

#define XASH_64BIT() 0
#define XASH_AMD64() 0
#define XASH_X86() 0

#define XASH_ARM() 0
#define XASH_ARMv4() 0
#define XASH_ARMv5() 0
#define XASH_ARMv6() 0
#define XASH_ARMv7() 0
#define XASH_ARMv8() 0
#define XASH_ARM_HARDFP() 0
#define XASH_ARM_SOFTFP() 0

#define XASH_RISCV() 0
#define XASH_RISCV_SOFTFP() 0
#define XASH_RISCV_SINGLEFP() 0
#define XASH_RISCV_DOUBLEFP() 0

#define XASH_MIPS() 0
#define XASH_E2K() 0

//================================================================
//
//           PLATFORM DETECTION CODE
//
//================================================================

#ifdef _WIN32

// Platform is Windows
#undef XASH_WIN32
#define XASH_WIN32() 1

#else  // _WIN32

// Platform is not Windows, so is assumed to be POSIX-compliant.
#undef XASH_POSIX
#define XASH_POSIX() 1

// Which platform is it specifically?
#ifdef __linux__

// Platform is Linux
#undef XASH_LINUX
#define XASH_LINUX() 1

#include <features.h>

#ifndef __GLIBC__
// This is some weird variant of Linux.
#undef XASH_LINUX_UNKNOWN
#define XASH_LINUX_UNKNOWN() 1
#endif  // __GLIBC__

#endif  // __linux__
#endif  // _WIN32

// This should shake out to either Windows or Linux, as far as we're concerned.
#if !XASH_WIN32() && !XASH_LINUX()
#error Unsupported platform!
#endif  // !XASH_WIN32() && !XASH_LINUX()

//================================================================
//
//           ENDIANNESS DEFINES
//
//================================================================

// Easy, explicit cases:
#if __LITTLE_ENDIAN__
#undef XASH_LITTLE_ENDIAN
#define XASH_LITTLE_ENDIAN() 1
#elif __BIG_ENDIAN__
#undef XASH_BIG_ENDIAN
#define XASH_BIG_ENDIAN() 1
#elif defined __BYTE_ORDER__ && defined __ORDER_BIG_ENDIAN__ && \
	defined __ORDER_LITTLE_ENDIAN__  // some compilers define this
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#undef XASH_BIG_ENDIAN
#define XASH_BIG_ENDIAN() 1
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#undef XASH_LITTLE_ENDIAN
#define XASH_LITTLE_ENDIAN() 1
#endif  // __BYTE_ORDER__
#endif  // __LITTLE_ENDIAN__

// Platform-based checks are separate, just to make sure they
// don't conflict with the above (we sanity check at the end).
#if XASH_WIN32()
// We treat all Windows installs as little-endian
#undef XASH_LITTLE_ENDIAN
#define XASH_LITTLE_ENDIAN() 1
#else
#include <sys/param.h>
#if __BYTE_ORDER == __BIG_ENDIAN
#undef XASH_BIG_ENDIAN
#define XASH_BIG_ENDIAN() 1
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#undef XASH_LITTLE_ENDIAN
#define XASH_LITTLE_ENDIAN() 1
#endif
#endif  //  XASH_WIN32()

// Sanity:
#if XASH_BIG_ENDIAN() == XASH_LITTLE_ENDIAN()
#error Platform must be either big-endian or little-endian!
#endif

//================================================================
//
//           CPU ARCHITECTURE DEFINES
//
//================================================================

#if defined __x86_64__ || defined _M_X64
#undef XASH_64BIT
#define XASH_64BIT() 1
#undef XASH_AMD64
#define XASH_AMD64() 1
#elif defined __i386__ || defined _X86_ || defined _M_IX86
#undef XASH_X86
#define XASH_X86() 1
#elif defined __aarch64__ || defined _M_ARM64
#undef XASH_64BIT
#define XASH_64BIT() 1
#undef XASH_ARM
#define XASH_ARM() 8
#elif defined __mips__
#undef XASH_MIPS
#define XASH_MIPS() 1
#elif defined __e2k__
#undef XASH_64BIT
#define XASH_64BIT() 1
#undef XASH_E2K
#define XASH_E2K() 1
#elif defined _M_ARM  // MSVC
#undef XASH_ARM
#define XASH_ARM() 7
#undef XASH_ARM_HARDFP
#define XASH_ARM_HARDFP() 1
#elif defined __arm__
#if __ARM_ARCH == 8 || __ARM_ARCH_8__
#undef XASH_ARM
#define XASH_ARM() 8
#elif __ARM_ARCH == 7 || __ARM_ARCH_7__
#undef XASH_ARM
#define XASH_ARM() 7
#elif __ARM_ARCH == 6 || __ARM_ARCH_6__ || __ARM_ARCH_6J__
#undef XASH_ARM
#define XASH_ARM() 6
#elif __ARM_ARCH == 5 || __ARM_ARCH_5__
#undef XASH_ARM
#define XASH_ARM() 5
#elif __ARM_ARCH == 4 || __ARM_ARCH_4__
#undef XASH_ARM
#define XASH_ARM() 4
#else
#error "Unknown ARM architecture!"
#endif

#if defined __SOFTFP__ || __ARM_PCS_VFP == 0
#undef XASH_ARM_SOFTFP
#define XASH_ARM_SOFTFP() 1
#else  // __SOFTFP__
#undef XASH_ARM_HARDFP
#define XASH_ARM_HARDFP() 1
#endif  // __SOFTFP__
#elif defined __riscv
#undef XASH_RISCV
#define XASH_RISCV() 1

#if __riscv_xlen == 64
#undef XASH_64BIT
#define XASH_64BIT() 1
#elif __riscv_xlen != 32
#error "Unknown RISC-V ABI!"
#endif

#if defined __riscv_float_abi_soft
#undef XASH_RISCV_SOFTFP
#define XASH_RISCV_SOFTFP() 1
#elif defined __riscv_float_abi_single
#undef XASH_RISCV_SINGLEFP
#define XASH_RISCV_SINGLEFP() 1
#elif defined __riscv_float_abi_double
#undef XASH_RISCV_DOUBLEFP
#define XASH_RISCV_DOUBLEFP() 1
#else
#error "Unknown RISC-V float ABI!"
#endif
#else
#error "Unknown CPU architecture!"
#endif

#if XASH_ARM() == 8
#undef XASH_ARMv8
#define XASH_ARMv8() 1
#elif XASH_ARM() == 7
#undef XASH_ARMv7
#define XASH_ARMv7() 1
#elif XASH_ARM() == 6
#undef XASH_ARMv6
#define XASH_ARMv6() 1
#elif XASH_ARM() == 5
#undef XASH_ARMv5
#define XASH_ARMv5() 1
#elif XASH_ARM() == 4
#undef XASH_ARMv4
#define XASH_ARMv4() 1
#endif
