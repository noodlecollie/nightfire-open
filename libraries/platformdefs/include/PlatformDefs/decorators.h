#pragma once

// NFTODO: We should remove EXPORT and GAME_EXPORT - I'm not sure what these are doing,
// but they're not used in a conventional way and I think this is part of the reason
// why messing about with -fvisibility on Linux causes crashes.

#if defined(__GNUC__)
#ifndef __i386__
#define EXPORT __attribute__((visibility("default")))
#define GAME_EXPORT
#else
#error 32-bit builds are not supported.
#endif
#define _format(x) __attribute__((format(printf, x, x + 1)))
#define NORETURN __attribute__((noreturn))
#define FORCEINLINE inline __attribute__((always_inline))
#define DELIBERATE_FALL_THROUGH __attribute__((fallthrough));
#elif defined(_MSC_VER)
#define EXPORT __declspec(dllexport)
#define GAME_EXPORT
#define _format(x)
#define NORETURN
#define FORCEINLINE __forceinline
#define DELIBERATE_FALL_THROUGH
#else
#error Unsupported compiler!
#endif

#if ( __GNUC__ >= 3 )
#define unlikely(x) __builtin_expect(x, 0)
#define likely(x) __builtin_expect(x, 1)
#elif defined(__has_builtin)
#if __has_builtin(__builtin_expect)
#define unlikely(x) __builtin_expect(x, 0)
#define likely(x) __builtin_expect(x, 1)
#else
#define unlikely(x) (x)
#define likely(x) (x)
#endif
#else
#define unlikely(x) (x)
#define likely(x) (x)
#endif

// These are not supported on non-Windows platforms.
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#endif
