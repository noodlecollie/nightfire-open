#include <atomic>
#include "SigintHandler.h"

static std::atomic<bool> g_CancelFlag{false};

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch ( fdwCtrlType )
	{
		// Handle the CTRL-C signal.
		case CTRL_C_EVENT:
		{
			g_CancelFlag.store(true);
			return TRUE;
		}

		default:
		{
			return FALSE;
		}
	}
}
#else
#include <signal.h>
static void SigintHandler(int)
{
	g_CancelFlag.store(true);
}
#endif

bool SetSigintHandler()
{
#if defined(_WIN32)
	return SetConsoleCtrlHandler(CtrlHandler, TRUE);
#else
	signal(SIGINT, SigintHandler);
	return true;
#endif
}

bool SigintSignalled()
{
	return g_CancelFlag.load();
}
