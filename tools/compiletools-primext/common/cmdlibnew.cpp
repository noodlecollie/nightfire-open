#include <stdarg.h>
#include <stdlib.h>
#include "CRTLib/crtlib.h"
#include "BuildPlatform/PlatformID.h"
#include "cmdlibnew.h"
#include "conprint.h"

#if XASH_WIN32()
#include WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <winbase.h>
#else
#include <time.h>
#endif

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
	static int64 g_PerformanceFrequency;
	static int64 g_ClockStart;
	int64 CurrentTime;
	struct timespec ts;

	if ( !g_PerformanceFrequency )
	{
		struct timespec res;
		if ( !clock_getres(CLOCK_MONOTONIC, &res) )
			g_PerformanceFrequency = 1000000000LL / res.tv_nsec;
	}
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
#else
#error "Implement me!"
#endif
}
