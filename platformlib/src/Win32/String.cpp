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

int PlatformLib_VSScanF(const char* buffer, const char* format, va_list argptr)
{
	return vsscanf_s(buffer, format, argptr);
}

char* PlatformLib_StrDup(const char* in)
{
	return _strdup(in);
}

char* PlatformLib_StrTok(char* str, const char* delimiters)
{
	// This is not thread safe, but that's OK - we're not looking for that right now.
	static char* context = nullptr;
	return strtok_s(str, delimiters, &context);
}

const char* PlatformLib_StrError(int errornum)
{
	static char buffer[256];

	errno_t result = strerror_s(buffer, errornum);
	return result == 0 ? buffer : nullptr;
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
