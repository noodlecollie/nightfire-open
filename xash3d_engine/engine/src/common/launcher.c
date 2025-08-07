/*
launcher.c - direct xash3d launcher
Copyright (C) 2015 Mittorn

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "BuildPlatform/PlatformID.h"
#include "common/common.h"
#include "PlatformLib/System.h"

#ifdef XASH_SDLMAIN
#include "SDL.h"
#endif

#if XASH_WIN32()
#include <shellapi.h>

// Enable NVIDIA High Performance Graphics while using Integrated Graphics.
__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

// Enable AMD High Performance Graphics while using Integrated Graphics.
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif

#define E_GAME "NFOPEN_GAMEDIR"  // default env dir to start from

static char szGameDir[128];  // safe place to keep gamedir
static int szArgc;
static char** szArgv;

static void Sys_ChangeGame(const char* progname)
{
	// a1ba: may never be called within engine
	// if platform supports execv() function
	Q_strncpy(szGameDir, progname, sizeof(szGameDir) - 1);
	Host_Shutdown();
	exit(Host_Main(szArgc, szArgv, szGameDir, 1, &Sys_ChangeGame));
}

static inline int Sys_Start(void)
{
	int ret;
	const char* game = PlatformLib_GetEnv(E_GAME);

	if ( !game )
	{
		game = "nfopen";
	}

	Q_strncpy(szGameDir, game, sizeof(szGameDir));

	ret = Host_Main(szArgc, szArgv, game, 0, Sys_ChangeGame);

	return ret;
}

#if !XASH_WIN32()
int main(int argc, char** argv)
{
	szArgc = argc;
	szArgv = argv;
	return Sys_Start();
}
#else
int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int nShow)
{
	LPWSTR* lpArgv;
	LPWSTR cmdLineW;
	int ret, i;

	(void)nShow;
	(void)cmdLine;
	(void)hPrevInst;
	(void)hInst;

	cmdLineW = GetCommandLineW();
	lpArgv = CommandLineToArgvW(cmdLineW, &szArgc);
	szArgv = (char**)malloc((szArgc + 1) * sizeof(char*));

	for ( i = 0; i < szArgc; ++i )
	{
		size_t size = wcslen(lpArgv[i]) + 1;

		// just in case, allocate some more memory
		szArgv[i] = (char*)malloc(size * sizeof(wchar_t));

		size_t convertedLength = 0;
		wcstombs_s(&convertedLength, szArgv[i], size * sizeof(wchar_t), lpArgv[i], size);
	}
	szArgv[szArgc] = 0;

	LocalFree(lpArgv);

	ret = Sys_Start();

	for ( ; i < szArgc; ++i )
		free(szArgv[i]);
	free(szArgv);

	return ret;
}
#endif  // XASH_WIN32()
