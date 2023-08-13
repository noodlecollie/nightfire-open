#ifndef PLATFORMLIB_STRING_H
#define PLATFORMLIB_STRING_H

#include <stdlib.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int PlatformLib_StrCaseCmp(const char* s1, const char* s2);
int PlatformLib_StrNCaseCmp(const char* s1, const char* s2, size_t n);
char* PlatformLib_StrCat(char* destination, size_t destSize, const char* source);
char* PlatformLib_StrNCat(char* destination, size_t destSize, const char* source, size_t num);
char* PlatformLib_StrCpy(char* destination, size_t destSize, const char* source);
char* PlatformLib_StrNCpy(char* destination, size_t destSize, const char* source, size_t num);
int PlatformLib_VSNPrintF(char* buffer, size_t count, const char* format, va_list argptr);
char* PlatformLib_StrDup(const char* in);
char* PlatformLib_StrTok(char* str, const char* delimiters);
const char* PlatformLib_StrError(int errornum);

static inline int PlatformLib_SNPrintF(char* buffer, size_t count, const char* format, ...)
{
	va_list args;

	va_start(args, format);
	const int returnVal = PlatformLib_VSNPrintF(buffer, count, format, args);
	va_end(args);

	return returnVal;
}

// TODO: sscanf and its related variants are just considered bad to use.
// Maybe we should modify the code to remove usage of these, if possible.
// For now, we don't guarantee safety for these function wrappers.
int PlatformLib_VSScanF(const char* buffer, const char* format, va_list argptr);

static inline int PlatformLib_SScanF(const char* buffer, const char* format, ...)
{
	va_list args;

	va_start(args, format);
	const int returnVal = PlatformLib_VSScanF(buffer, format, args);
	va_end(args);

	return returnVal;
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // PLATFORMLIB_STRING_H
