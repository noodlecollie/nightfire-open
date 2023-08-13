#include "PlatformLib/String.h"
#include <strings.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int PlatformLib_StrCaseCmp(const char* s1, const char* s2)
{
	// SAFESTR REPLACE
	return strcasecmp(s1, s2);
}

int PlatformLib_StrNCaseCmp(const char* s1, const char* s2, size_t n)
{
	return strncasecmp(s1, s2, n);
}

char* PlatformLib_StrCat(char* destination, size_t destSize, const char* source)
{
	if ( !destination || destSize < 1 )
	{
		return destination;
	}

	char* result = strncat(destination, source, destSize);

	// Cheap way to stay safe
	destination[destSize - 1] = '\0';

	return result;
}

char* PlatformLib_StrNCat(char* destination, size_t destSize, const char* source, size_t num)
{
	if ( !destination || destSize < 1 )
	{
		return destination;
	}

	char* result = strncat(destination, source, destSize < num ? destSize : num);

	// Cheap way to stay safe
	destination[destSize - 1] = '\0';

	return result;
}

char* PlatformLib_StrCpy(char* destination, size_t destSize, const char* source)
{
	if ( !destination || destSize < 1 )
	{
		return destination;
	}

	char* result = strncpy(destination, source, destSize);

	// Cheap way to stay safe
	destination[destSize - 1] = '\0';

	return result;
}

char* PlatformLib_StrNCpy(char* destination, size_t destSize, const char* source, size_t num)
{
	if ( !destination || destSize < 1 )
	{
		return destination;
	}

	char* result = strncpy(destination, source, destSize < num ? destSize : num);

	// Cheap way to stay safe
	destination[destSize - 1] = '\0';

	return result;
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

char* PlatformLib_StrTok(char* str, const char* delimiters)
{
	return strtok(str, delimiters);
}

const char* PlatformLib_StrError(int errornum)
{
	return strerror(errornum);
}

int PlatformLib_VSScanF(const char* buffer, const char* format, va_list argptr)
{
	return vsscanf(buffer, format, argptr);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
