/*
host.c - dedicated and normal host
Copyright (C) 2007 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include <stdarg.h>
#include <errno.h>
#include <string.h>

#include "BuildPlatform/PlatformID.h"

#ifdef XASH_SDL
#include <SDL.h>
#endif  // XASH_SDL

#if !XASH_WIN32()
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif  // !XASH_WIN32()

#include "EnginePublicAPI/engine_version.h"
#include "PlatformLib/System.h"
#include "PlatformLib/String.h"
#include "MathLib/mathlib.h"
#include "MathLib/utils.h"
#include "BuildPlatform/Arch.h"
#include "CRTLib/bitdefs.h"
#include "common/common.h"
#include "common/base_cmd.h"
#include "client/client.h"
#include "common/netchan.h"
#include "EngineInternalAPI/protocol.h"
#include "common/mod_local.h"
#include "client/input.h"
#include "EngineInternalAPI/enginefeatures.h"
#include "EngineInternalAPI/render_api.h"
#include "common/tests.h"
#include "common/fscallback.h"
#include "common/engine_builddefs.h"
#include "common/buildnum.h"
#include "common/engine_mempool.h"

pfnChangeGame pChangeGame = NULL;
host_parm_t host;  // host parms
sysinfo_t SI;

#if XASH_ENGINE_TESTS()
struct tests_stats_s tests_stats;
#endif  // XASH_ENGINE_TESTS()

CVAR_DEFINE(host_developer, "developer", "0", FCVAR_FILTERABLE, "engine is in development-mode");
CVAR_DEFINE_AUTO(sys_timescale, "1.0", FCVAR_CHEAT | FCVAR_FILTERABLE, "scale frame time");
CVAR_DEFINE_AUTO(sys_ticrate, "100", 0, "framerate in dedicated mode");

convar_t* host_serverstate;
convar_t* host_gameloaded;
convar_t* host_clientloaded;
convar_t* host_limitlocal;
convar_t* host_maxfps;
convar_t* host_framerate;
convar_t* host_sleeptime;
convar_t* con_gamemaps;
convar_t *build, *ver;

#if XASH_WIN32()
#define EXECUTABLE_NAME XASH_EXECUTABLE_NAME ".exe"
#else  // XASH_WIN32()
#define EXECUTABLE_NAME XASH_EXECUTABLE_NAME
#endif  // !XASH_WIN32()

void Sys_PrintUsage(void)
{
	const char* usage_str =
		""

#if XASH_MESSAGEBOX == MSGBOX_STDERR
		"\n"  // dirty hack to not have Xash Error: Usage: on same line
#endif  // XASH_MESSAGEBOX == MSGBOX_STDERR

		S_USAGE
		"\n"
		"   " EXECUTABLE_NAME
		" [options] [+command1] [+command2 arg]\n"
		"Options:\n"
		"   -dev [level]       set log verbosity 0-5\n"
		"   -log               write log to \"engine.log\"\n"
		"   -nowriteconfig     disable config save\n"

#if !XASH_WIN32()
		"   -casesensitive     disable case-insensitive FS emulation\n"
#endif  // !XASH_WIN32()

		"   -daemonize         run engine in background, dedicated only\n"

#if XASH_ENGINE_TESTS()
		"   -runtests          run engine tests\n"
#endif

#if !XASH_DEDICATED()
		"   -toconsole         run engine with console open\n"
		"   -width <n>         set window width\n"
		"   -height <n>        set window height\n"
		"   -oldfont           enable unused Quake font in Half-Life\n"
		"   -fullscreen        run engine in fullscreen mode\n"
		"   -windowed          run engine in windowed mode\n"
		"   -dedicated         run engine in dedicated server mode\n"

#if XASH_WIN32()
		"   -noavi             disable AVI support\n"
		"   -nointro           disable intro video\n"
		"   -minidumps         enable writing minidumps when game crashed\n"
#endif  // XASH_WIN32()

#if XASH_VIDEO == VIDEO_FBDEV
		"   -fbdev <path>      open selected framebuffer\n"
		"   -ttygfx            set graphics mode in tty\n"
		"   -doublebuffer      enable doublebuffering\n"
#endif  // XASH_VIDEO == VIDEO_FBDEV

#if XASH_SOUND == SOUND_ALSA
		"   -alsadev <dev>     open selected ALSA device\n"
#endif  // XASH_SOUND == SOUND_ALSA

		"   -nojoy             disable joystick support\n"

#ifdef XASH_SDL
		"   -sdl_joy_old_api   use SDL legacy joystick API\n"
		"   -sdl_renderer <n>  use alternative SDL_Renderer for software\n"
#endif  // XASH_SDL

		"   -nosound           disable sound\n"
		"   -noenginemouse     disable mouse completely\n"
		"   -ref <name>        use selected renderer dll\n"
		"   -gldebug           enable OpenGL debug log\n"
#endif  // XASH_DEDICATED()

		"   -noip              disable TCP/IP\n"
		"   -noch              disable crashhandler\n"
		"   -disablehelp       disable this message\n"
		"   -dll <path>        override server DLL path\n"

#if !XASH_DEDICATED()
		"   -clientlib <path>  override client DLL path\n"
#endif

		"   -rodir <path>      set read-only base directory, experimental\n"
		"   -bugcomp           enable precise bug compatibility. Will break games that don't require it\n"
		"                      Refer to engine documentation for more info\n"
		"   -ip <ip>           set custom ip\n"
		"   -port <port>       set custom host port\n"
		"   -clockwindow <cw>  adjust clockwindow\n";

	Sys_Error("%s", usage_str);
}

int Host_CompareFileTime(int ft1, int ft2)
{
	if ( ft1 < ft2 )
	{
		return -1;
	}
	else if ( ft1 > ft2 )
	{
		return 1;
	}
	return 0;
}

void Host_ShutdownServer(void)
{
	SV_Shutdown("Server was killed\n");
}

/*
================
Host_PrintEngineFeatures
================
*/
void Host_PrintEngineFeatures(void)
{
	if ( FBitSet(host.features, ENGINE_WRITE_LARGE_COORD) )
		Con_Reportf("^3EXT:^7 big world support enabled\n");

	if ( FBitSet(host.features, ENGINE_LOAD_DELUXEDATA) )
		Con_Reportf("^3EXT:^7 deluxemap support enabled\n");

	if ( FBitSet(host.features, ENGINE_PHYSICS_PUSHER_EXT) )
		Con_Reportf("^3EXT:^7 Improved MOVETYPE_PUSH is used\n");

	if ( FBitSet(host.features, ENGINE_LARGE_LIGHTMAPS) )
		Con_Reportf("^3EXT:^7 Large lightmaps enabled\n");

	if ( FBitSet(host.features, ENGINE_COMPENSATE_QUAKE_BUG) )
		Con_Reportf("^3EXT:^7 Compensate quake bug enabled\n");
}

/*
==============
Host_IsQuakeCompatible

==============
*/
qboolean Host_IsQuakeCompatible(void)
{
	// feature set
	if ( FBitSet(host.features, ENGINE_QUAKE_COMPATIBLE) )
		return true;

#if !XASH_DEDICATED()
	// quake demo playing
	if ( cls.demoplayback == DEMO_QUAKE1 )
		return true;
#endif  // XASH_DEDICATED()

	return false;
}

/*
================
Host_EndGame
================
*/
void Host_EndGame(qboolean abort, const char* message, ...)
{
	va_list argptr;
	static char string[MAX_SYSPATH];

	va_start(argptr, message);
	Q_vsnprintf(string, sizeof(string), message, argptr);
	va_end(argptr);

	Con_Printf("Host_EndGame: %s\n", string);

	SV_Shutdown("\n");
#if !XASH_DEDICATED()
	CL_Disconnect();

	// recreate world if needs
	CL_ClearEdicts();
#endif

	// release all models
	Mod_FreeAll();

	if ( abort )
		Host_AbortCurrentFrame();
}

/*
================
Host_AbortCurrentFrame

aborts the current host frame and goes on with the next one
================
*/
void Host_AbortCurrentFrame(void)
{
	longjmp(host.abortframe, 1);
}

/*
==================
Host_CalcSleep
==================
*/
static int Host_CalcSleep(void)
{
#if !XASH_DEDICATED()
	// never sleep in timedemo for benchmarking purposes
	// also don't sleep with vsync for less lag
	if ( CL_IsTimeDemo() || CVAR_TO_BOOL(gl_vsync) )
	{
		return 0;
	}
#endif

	if ( Host_IsDedicated() )
	{
		// let the dedicated server some sleep
		return (int)host_sleeptime->value;
	}

	switch ( host.status )
	{
		case HOST_NOFOCUS:
			if ( SV_Active() && CL_IsInGame() )
			{
				return (int)host_sleeptime->value;
			}
			// fall through
		case HOST_SLEEP:
			return 20;
		default:
			break;
	}

	return (int)host_sleeptime->value;
}

void Host_NewInstance(const char* name, const char* finalmsg)
{
	if ( !pChangeGame )
		return;

	host.change_game = true;
	Q_strncpy(host.finalmsg, finalmsg, sizeof(host.finalmsg));

	if ( !Sys_NewInstance(name) )
		pChangeGame(name);  // call from hl.exe
}

/*
=================
Host_ChangeGame_f

Change game modification
=================
*/
void Host_ChangeGame_f(void)
{
	int i;

	if ( Cmd_Argc() != 2 )
	{
		Con_Printf(S_USAGE "game <directory>\n");
		return;
	}

	// validate gamedir
	for ( i = 0; i < FI->numgames; i++ )
	{
		if ( !Q_stricmp(FI->games[i]->gamefolder, Cmd_Argv(1)) )
			break;
	}

	if ( i == FI->numgames )
	{
		Con_Printf("%s does not exist\n", Cmd_Argv(1));
	}
	else if ( !Q_stricmp(GI->gamefolder, Cmd_Argv(1)) )
	{
		Con_Printf("%s already active\n", Cmd_Argv(1));
	}
	else
	{
		char finalmsg[MAX_VA_STRING];

		Q_snprintf(finalmsg, sizeof(finalmsg), "change game to '%s'", FI->games[i]->title);
		Host_NewInstance(Cmd_Argv(1), finalmsg);
	}
}

/*
===============
Host_Exec_f
===============
*/
void Host_Exec_f(void)
{
	string cfgpath;
	byte* f;
	char* txt;
	fs_offset_t len;
	const char* arg;

	if ( Cmd_Argc() != 2 )
	{
		Con_Printf(S_USAGE "exec <filename>\n");
		return;
	}

	arg = Cmd_Argv(1);

#if !XASH_DEDICATED()
	if ( !Cmd_CurrentCommandIsPrivileged() )
	{
		const char* unprivilegedWhitelist[] = {
			NULL,
			"mapdefault.cfg",
			"scout.cfg",
			"sniper.cfg",
			"soldier.cfg",
			"demoman.cfg",
			"medic.cfg",
			"hwguy.cfg",
			"pyro.cfg",
			"spy.cfg",
			"engineer.cfg",
			"civilian.cfg"};
		size_t i;
		char temp[MAX_VA_STRING];
		qboolean allow = false;

		Q_snprintf(temp, sizeof(temp), "%s.cfg", clgame.mapname);
		unprivilegedWhitelist[0] = temp;

		for ( i = 0; i < SIZE_OF_ARRAY(unprivilegedWhitelist); i++ )
		{
			if ( !Q_strcmp(arg, unprivilegedWhitelist[i]) )
			{
				allow = true;
				break;
			}
		}

		if ( !allow )
		{
			Con_Printf("exec %s: not privileged or in whitelist\n", arg);
			return;
		}
	}
#endif  // XASH_DEDICATED()

	if ( !Q_stricmp("game.cfg", arg) )
	{
		// don't execute game.cfg in singleplayer
		if ( SV_GetMaxClients() == 1 )
			return;
	}

	Q_strncpy(cfgpath, arg, sizeof(cfgpath));
	COM_DefaultExtension(cfgpath, sizeof(cfgpath), ".cfg");  // append as default

	f = FS_LoadFile(cfgpath, &len, false);
	if ( !f )
	{
		Con_Reportf("couldn't exec %s\n", Cmd_Argv(1));
		return;
	}

	if ( !Q_stricmp("config.cfg", arg) )
		host.config_executed = true;

	// adds \n\0 at end of the file
	txt = Z_Calloc(len + 2);
	memcpy(txt, f, len);
	Q_strncat(txt, "\n", len + 2);
	Mem_Free(f);

	if ( !host.apply_game_config )
		Con_Printf("execing %s\n", arg);
	Cbuf_InsertText(txt);
	Mem_Free(txt);
}

/*
===============
Host_MemStats_f
===============
*/
void Host_MemStats_f(void)
{
	switch ( Cmd_Argc() )
	{
		case 1:
			Mem_PrintList(1 << 30);
			Mem_PrintStats();
			break;
		case 2:
			Mem_PrintList(Q_atoi(Cmd_Argv(1)) * 1024);
			Mem_PrintStats();
			break;
		default:
			Con_Printf(S_USAGE "memlist <all>\n");
			break;
	}
}

void Host_Minimize_f(void)
{
#ifdef XASH_SDL
	if ( host.hWnd )
		SDL_MinimizeWindow(host.hWnd);
#endif
}

/*
=================
Host_IsLocalGame

singleplayer game detect
=================
*/
qboolean Host_IsLocalGame(void)
{
	if ( SV_Active() )
	{
		return (SV_GetMaxClients() == 1) ? true : false;
	}
	else
	{
		return (CL_GetMaxClients() == 1) ? true : false;
	}
}

qboolean Host_IsLocalClient(void)
{
	// only the local client have the active server
	if ( CL_Initialized() && SV_Initialized() )
		return true;
	return false;
}

static qboolean Host_RegisterDecalEx(const char* decalName, const char* decalPath, size_t* count)
{
	int i = 0;

	if ( !COM_CheckString(decalName) || !COM_CheckString(decalPath) )
	{
		return false;
	}

	for ( i = 1; i < MAX_DECALS && host.draw_decals[i].name[0]; i++ )
	{
		if ( !Q_stricmp(host.draw_decals[i].name, decalName) )
		{
			return true;
		}
	}

	if ( i == MAX_DECALS )
	{
		Con_DPrintf(S_ERROR "MAX_DECALS limit exceeded (%d)\n", MAX_DECALS);
		return false;
	}

	// register new decal
	Q_strncpy(host.draw_decals[i].name, decalName, sizeof(host.draw_decals[i].name));
	Q_strncpy(host.draw_decals[i].path, decalPath, sizeof(host.draw_decals[i].path));
	*count += 1;

	return true;
}

/*
=================
Host_RegisterDecal
=================
*/
static qboolean Host_RegisterDecal(const char* name, size_t* count)
{
	char shortname[MAX_QPATH];

	if ( !COM_CheckString(name) )
	{
		return false;
	}

	COM_FileBase(name, shortname, sizeof(shortname));
	return Host_RegisterDecalEx(shortname, shortname, count);
}

static size_t RegisterOldDecalList(search_t* list)
{
	size_t count = 0;

	if ( !list )
	{
		return 0;
	}

	for ( int i = 0; i < list->numfilenames; i++ )
	{
		if ( !Host_RegisterDecal(list->filenames[i], &count) )
		{
			break;
		}
	}

	return count;
}

static size_t RegisterNewDecalList(search_t* list)
{
	size_t count = 0;

	if ( !list )
	{
		return 0;
	}

	for ( int i = 0; i < list->numfilenames; i++ )
	{
		char shortname[MAX_QPATH];
		COM_FileBase(list->filenames[i], shortname, sizeof(shortname));

		if ( !Host_RegisterDecalEx(shortname, list->filenames[i], &count) )
		{
			break;
		}
	}

	return count;
}

/*
=================
Host_InitDecals
=================
*/
void Host_InitDecals(void)
{
	search_t* t = NULL;
	size_t numOldDecals = 0;
	size_t numNewDecals = 0;

	// NOTE: only once resource without which engine can't continue work
	if ( !FS_FileExists("gfx/conchars", false) )
	{
		Sys_Error("W_LoadWadFile: couldn't load gfx.wad\n");
	}

	memset(host.draw_decals, 0, sizeof(host.draw_decals));

	// lookup all the decals in decals.wad (basedir, gamedir, falldir)
	t = FS_SearchFiles("decals.wad/*.*", true, false);
	numOldDecals = RegisterOldDecalList(t);
	if ( t )
	{
		Mem_Free(t);
	}

	t = FS_SearchFiles("textures/decals/*.*", true, false);
	numNewDecals = RegisterNewDecalList(t);
	if ( t )
	{
		Mem_Free(t);
	}

	Con_Reportf(
		"InitDecals: %zu decals (%zu from WAD, %zu from filesystem)\n",
		numOldDecals + numNewDecals,
		numOldDecals,
		numNewDecals);
}

/*
===================
Host_GetCommands

Add them exactly as if they had been typed at the console
===================
*/
void Host_GetCommands(void)
{
	char* cmd;

	for ( cmd = Sys_Input(); cmd; cmd = Sys_Input() )
	{
		Cbuf_AddText(cmd);
		Cbuf_Execute();
	}
}

/*
===================
Host_CalcFPS

compute actual FPS for various modes
===================
*/
double Host_CalcFPS(void)
{
	double fps = 0.0;

	if ( Host_IsDedicated() )
	{
		fps = sys_ticrate.value;
	}
#if !XASH_DEDICATED()
	else if ( CL_IsPlaybackDemo() || CL_IsRecordDemo() )  // NOTE: we should play demos with same fps as it was recorded
	{
		fps = CL_GetDemoFramerate();
	}
	else if ( Host_IsLocalGame() )
	{
		if ( !CVAR_TO_BOOL(gl_vsync) )
			fps = host_maxfps->value;
	}
	else
	{
		if ( !CVAR_TO_BOOL(gl_vsync) )
		{
			fps = host_maxfps->value;
			if ( fps == 0.0 )
				fps = MAX_FPS;
			fps = bound(MIN_FPS, fps, MAX_FPS);
		}
	}
#endif

	return fps;
}

/*
===================
Host_FilterTime

Returns false if the time is too short to run a frame
===================
*/
qboolean Host_FilterTime(float time)
{
	static double oldtime;
	double fps, scale = sys_timescale.value;

	host.realtime += time * scale;
	fps = Host_CalcFPS();

	// clamp the fps in multiplayer games
	if ( fps != 0.0 )
	{
		static int sleeps;
		double targetframetime;
		int sleeptime = Host_CalcSleep();

		// limit fps to withing tolerable range
		fps = bound(MIN_FPS, fps, MAX_FPS);

		if ( Host_IsDedicated() )
			targetframetime = (1.0 / (fps + 1.0));
		else
			targetframetime = (1.0 / fps);

		if ( (host.realtime - oldtime) < targetframetime * scale )
		{
			if ( sleeptime > 0 && sleeps > 0 )
			{
				Sys_Sleep(sleeptime);
				sleeps--;
			}

			return false;
		}

		if ( sleeptime > 0 && sleeps <= 0 )
		{
			if ( host.status == HOST_FRAME )
			{
				// give few sleeps this frame with small margin
				double targetsleeptime = targetframetime - host.pureframetime * 2;

				// don't sleep if we can't keep up with the framerate
				if ( targetsleeptime > 0 )
				{
					sleeps = (int)(targetsleeptime / ((double)sleeptime * 0.001));
				}
				else
				{
					sleeps = 0;
				}
			}
			else
			{
				// always sleep at least once in minimized/nofocus state
				sleeps = 1;
			}
		}
	}

	host.frametime = host.realtime - oldtime;
	host.realframetime = bound(MIN_FRAMETIME, host.frametime, MAX_FRAMETIME);
	oldtime = host.realtime;

	// NOTE: allow only in singleplayer while demos are not active
	if ( host_framerate->value > 0.0f && Host_IsLocalGame() && !CL_IsPlaybackDemo() && !CL_IsRecordDemo() )
		host.frametime = bound(MIN_FRAMETIME, host_framerate->value * scale, MAX_FRAMETIME);
	else
		host.frametime = bound(MIN_FRAMETIME, host.frametime, MAX_FRAMETIME);

	return true;
}

/*
=================
Host_Frame
=================
*/
void Host_Frame(float time)
{
	double t1, t2;

	// decide the simulation time
	if ( !Host_FilterTime(time) )
		return;

	t1 = Sys_DoubleTime();

	if ( host.framecount == 0 )
		Con_DPrintf("Time to first frame: %.3f seconds\n", t1 - host.starttime);

	Host_InputFrame();  // input frame
	Host_ClientBegin();  // begin client
	Host_GetCommands();  // dedicated in
	Host_ServerFrame();  // server frame
	Host_ClientFrame();  // client frame
	HTTP_Run();  // both server and client

	t2 = Sys_DoubleTime();

	host.pureframetime = t2 - t1;

	host.framecount++;
}

/*
=================
Host_Error
=================
*/
void GAME_EXPORT Host_Error(const char* error, ...)
{
	static char hosterror1[MAX_SYSPATH];
	static char hosterror2[MAX_SYSPATH];
	static qboolean recursive = false;
	va_list argptr;

	va_start(argptr, error);
	Q_vsnprintf(hosterror1, sizeof(hosterror1), error, argptr);
	va_end(argptr);

	CL_WriteMessageHistory();  // before Q_error call

	if ( host.framecount < 3 )
	{
		Sys_Error("Host_InitError: %s", hosterror1);
	}
	else if ( host.framecount == host.errorframe )
	{
		Sys_Error("Host_MultiError: %s", hosterror2);
	}
	else
	{
		if ( host.allow_console )
		{
			UI_SetActiveMenu(false);
			Key_SetKeyDest(key_console);
			Con_Printf("Host_Error: %s", hosterror1);
		}
		else
			MSGBOX2(hosterror1);
	}

	// host is shutting down. don't invoke infinite loop
	if ( host.status == HOST_SHUTDOWN )
		return;

	if ( recursive )
	{
		Con_Printf("Host_RecursiveError: %s", hosterror2);
		Sys_Error("%s", hosterror1);
	}

	recursive = true;
	Q_strncpy(hosterror2, hosterror1, MAX_SYSPATH);
	host.errorframe = host.framecount;  // to avoid multply calls per frame
	Q_snprintf(host.finalmsg, sizeof(host.finalmsg), "Server crashed: %s", hosterror1);

	// clearing cmd buffer to prevent execute any commands
	COM_InitHostState();
	Cbuf_Clear();

	Host_ShutdownServer();
	CL_Drop();  // drop clients

	// recreate world if needs
	CL_ClearEdicts();

	// release all models
	Mod_FreeAll();

	recursive = false;
	Host_AbortCurrentFrame();
}

void Host_Error_f(void)
{
	const char* error = Cmd_Argv(1);

	if ( !*error )
		error = "Invoked host error";
	Host_Error("%s\n", error);
}

void Sys_Error_f(void)
{
	const char* error = Cmd_Argv(1);

	if ( !*error )
		error = "Invoked sys error";
	Sys_Error("%s\n", error);
}

/*
=================
Host_Crash_f
=================
*/
static void Host_Crash_f(void)
{
	*(volatile int*)0 = 0xffffffff;
}

/*
=================
Host_Userconfigd_f
=================
*/
void Host_Userconfigd_f(void)
{
	search_t* t;
	int i;

	t = FS_SearchFiles("userconfig.d/*.cfg", true, false);

	if ( !t )
	{
		return;
	}

	for ( i = 0; i < t->numfilenames; i++ )
	{
		Cbuf_AddTextf("exec %s\n", t->filenames[i]);
	}

	Mem_Free(t);
}

#if XASH_ENGINE_TESTS()
static void Host_RunTests(int stage)
{
	switch ( stage )
	{
		case 0:  // early engine load
			memset(&tests_stats, 0, sizeof(tests_stats));
			TEST_LIST_0;
#if !XASH_DEDICATED()
			TEST_LIST_0_CLIENT;
#endif
			break;
		case 1:  // after FS load
			TEST_LIST_1;
#if !XASH_DEDICATED()
			TEST_LIST_1_CLIENT;
#endif
			Msg("Done! %d passed, %d failed\n", tests_stats.passed, tests_stats.failed);
			Sys_Quit(tests_stats.failed > 0 ? 1 : 0);
	}
}
#endif

/*
=================
Host_InitCommon
=================
*/
void Host_InitCommon(int argc, char** argv, const char* progname, qboolean bChangeGame)
{
	char dev_level[4];
	int developer = DEFAULT_DEV;
	const char* baseDir;
	char ticrate[16];
	size_t len;

	// some commands may turn engine into infinite loop,
	// e.g. xash.exe +game xash -game xash
	// so we clear all cmd_args, but leave dbg states as well
	Sys_ParseCommandLine(argc, argv);

	if ( !Sys_CheckParm("-disablehelp") )
	{
		if ( Sys_CheckParm("-help") || Sys_CheckParm("-h") || Sys_CheckParm("--help") )
		{
			Sys_PrintUsage();
		}
	}

	if ( !Sys_CheckParm("-noch") )
		Sys_SetupCrashHandler();

	host.enabledll = !Sys_CheckParm("-nodll");

	host.change_game = bChangeGame || Sys_CheckParm("-changegame");
	host.config_executed = false;
	host.status = HOST_INIT;  // initialzation started

	// init memory subsystem
	MemPool_SetMessageCallback(&Mem_MessageFunc);

	host.mempool = Mem_AllocPool("Zone Engine");

	host.allow_console = DEFAULT_ALLOWCONSOLE;

	// HACKHACK: Quake console is always allowed
	// TODO: determine if we are running QWrap more reliable
	if ( !host.allow_console && (Sys_CheckParm("-console") || !Q_stricmp(SI.exeName, "quake")) )
		host.allow_console = true;

	if ( Sys_CheckParm("-dev") )
	{
		host.allow_console = true;
		developer = DEV_NORMAL;

		if ( Sys_GetParmFromCmdLine("-dev", dev_level) )
		{
			if ( Q_isdigit(dev_level) )
				developer = bound(DEV_NONE, abs(Q_atoi(dev_level)), DEV_EXTENDED);
		}
	}

#if XASH_ENGINE_TESTS()
	if ( Sys_CheckParm("-runtests") )
	{
		host.allow_console = true;
		developer = DEV_EXTENDED;
	}
#endif

	host.con_showalways = true;

#if XASH_DEDICATED()
	host.type = HOST_DEDICATED;  // predict state
#else
	if ( Sys_CheckParm("-dedicated") || progname[0] == '#' )
	{
		host.type = HOST_DEDICATED;
	}
	else
	{
		host.type = HOST_NORMAL;
	}
#endif

	// set default gamedir
	if ( progname[0] == '#' )
		progname++;

	Q_strncpy(SI.exeName, progname, sizeof(SI.exeName));
	Q_strncpy(SI.basedirName, progname, sizeof(SI.exeName));

	if ( Host_IsDedicated() )
	{
		Sys_MergeCommandLine();

		host.allow_console = true;
	}
	else
	{
		// don't show console as default
		if ( developer <= DEV_NORMAL )
			host.con_showalways = false;
	}

	// member console allowing
	host.allow_console_init = host.allow_console;

	if ( Sys_CheckParm("-bugcomp") )
	{
		// add argument check here when we add other levels
		// of bugcompatibility
		host.bugcomp = BUGCOMP_GOLDSRC;
	}

	// timeBeginPeriod( 1 ); // a1ba: Do we need this?

	// NOTE: this message couldn't be passed into game console but it doesn't matter
	//	Con_Reportf( "Sys_LoadLibrary: Loading xash.dll - ok\n" );

	// get default screen res
	VID_InitDefaultResolution();

	// init host state machine
	COM_InitHostState();

	// init hashed commands
	BaseCmd_Init();

	// startup cmds and cvars subsystem
	Cmd_Init();
	Cvar_Init();

	// share developer level across all dlls
	Q_snprintf(dev_level, sizeof(dev_level), "%i", developer);
	Cvar_DirectSet(&host_developer, dev_level);
	Cvar_RegisterVariable(&sys_ticrate);

	if ( Sys_GetParmFromCmdLine("-sys_ticrate", ticrate) )
	{
		double fps = bound(MIN_FPS, atof(ticrate), MAX_FPS);
		Cvar_SetValue("sys_ticrate", (float)fps);
	}

	Con_Init();  // early console running to catch all the messages

#if XASH_ENGINE_TESTS()
	if ( Sys_CheckParm("-runtests") )
		Host_RunTests(0);
#endif

	Platform_Init();

	baseDir = PlatformLib_GetEnv("XASH3D_BASEDIR");

	if ( COM_CheckString(baseDir) )
	{
		Q_strncpy(host.rootdir, baseDir, sizeof(host.rootdir));
	}
	else
	{
#if ( XASH_SDL == 2 )
		char* szBasePath = SDL_GetBasePath();

		if ( !szBasePath )
		{
			Sys_Error("couldn't determine current directory: %s", SDL_GetError());
		}

		Q_strncpy(host.rootdir, szBasePath, sizeof(host.rootdir));
		SDL_free(szBasePath);
#else
		if ( !PlatformLib_GetCWD(host.rootdir, sizeof(host.rootdir)) )
		{
			Sys_Error("couldn't determine current directory: %s", PlatformLib_StrError(errno));
			host.rootdir[0] = 0;
		}
#endif
	}

#if XASH_WIN32()
	COM_FixSlashes(host.rootdir);
#endif

	len = Q_strlen(host.rootdir);

	if ( len && host.rootdir[len - 1] == '/' )
	{
		host.rootdir[len - 1] = 0;
	}

	// get readonly root. The order is: check for arg, then env.
	// if still not got it, rodir is disabled.
	host.rodir[0] = '\0';
	if ( !Sys_GetParmFromCmdLine("-rodir", host.rodir) )
	{
		const char* roDir = PlatformLib_GetEnv("XASH3D_RODIR");

		if ( COM_CheckString(roDir) )
			Q_strncpy(host.rodir, roDir, sizeof(host.rodir));
	}

#if XASH_WIN32()
	COM_FixSlashes(host.rootdir);
#endif

	len = Q_strlen(host.rodir);

	if ( len && host.rodir[len - 1] == '/' )
	{
		host.rodir[len - 1] = 0;
	}

	if ( !COM_CheckStringEmpty(host.rootdir) )
	{
		Sys_Error("Changing working directory failed (empty working directory)\n");
		return;
	}

	FS_LoadProgs();

	if ( FS_SetCurrentDirectory(host.rootdir) != 0 )
	{
		Con_Reportf("%s is working directory now\n", host.rootdir);
	}
	else
	{
		Sys_Error("Changing working directory to %s failed.\n", host.rootdir);
	}

	FS_Init();
	Sys_InitLog();

	// print bugcompatibility level here, after log was initialized
	if ( host.bugcomp == BUGCOMP_GOLDSRC )
	{
		Con_Printf("^3BUGCOMP^7: GoldSrc bug-compatibility enabled\n");
	}

	Cmd_AddCommand("exec", Host_Exec_f, "execute a script file");
	Cmd_AddCommand("memlist", Host_MemStats_f, "prints memory pool information");
	Cmd_AddRestrictedCommand("userconfigd", Host_Userconfigd_f, "execute all scripts from userconfig.d");

	Image_Init();
	Sound_Init();

#if XASH_ENGINE_TESTS()
	if ( Sys_CheckParm("-runtests") )
	{
		Host_RunTests(1);
	}
#endif

	FS_LoadGameInfo(NULL);
	Cvar_PostFSInit();

	if ( FS_FileExists(va("%s.rc", SI.basedirName), false) )
	{
		Q_strncpy(SI.rcName, SI.basedirName, sizeof(SI.rcName));  // e.g. valve.rc
	}
	else
	{
		Q_strncpy(SI.rcName, SI.exeName, sizeof(SI.rcName));  // e.g. quake.rc
	}

	Q_strncpy(host.gamefolder, GI->gamefolder, sizeof(host.gamefolder));

	Image_CheckPaletteQ1();
	Host_InitDecals();  // reload decals

	// DEPRECATED: by FWGS fork
#if 0
	if( GI->secure )
	{
		// clear all developer levels when game is protected
		Cvar_DirectSet( &host_developer, "0" );
		host.allow_console_init = false;
		host.con_showalways = false;
		host.allow_console = false;
	}
#endif
	HPAK_Init();

	IN_Init();
	Key_Init();
}

void Host_FreeCommon(void)
{
	Image_Shutdown();
	Sound_Shutdown();
	Netchan_Shutdown();
	HPAK_FlushHostQueue();
	FS_Shutdown();
}

static void Cmd_Sys_Quit(void)
{
	Sys_Quit(0);
}

/*
=================
Host_Main
=================
*/
int EXPORT Host_Main(int argc, char** argv, const char* progname, int bChangeGame, pfnChangeGame func)
{
	static double oldtime, newtime;

	host.starttime = Sys_DoubleTime();

	pChangeGame = func;  // may be NULL

	Host_InitCommon(argc, argv, progname, bChangeGame);

	// init commands and vars
	if ( host_developer.value >= DEV_EXTENDED )
	{
		Cmd_AddRestrictedCommand("sys_error", Sys_Error_f, "just throw a fatal error to test shutdown procedures");
		Cmd_AddRestrictedCommand("host_error", Host_Error_f, "just throw a host error to test shutdown procedures");
		Cmd_AddRestrictedCommand("crash", Host_Crash_f, "a way to force a bus error for development reasons");
	}

	host_serverstate = Cvar_Get("host_serverstate", "0", FCVAR_READ_ONLY, "displays current server state");
	host_maxfps = Cvar_Get("fps_max", "72", FCVAR_ARCHIVE | FCVAR_FILTERABLE, "host fps upper limit");
	host_framerate = Cvar_Get("host_framerate", "0", FCVAR_FILTERABLE, "locks frame timing to this value in seconds");
	host_sleeptime = Cvar_Get(
		"sleeptime",
		"1",
		FCVAR_ARCHIVE | FCVAR_FILTERABLE,
		"milliseconds to sleep for each frame. higher values reduce fps accuracy");
	host_gameloaded = Cvar_Get("host_gameloaded", "0", FCVAR_READ_ONLY, "inidcates a loaded game.dll");
	host_clientloaded = Cvar_Get("host_clientloaded", "0", FCVAR_READ_ONLY, "inidcates a loaded client.dll");
	host_limitlocal = Cvar_Get("host_limitlocal", "0", 0, "apply cl_cmdrate and rate to loopback connection");
	con_gamemaps = Cvar_Get("con_mapfilter", "1", FCVAR_ARCHIVE, "when true show only maps in game folder");
	Cvar_RegisterVariable(&sys_timescale);

	build = Cvar_Getf("buildnum", FCVAR_READ_ONLY, "returns a current build number", "%i", Q_buildnum_compat());
	ver = Cvar_Getf(
		"ver",
		FCVAR_READ_ONLY,
		"shows an engine version",
		"%i/%s (hw build %i)",
		PROTOCOL_VERSION,
		XASH_COMPAT_VERSION,
		Q_buildnum_compat());
	Cvar_Getf(
		"host_ver",
		FCVAR_READ_ONLY,
		"detailed info about this build",
		"%i " XASH_VERSION " %s %s %s",
		Q_buildnum(),
		BuildPlatform_PlatformString(),
		BuildPlatform_ArchitectureString(),
		BuildPlatform_CommitString());

	Mod_Init();
	NET_Init();
	NET_InitMasters();
	Netchan_Init();

	// allow to change game from the console
	if ( pChangeGame != NULL )
	{
		Cmd_AddRestrictedCommand("game", Host_ChangeGame_f, "change game");
		Cvar_Get("host_allow_changegame", "1", FCVAR_READ_ONLY, "allows to change games");
	}
	else
	{
		Cvar_Get("host_allow_changegame", "0", FCVAR_READ_ONLY, "allows to change games");
	}

	SV_Init();
	CL_Init();

	HTTP_Init();
	ID_Init();

	if ( Host_IsDedicated() )
	{
#ifdef _WIN32
		Wcon_InitConsoleCommands();
#endif

		Cmd_AddRestrictedCommand("quit", Cmd_Sys_Quit, "quit the game");
		Cmd_AddRestrictedCommand("exit", Cmd_Sys_Quit, "quit the game");
	}
	else
		Cmd_AddRestrictedCommand("minimize", Host_Minimize_f, "minimize main window to tray");

	HPAK_CheckIntegrity(CUSTOM_RES_PATH);

	host.errorframe = 0;

	// post initializations
	switch ( host.type )
	{
		case HOST_NORMAL:
#ifdef _WIN32
			Wcon_ShowConsole(false);  // hide console
#endif
			// execute startup config and cmdline
			Cbuf_AddTextf("exec %s.rc\n", SI.rcName);
			Cbuf_Execute();
			if ( !host.config_executed )
			{
				Cbuf_AddText("exec config.cfg\n");
				Cbuf_Execute();
			}
			// exec all files from userconfig.d
			Host_Userconfigd_f();
			break;
		case HOST_DEDICATED:
			// allways parse commandline in dedicated-mode
			host.stuffcmds_pending = true;
			break;
	}

	host.change_game = false;  // done
	Cmd_RemoveCommand("setgl");
	Cbuf_ExecStuffCmds();  // execute stuffcmds (commandline)
	SCR_CheckStartupVids();  // must be last

	oldtime = Sys_DoubleTime() - 0.1;

	if ( Host_IsDedicated() )
	{
		// in dedicated server input system can't set HOST_FRAME status
		// so set it here as we're finished initializing
		host.status = HOST_FRAME;

		if ( GameState->nextstate == STATE_RUNFRAME )
			Con_Printf("Type 'map <mapname>' to start game... (TAB-autocomplete works too)\n");

		// execute server.cfg after commandline
		// so we have a chance to set servercfgfile
		Cbuf_AddTextf("exec %s\n", Cvar_VariableString("servercfgfile"));
		Cbuf_Execute();
	}

	// main window message loop
	while ( !host.crashed )
	{
		newtime = Sys_DoubleTime();
		COM_Frame((float)(newtime - oldtime));
		oldtime = newtime;
	}

	// never reached
	return 0;
}

/*
=================
Host_Shutdown
=================
*/
void EXPORT Host_Shutdown(void)
{
	if ( host.shutdown_issued )
		return;
	host.shutdown_issued = true;

	if ( host.status != HOST_ERR_FATAL )
		host.status = HOST_SHUTDOWN;  // prepare host to normal shutdown
	if ( !host.change_game )
		Q_strncpy(host.finalmsg, "Server shutdown", sizeof(host.finalmsg));

#if !XASH_DEDICATED()
	if ( host.type == HOST_NORMAL )
		Host_WriteConfig();
#endif

	SV_Shutdown("Server shutdown\n");
	SV_ShutdownFilter();
	CL_Shutdown();

	Mod_Shutdown();
	NET_Shutdown();
	HTTP_Shutdown();
	Host_FreeCommon();
	Platform_Shutdown();

	// must be last, console uses this
	Mem_FreePool(&host.mempool);

	// restore filter
	Sys_RestoreCrashHandler();
	Sys_CloseLog();
}
