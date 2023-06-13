#include "PlatformLib/String.h"
#include <strings.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int PlatformLib_StrCaseCmp(const char* s1, const char* s2)
{
	return strcasecmp(s1, s2);
}

int PlatformLib_StrNCaseCmp(const char* s1, const char* s2, size_t n)
{
	return strncasecmp(s1, s2, n);
}

int PlatformLib_VSNPrintF(char* buffer, size_t count, const char* format, va_list argptr)
{
	const int returnVal = vsnprintf(buffer, count, format, argptr);

	if ( returnVal < 0 )
	{
		// Encoding failed, so ensure string is empty.
		if ( buffer )
		{
			buffer[0] = '\0';
		}
	}
	else if ( static_cast<size_t>(returnVal) >= count )
	{
		// String was truncated, so ensure it is terminated.
		if ( buffer && count > 0 )
		{
			buffer[count - 1] = '\0';
		}
	}

	return returnVal;
}

char* PlatformLib_StrDup(const char* in)
{
	return strdup(in);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
