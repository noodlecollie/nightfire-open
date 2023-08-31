#pragma once

#if defined(__GNUC__)
#ifdef __i386__
#define EXPORT __attribute__((visibility("default"), force_align_arg_pointer))
#define GAME_EXPORT __attribute((force_align_arg_pointer))
#else
#define EXPORT __attribute__((visibility("default")))
#define GAME_EXPORT
#endif
#define _format(x) __attribute__((format(printf, x, x + 1)))
#define NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define EXPORT __declspec(dllexport)
#define GAME_EXPORT
#define _format(x)
#define NORETURN
#else
#define EXPORT
#define GAME_EXPORT
#define _format(x)
#define NORETURN
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
