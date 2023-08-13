#include "PlatformLib/String.h"
#include <strings.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

// Must be within extern "C" block as this is C code,
// and linking fails otherwise.
#include "safe_str_lib.h"

int PlatformLib_StrCaseCmp(const char* s1, const char* s2)
{
	// This does not use the safe string lib because
	// strcasecmp_s requires a buffer size for s1, which
	// is a significant departure from the existing API.
	return strcasecmp(s1, s2);
}

int PlatformLib_StrNCaseCmp(const char* s1, const char* s2, size_t n)
{
	// There is no equivanelt safe string function for this.
	return strncasecmp(s1, s2, n);
}

char* PlatformLib_StrCat(char* destination, size_t destSize, const char* source)
{
	strcat_s(destination, destSize, source);
	return destination;
}

char* PlatformLib_StrNCat(char* destination, size_t destSize, const char* source, size_t num)
{
	strncat_s(destination, destSize, source, num);
	return destination;
}

char* PlatformLib_StrCpy(char* destination, size_t destSize, const char* source)
{
	strcpy_s(destination, destSize, source);
	return destination;
}

char* PlatformLib_StrNCpy(char* destination, size_t destSize, const char* source, size_t num)
{
	strncpy_s(destination, destSize, source, num);
	return destination;
}

int PlatformLib_VSNPrintF(char* buffer, size_t count, const char* format, va_list argptr)
{
	// There is no safe string lib implementation of this, so we have to do it manually.

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

char* PlatformLib_StrTok(char* str, size_t* sizePtr, const char* delimiters, char** context)
{
	return strtok_s(str, sizePtr, delimiters, context);
}

const char* PlatformLib_StrError(int errornum)
{
	return strerror(errornum);
}

int PlatformLib_VSScanF(const char* buffer, const char* format, va_list argptr)
{
	// There is no safe string lib version of this function.
	return vsscanf(buffer, format, argptr);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
