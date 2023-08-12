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
int PlatformLib_VSNPrintF(char* buffer, size_t count, const char* format, va_list argptr);
int PlatformLib_VSScanF(const char* buffer, const char* format, va_list argptr);
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
