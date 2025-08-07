#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include "CRTLib/crtlib.h"
#include "BuildPlatform/PlatformID.h"
#include "PlatformLib/System.h"
#include "CompileTools/cmdlib.h"
#include "CompileTools/conprint.h"
#include "CompileTools/zone.h"

#if XASH_WIN32()
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <winbase.h>
#else
#include <time.h>
#endif

NORETURN void COM_Assert(const char* file, int line, const char* error, ...)
{
	static char message[8192];
	va_list argptr;

	va_start(argptr, error);
	Q_vsnprintf(message, sizeof(message), error, argptr);
	va_end(argptr);

	Msg("^1assert failed at:^7 %s:%d: %s", (file ? file : "<unknown>"), line, message);
	exit(1);
}

NORETURN void COM_FatalError(const char* error, ...)
{
	static char message[8192];
	va_list argptr;

	va_start(argptr, error);
	Q_snprintf(message, sizeof(message), error, argptr);
	va_end(argptr);

	Msg("^1Fatal Error:^7 %s", message);
	exit(1);
}

char* COM_ExpandArg(const char* path)
{
	static char full[1024];

	if ( path[0] != '/' && path[0] != '\\' && path[1] != ':' )
	{
		PlatformLib_GetCWD(full, sizeof(full));
		Q_strncat(full, path, sizeof(full));
	}
	else
	{
		Q_strncpy(full, path, sizeof(full));
	}

	return full;
}

double I_FloatTime(void)
{
#if XASH_WIN32()
	static LARGE_INTEGER g_PerformanceFrequency;
	static LARGE_INTEGER g_ClockStart;
	LARGE_INTEGER CurrentTime;

	if ( !g_PerformanceFrequency.QuadPart )
	{
		QueryPerformanceFrequency(&g_PerformanceFrequency);
		QueryPerformanceCounter(&g_ClockStart);
	}

	QueryPerformanceCounter(&CurrentTime);
	return (double)(CurrentTime.QuadPart - g_ClockStart.QuadPart) / (double)(g_PerformanceFrequency.QuadPart);
#elif XASH_POSIX()
	static int64_t g_PerformanceFrequency;
	struct timespec ts;

	if ( !g_PerformanceFrequency )
	{
		struct timespec res;
		if ( !clock_getres(CLOCK_MONOTONIC, &res) )
		{
			g_PerformanceFrequency = 1000000000LL / res.tv_nsec;
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
#else
#error "Implement me!"
#endif
}

char* copystring(const char* str)
{
	if ( !str )
	{
		return NULL;
	}

	const size_t length = Q_strlen(str) + 1;
	char* newString = (char*)Mem_Alloc(length, C_STRING);
	Q_strcpy(newString, length, str);

	return newString;
}

void freestring(char* str)
{
	Mem_Free(str, C_STRING);
}
