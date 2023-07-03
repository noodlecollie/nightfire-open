#include "PlatformLib/String.h"
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int PlatformLib_StrCaseCmp(const char* s1, const char* s2)
{
	return _stricmp(s1, s2);
}

int PlatformLib_StrNCaseCmp(const char* s1, const char* s2, size_t n)
{
	return _strnicmp(s1, s2, n);
}

// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/vsnprintf-vsnprintf-vsnprintf-l-vsnwprintf-vsnwprintf-l
// "The vsnprintf function always writes a null terminator, even if it truncates the output."
// This makes this function safe for our use.
int PlatformLib_VSNPrintF(char* buffer, size_t count, const char* format, va_list argptr)
{
	return vsnprintf(buffer, count, format, argptr);
}

char* PlatformLib_StrDup(const char* in)
{
	return _strdup(in);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
