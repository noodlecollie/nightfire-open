/*
menu_int.h - interface between engine and menu
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#pragma once

#include <stddef.h>
#include <stdint.h>
#include "BuildPlatform/Decorators.h"
#include "BuildPlatform/Typedefs.h"
#include "EngineInternalAPI/gameinfo.h"
#include "EnginePublicAPI/netadr.h"

// a macro for mainui_cpp, indicating that mainui should be compiled for
// Xash3D 1.0 interface
#define NEW_ENGINE_INTERFACE

// Convenience define for modifications that are specific to Nightfire Open,
// to avoid ripping code out of the repo completely.
#define MENU_AFTERBURNER

#define MENU_EXTENDED_API_VERSION 1

// flags for PIC_Load
#define PIC_NEAREST (1 << 0)  // disable texfilter
#define PIC_KEEP_SOURCE (1 << 1)  // some images keep source
#define PIC_NOFLIP_TGA (1 << 2)  // Steam background completely ignore tga attribute 0x20
#define PIC_EXPAND_SOURCE (1 << 3)  // don't keep as 8-bit source, expand to RGBA

// flags for COM_ParseFileSafe
#define PFILE_IGNOREBRACKET (1 << 0)
#define PFILE_HANDLECOLON (1 << 1)

#define PLATFORM_UPDATE_PAGE "PlatformUpdatePage"
#define GENERIC_UPDATE_PAGE "GenericUpdatePage"

struct con_nprint_s;
struct wrect_s;
struct ref_viewpass_s;
struct cvar_s;
struct cl_entity_s;
struct con_nprint_s;
struct GAMEINFO_S;

typedef int HIMAGE;  // handle to a graphic

typedef struct ui_globalvars_s
{
	float time;  // unclamped host.realtime
	float frametime;

	int scrWidth;  // actual values
	int scrHeight;

	int maxClients;
	int developer;  // boolean, changed from allow_console to make mainui_cpp compile for both engines
	int demoplayback;
	int demorecording;
	char demoname[64];  // name of currently playing demo
	char maptitle[64];  // title of active map
} ui_globalvars_t;

typedef struct ui_enginefuncs_s
{
	// image handlers
	HIMAGE (*pfnPIC_Load)(const char* szPicName, const byte* ucRawImage, int ulRawImageSize, int flags);
	void (*pfnPIC_Free)(const char* szPicName);
	int (*pfnPIC_Width)(HIMAGE hPic);
	int (*pfnPIC_Height)(HIMAGE hPic);
	void (*pfnPIC_Set)(HIMAGE hPic, int r, int g, int b, int a);
	void (*pfnPIC_Draw)(int x, int y, int width, int height, const struct wrect_s* prc);
	void (*pfnPIC_DrawHoles)(int x, int y, int width, int height, const struct wrect_s* prc);
	void (*pfnPIC_DrawTrans)(int x, int y, int width, int height, const struct wrect_s* prc);
	void (*pfnPIC_DrawAdditive)(int x, int y, int width, int height, const struct wrect_s* prc);
	void (*pfnPIC_EnableScissor)(int x, int y, int width, int height);
	void (*pfnPIC_DisableScissor)(void);

	// screen handlers
	void (*pfnFillRGBA)(int x, int y, int width, int height, int r, int g, int b, int a);

	// cvar handlers
	struct cvar_s* (*pfnRegisterVariable)(const char* szName, const char* szValue, int flags);
	float (*pfnGetCvarFloat)(const char* szName);
	const char* (*pfnGetCvarString)(const char* szName);
	void (*pfnCvarSetString)(const char* szName, const char* szValue);
	void (*pfnCvarSetValue)(const char* szName, float flValue);

	// command handlers
	int (*pfnAddCommand)(const char* cmd_name, void (*function)(void));
	void (*pfnClientCmd)(int execute_now, const char* szCmdString);
	void (*pfnDelCommand)(const char* cmd_name);
	int (*pfnCmdArgc)(void);
	const char* (*pfnCmdArgv)(int argc);
	const char* (*pfnCmd_Args)(void);

	// debug messages (in-menu shows only notify)
	void (*Con_Printf)(const char* fmt, ...) _format(1);
	void (*Con_DPrintf)(const char* fmt, ...) _format(1);
	void (*Con_NPrintf)(int pos, const char* fmt, ...) _format(2);
	void (*Con_NXPrintf)(struct con_nprint_s* info, const char* fmt, ...) _format(2);

	// sound handlers
	void (*pfnPlayLocalSound)(const char* szSound);

	// cinematic handlers
	void (*pfnDrawLogo)(const char* filename, float x, float y, float width, float height);
	int (*pfnGetLogoWidth)(void);
	int (*pfnGetLogoHeight)(void);
	float (*pfnGetLogoLength)(void);  // cinematic duration in seconds

	// text message system
	void (*pfnDrawCharacter)(int x, int y, int width, int height, int ch, int ulRGBA, HIMAGE hFont);
	int (*pfnDrawConsoleString)(int x, int y, const char* string);
	void (*pfnDrawSetTextColor)(int r, int g, int b, int alpha);
	void (*pfnDrawConsoleStringLen)(const char* string, int* length, int* height);
	void (*pfnSetConsoleDefaultColor)(int r, int g, int b);  // color must came from colors.lst

	// custom rendering (for playermodel preview)
	void (*pfnSetModel)(struct cl_entity_s* ed, const char* path);
	int (*pfnGetModelSequenceCount)(struct cl_entity_s* ent);
	int (*pfnGetModelSkinCount)(struct cl_entity_s* ent);
	int (*pfnGetModelBodyVariationCount)(struct cl_entity_s* ent);
	const char* (*pfnGetModelSequenceName)(struct cl_entity_s* ent, int sequenceIndex);
	qboolean (*pfnGetModelSequenceBounds)(
		struct cl_entity_s* ent,
		int sequenceIndex,
		float* outVec3Mins,
		float* outVec3Maxs
	);
	void (*pfnGetModelEyePosition)(struct cl_entity_s* ent, float* outVec3Pos);
	float (*pfnGetModelSequenceDuration)(struct cl_entity_s* ent, int sequenceIndex);
	void (*pfnClearScene)(void);
	void (*pfnUpdateScene)(void);
	void (*pfnRenderScene)(const struct ref_viewpass_s* rvp);
	void (*pfnStoreLine)(const float* vec3Begin, const float* vec3End, uint32_t colourRGBA);
	int (*CL_CreateVisibleEntity)(int type, struct cl_entity_s* ent);

	// misc handlers
	void (*pfnHostError)(const char* szFmt, ...) _format(1);
	int (*pfnFileExists)(const char* filename, int gamedironly);
	void (*pfnGetGameDir)(char* szGetGameDir, int bufferSize);

	// gameinfo handlers
	int (*pfnCreateMapsList)(int fRefresh);
	int (*pfnClientInGame)(void);
	void (*pfnClientJoin)(const struct netadr_s adr);

	// parse txt files
	byte* (*COM_LoadFile)(const char* filename, int* pLength);
	char* (*COM_ParseFile)(char* data, char* token);
	void (*COM_FreeFile)(void* buffer);

	// keyfuncs
	void (*pfnKeyClearStates)(void);  // call when menu open or close
	void (*pfnSetKeyDest)(int dest);
	const char* (*pfnKeynumToString)(int keynum);
	const char* (*pfnKeyGetBinding)(int keynum);
	void (*pfnKeySetBinding)(int keynum, const char* binding);
	int (*pfnKeyIsDown)(int keynum);
	int (*pfnKeyGetOverstrikeMode)(void);
	void (*pfnKeySetOverstrikeMode)(int fActive);
	void* (*pfnKeyGetState)(const char* name);  // for mlook, klook etc

	// engine memory manager
	void* (*pfnMemAlloc)(size_t cb, const char* filename, const int fileline);
	void (*pfnMemFree)(void* mem, const char* filename, const int fileline);

	// collect info from engine
	int (*pfnGetGameInfo)(struct GAMEINFO_S* pgameinfo);
	struct GAMEINFO_S** (*pfnGetGamesList)(int* numGames);  // collect info about all mods
	char** (*pfnGetFilesList)(const char* pattern, int* numFiles, int gamedironly);  // find in files
	char** (*pfnGetDirectoriesList)(const char* parentDir, int* numFiles, int gamedironly);
	int (*pfnGetSaveComment)(const char* savename, char* comment);
	int (*pfnGetDemoComment)(const char* demoname, char* comment);
	int (*pfnCheckGameDll)(void);  // returns false if hl.dll is missed or invalid
	char* (*pfnGetClipboardData)(void);

	// engine launcher
	void (*pfnShellExecute)(const char* name, const char* args, int closeEngine);
	void (*pfnWriteServerConfig)(const char* name);
	void (*pfnChangeInstance)(const char* newInstance, const char* szFinalMessage);
	void (*pfnPlayBackgroundTrack)(const char* introName, const char* loopName);
	void (*pfnHostEndGame)(const char* szFinalMessage);

	// menu interface is freezed at version 0.75
	// new functions starts here
	float (*pfnRandomFloat)(float flLow, float flHigh);
	int (*pfnRandomLong)(int lLow, int lHigh);

	void (*pfnSetCursor)(void* hCursor);  // change cursor
	int (*pfnIsMapValid)(char* filename);
	void (*pfnProcessImage)(int texnum, float gamma, int topColor, int bottomColor);
	int (*pfnCompareFileTime)(const char* filename1, const char* filename2, int* iCompare);

	const char* (*pfnGetModeString)(int vid_mode);
	int (*COM_SaveFile)(const char* filename, const void* data, int len);
	int (*COM_RemoveFile)(const char* filepath);
	qboolean (*NET_MasterServersDisabled)(void);
} ui_enginefuncs_t;

typedef struct
{
	int (*pfnVidInit)(void);
	void (*pfnInit)(void);
	void (*pfnShutdown)(void);
	void (*pfnRedraw)(float flTime);
	void (*pfnKeyEvent)(int key, int down);
	void (*pfnMouseMove)(int x, int y);
	void (*pfnSetActiveMenu)(int active);
	void (*pfnAddServerToList)(struct netadr_s adr, const char* info);
	void (*pfnGetCursorPos)(int* pos_x, int* pos_y);
	void (*pfnSetCursorPos)(int pos_x, int pos_y);
	void (*pfnShowCursor)(int show);
	void (*pfnCharEvent)(int key);
	int (*pfnMouseInRect)(void);  // mouse entering\leave game window
	int (*pfnIsVisible)(void);
	int (*pfnCreditsActive)(void);  // unused
	void (*pfnFinalCredits)(void);  // show credits + game end
} UI_FUNCTIONS;

typedef struct ui_extendedfuncs_s
{
	// text functions, frozen
	void (*pfnEnableTextInput)(int enable);
	int (*pfnUtfProcessChar)(int ch);
	int (*pfnUtfMoveLeft)(char* str, int pos);
	int (*pfnUtfMoveRight)(char* str, int pos, int length);

	// new engine extended api start here
	// returns 1 if there are more in list, otherwise 0
	int (*pfnGetRenderers)(size_t num, char* shortName, size_t size1, char* readableName, size_t size2);
	double (*pfnDoubleTime)(void);
	char* (*pfnParseFile)(char* data, char* buf, const int size, unsigned int flags, int* len);

	// network address funcs
	const char* (*pfnAdrToString)(const struct netadr_s a);
	int (*pfnCompareAdr)(const void* a, const void* b);  // netadr_t
} ui_extendedfuncs_t;

// deprecated export from old engine
typedef void (*ADDTOUCHBUTTONTOLIST)(
	const char* name,
	const char* texture,
	const char* command,
	unsigned char* color,
	int flags
);

typedef struct
{
	ADDTOUCHBUTTONTOLIST pfnAddTouchButtonToList;
	void (*pfnResetPing)(void);
	void (*pfnShowConnectionWarning)(void);
	void (*pfnShowUpdateDialog)(int preferStore);
	void (*pfnShowMessageBox)(const char* text);
	void (*pfnConnectionProgress_Disconnect)(void);
	void (*pfnConnectionProgress_Download)(
		const char* pszFileName,
		const char* pszServerName,
		int iCurrent,
		int iTotal,
		const char* comment
	);
	void (*pfnConnectionProgress_DownloadEnd)(void);
	void (*pfnConnectionProgress_Precache)(void);
	void (*pfnConnectionProgress_Connect)(const char* server);  // NULL for local server
	void (*pfnConnectionProgress_ChangeLevel)(void);
	void (*pfnConnectionProgress_ParseServerInfo)(const char* server);
} UI_EXTENDED_FUNCTIONS;

typedef int (*MENUAPI)(UI_FUNCTIONS* pFunctionTable, ui_enginefuncs_t* engfuncs, ui_globalvars_t* pGlobals);

typedef int (*UIEXTENEDEDAPI)(int version, UI_EXTENDED_FUNCTIONS* pFunctionTable, ui_extendedfuncs_t* engfuncs);

// deprecated interface from old engine
typedef int (*UITEXTAPI)(ui_extendedfuncs_t* engfuncs);

#define MENU_UIGLAPI_VERSION 1

typedef struct ui_gl_functions_s
{
	void (*beginFrame)(int viewportX, int viewportY, int viewportWidth, int viewportHeight);
	void (*endFrame)(void);
	void (*clear)(uint32_t colour, int stencil);
	void (*pushMatrixTranslation)(float x, float y, float z);
	void (*popMatrix)(void);

	// Positions are expected to be 2x GL_FLOAT,
	// and colours are expected to be 4x GL_UNSIGNED_BYTE.
	void (*prepareToDrawWithoutTexture)(const void* data, int objectSize, size_t positionOffset, size_t colourOffset);
	void (*prepareToDrawWithTexture)(
		uint32_t texture,
		const void* data,
		int objectSize,
		size_t positionOffset,
		size_t colourOffset,
		size_t textureCoOrdOffset
	);

	void (*drawElements)(int numIndices, const void* indices);
	void (*setScissorEnabled)(qboolean enabled);
	void (*setScissorRegion)(int left, int bottom, int width, int height);
	void (*setStencilEnabled)(qboolean enabled);

	// Returns the previous stencil test value that was set.
	int (*enableWritingToStencilMask)(qboolean clearStencilBuffer);

	// Takes a test value to use for future stencil ops.
	void (*disableWritingToStencilMask)(int testValue);

	void (*setStencilOpReplace)(void);
	void (*setStencilOpIncrement)(void);

	HIMAGE (*loadRGBAImageFromMemory)(
		const char* name,
		int width,
		int height,
		const byte* data,
		size_t dataSize,
		int flags
	);

	void (*freeImage)(HIMAGE image);

	// If null, sets identity transform.
	void (*setTransform)(const float* mat4x4);
} ui_gl_functions;

typedef int (*UIGLAPI)(int version, const ui_gl_functions* uiToEngineFuncs);
