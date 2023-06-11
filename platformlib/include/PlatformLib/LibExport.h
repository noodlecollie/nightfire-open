#ifndef PLATFORMLIB_LIBEXPORT_H
#define PLATFORMLIB_LIBEXPORT_H

#ifdef __cplusplus
	#define PLATFORMLIB_EXTERNC extern "C"
#else
	#define PLATFORMLIB_EXTERNC
#endif

#ifndef IS_64BIT
	#define PLATFORMLIB_CDECL __cdecl
#else
	#define PLATFORMLIB_CDECL
#endif // IS_64BIT

// No dllimport/dllexport required here since this library is static.
#define PLATFORMLIB_PUBLIC(returnType) PLATFORMLIB_EXTERNC returnType PLATFORMLIB_CDECL

#endif // PLATFORMLIB_LIBEXPORT_H
