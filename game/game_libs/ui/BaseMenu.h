/*
basemenu.h - menu basic header
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

#ifndef BASEMENU_H
#define BASEMENU_H
#ifdef CS16CLIENT
#include "../public/cl_dll/IGameClientExports.h"
#endif
#include "enginecallback_menu.h"
#include "EnginePublicAPI/keydefs.h"
#include "Primitive.h"
#include "EventSystem.h"
#include "Utils.h"
#include "FontManager.h"
#include "BtnsBMPTable.h"
#include "WindowSystem.h"
#include "Image.h"
#include <functional>

#define UI_MAX_MENUDEPTH 64
#define UI_MAX_MENUITEMS 64

#define UI_PULSE_DIVISOR 75.0f

#define UI_OUTLINE_WIDTH uiStatic.outlineWidth  // outline thickness

#define UI_MAXGAMES 1024  // slots for savegame/demos
#define UI_MAX_BGMAPS 32

#define MAX_HINT_TEXT 512

// menu buttons dims
#define UI_BUTTONS_WIDTH 240
#define UI_BUTTONS_HEIGHT 40

#define UI_DESCEND "gfx/shell/down"
#define UI_ASCEND "gfx/shell/up"
#define UI_LEFTARROW "gfx/shell/larrowdefault"
#define UI_LEFTARROWFOCUS "gfx/shell/larrowflyover"
#define UI_LEFTARROWPRESSED "gfx/shell/larrowpressed"
#define UI_RIGHTARROW "gfx/shell/rarrowdefault"
#define UI_RIGHTARROWFOCUS "gfx/shell/rarrowflyover"
#define UI_RIGHTARROWPRESSED "gfx/shell/rarrowpressed"
#define UI_UPARROW "gfx/shell/uparrowd"
#define UI_UPARROWFOCUS "gfx/shell/uparrowf"
#define UI_UPARROWPRESSED "gfx/shell/uparrowp"
#define UI_DOWNARROW "gfx/shell/dnarrowd"
#define UI_DOWNARROWFOCUS "gfx/shell/dnarrowf"
#define UI_DOWNARROWPRESSED "gfx/shell/dnarrowp"

// =====================================================================
// Main menu interface

#ifdef CS16CLIENT
extern IGameClientExports* g_pClient;
#endif
extern cvar_t* ui_precache;
extern cvar_t* ui_show_window_stack;
extern cvar_t* ui_borderclip;
extern cvar_t* ui_language;

enum EUISounds
{
	SND_IN = 0,
	SND_OUT,
	SND_LAUNCH,
	SND_ROLLOVER,
	SND_GLOW,
	SND_BUZZ,
	SND_KEY,
	SND_REMOVEKEY,
	SND_MOVE,
	SND_NULL,
	SND_COUNT
};

struct uiStatic_t
{
	CWindowStack menu;
	CWindowStack client;  // separate window stack for client windows
	char bgmaps[UI_MAX_BGMAPS][80];
	int bgmapcount;

	HIMAGE hFont;  // legacy qfont

	HFont hDefaultFont;
	HFont hSmallFont;
	HFont hBigFont;
	HFont hConsoleFont;
	HFont hBoldFont;
	HFont hLightBlur;
	HFont hHeavyBlur;

	bool m_fDemosPlayed;
	bool m_fNoOldBackground;
	int m_iOldMenuDepth;

	float scaleX;
	float scaleY;
	int outlineWidth;

	int cursorX;
	int cursorY;
	int realTime;
	int firstDraw;
	float enterSound;
	int mouseInRect;
	int hideCursor;
	int framecount;  // how many frames menu visible
	int initialized;

	// btns_main.bmp stuff
	HIMAGE buttonsPics[PC_BUTTONCOUNT];

	int buttons_width;  // btns_main.bmp global width
	int buttons_height;  // one button height
	int buttons_points[3];

	Size buttons_draw_size;  // scaled image what we drawing
	int width;
	bool textInput;
	bool enableAlphaFactor;
	float alphaFactor;
	int xOffset, yOffset;

	bool needMapListUpdate;
	bool nextFrameActive;
	bool renderPicbuttonText;

	char sounds[SND_COUNT][40];

	// Annoyingly, memset() is not safe because this struct contains
	// a few non-trivial members, so we have to do this...
	void Clear()
	{
		menu.Clean();
		client.Clean();

		memset(bgmaps, 0, sizeof(bgmaps));
		bgmapcount = 0;

		hFont = 0;
		hDefaultFont = 0;
		hSmallFont = 0;
		hBigFont = 0;
		hConsoleFont = 0;
		hBoldFont = 0;
		hLightBlur = 0;
		hHeavyBlur = 0;

		m_fDemosPlayed = false;
		m_fNoOldBackground = false;
		m_iOldMenuDepth = 0;

		scaleX = 0.0f;
		scaleY = 0.0f;
		outlineWidth = 0;

		cursorX = 0;
		cursorY = 0;
		realTime = 0;
		firstDraw = 0;
		enterSound = 0.0f;
		mouseInRect = 0;
		hideCursor = 0;
		framecount = 0;
		initialized = 0;

		memset(buttonsPics, 0, sizeof(buttonsPics));

		buttons_width = 0;
		buttons_height = 0;
		memset(buttons_points, 0, sizeof(buttons_points));

		buttons_draw_size = Size {};
		width = 0;
		textInput = false;
		enableAlphaFactor = false;
		alphaFactor = 0.0f;
		xOffset = 0;
		yOffset = 0;

		needMapListUpdate = false;
		nextFrameActive = false;
		renderPicbuttonText = false;

		memset(sounds, 0, sizeof(sounds));
	}
};

extern float cursorDY;  // use for touch scroll
extern bool g_bCursorDown;
extern uiStatic_t uiStatic;

#define DLG_X ((uiStatic.width - 640) / 2 - 192)  // Dialogs are 640px in width

extern unsigned int uiColorHelp;
extern unsigned int uiPromptBgColor;
extern unsigned int uiPromptTextColor;
extern unsigned int uiPromptFocusColor;
extern unsigned int uiInputTextColor;
extern unsigned int uiInputBgColor;
extern unsigned int uiInputFgColor;
extern unsigned int uiColorWhite;
extern unsigned int uiColorDkGrey;
extern unsigned int uiColorBlack;

// TODO: Move it under namespace?

void UI_ScaleCoords(int* x, int* y, int* w, int* h);
void UI_ScaleCoords(int& x, int& y, int& w, int& h);
void UI_ScaleCoords(int& x, int& y);

bool UI_CursorInRect(int x, int y, int w, int h);
inline bool UI_CursorInRect(Point pos, Size size)
{
	return UI_CursorInRect(pos.x, pos.y, size.w, size.h);
}

// temporarily override alpha by multiplying given alpha and factor
void UI_EnableAlphaFactor(float a);
void UI_DisableAlphaFactor();

enum ETextFlags
{
	ETF_FORCECOL = (1 << 0),
	ETF_SHADOW = (1 << 1),
	ETF_NOSIZELIMIT = (1 << 2),
	ETF_ADDITIVE = (1 << 3)
};

int UI_DrawString(
	HFont font,
	int x,
	int y,
	int w,
	int h,
	const char* str,
	const unsigned int col,
	int charH,
	uint justify,
	uint flags = 0
);
inline int UI_DrawString(
	HFont font,
	Point pos,
	Size size,
	const char* str,
	const unsigned int col,
	int charH,
	uint justify,
	uint flags = 0
)
{
	return UI_DrawString(font, pos.x, pos.y, size.w, size.h, str, col, charH, justify, flags);
}

void UI_DrawPic(
	int x,
	int y,
	int w,
	int h,
	const unsigned int color,
	CImage& pic,
	const ERenderMode eRenderMode = QM_DRAWNORMAL
);
inline void
UI_DrawPic(Point pos, Size size, const unsigned int color, CImage& pic, const ERenderMode eRenderMode = QM_DRAWNORMAL)
{
	UI_DrawPic(pos.x, pos.y, size.w, size.h, color, pic, eRenderMode);
}
inline void UI_DrawPic(
	int x,
	int y,
	int w,
	int h,
	const unsigned int color,
	const char* pic,
	const ERenderMode eRenderMode = QM_DRAWNORMAL
)
{
	CImage img = pic;
	UI_DrawPic(x, y, w, h, color, img, eRenderMode);
}
inline void UI_DrawPic(
	Point pos,
	Size size,
	const unsigned int color,
	const char* pic,
	const ERenderMode eRenderMode = QM_DRAWNORMAL
)
{
	CImage img = pic;
	UI_DrawPic(pos, size, color, img, eRenderMode);
}
void UI_FillRect(int x, int y, int w, int h, const unsigned int color);
inline void UI_FillRect(Point pos, Size size, const unsigned int color)
{
	UI_FillRect(pos.x, pos.y, size.w, size.h, color);
}

void UI_DrawRectangleExt(
	int in_x,
	int in_y,
	int in_w,
	int in_h,
	const unsigned int color,
	int outlineWidth,
	int flag = QM_TOP | QM_BOTTOM | QM_LEFT | QM_RIGHT
);
inline void UI_DrawRectangle(
	int x,
	int y,
	int w,
	int h,
	const unsigned int color,
	int flag = QM_TOP | QM_BOTTOM | QM_LEFT | QM_RIGHT
)
{
	UI_DrawRectangleExt(x, y, w, h, color, uiStatic.outlineWidth, flag);
}
inline void
UI_DrawRectangle(Point pos, Size size, const unsigned int color, int flag = QM_TOP | QM_BOTTOM | QM_LEFT | QM_RIGHT)
{
	UI_DrawRectangleExt(pos.x, pos.y, size.w, size.h, color, uiStatic.outlineWidth, flag);
}
inline void UI_DrawRectangleExt(
	Point pos,
	Size size,
	const unsigned int color,
	int outlineWidth,
	int flag = QM_TOP | QM_BOTTOM | QM_LEFT | QM_RIGHT
)
{
	UI_DrawRectangleExt(pos.x, pos.y, size.w, size.h, color, outlineWidth, flag);
}

void UI_StartSound(const char* sound);
void UI_LoadBmpButtons();

int UI_CreditsActive(void);
void UI_DrawFinalCredits(void);

void UI_CloseMenu(void);

// SCR support
void UI_LoadScriptConfig(void);

class CMenuEntry
{
public:
	CMenuEntry(const char* cmd, void (*pfnPrecache)(void), void (*pfnShow)(void), void (*pfnShutdown)(void) = NULL);
	const char* m_szCommand;
	void (*m_pfnPrecache)(void);
	void (*m_pfnShow)(void);
	void (*m_pfnShutdown)(void);
	CMenuEntry* m_pNext;
};

#define ADD_MENU4(cmd, precachefunc, showfunc, shutdownfunc) \
	void showfunc(void); \
	static CMenuEntry entry_##cmd(#cmd, precachefunc, showfunc, shutdownfunc)

#define ADD_MENU3(cmd, type, showfunc) \
	static type* cmd = NULL; \
	static void cmd##_Precache(void) \
	{ \
		cmd = new type(); \
	} \
	static void cmd##_Shutdown(void) \
	{ \
		delete cmd; \
	} \
	ADD_MENU4(cmd, cmd##_Precache, showfunc, cmd##_Shutdown)

#define ADD_MENU(cmd, type, showfunc) \
	void showfunc(void); \
	ADD_MENU3(cmd, type, showfunc); \
	void showfunc(void) \
	{ \
		cmd->Show(); \
	}

#define ADD_COMMAND(cmd, showfunc) static CMenuEntry cmd(#cmd, NULL, showfunc)

// Menus
void UI_Main_Menu(void);
void UI_NewGame_Menu(void);
void UI_LoadGame_Menu(void);
void UI_SaveGame_Menu(void);
void UI_SaveLoad_Menu(void);
void UI_MultiPlayer_Menu(void);
void UI_Options_Menu(void);
void UI_InternetGames_Menu(void);
void UI_LanGame_Menu(void);
void UI_ServerBrowser_Menu(void);
void UI_PlayerSetup_Menu(void);
void UI_Controls_Menu(void);
void UI_AdvControls_Menu(void);
void UI_GameOptions_Menu(void);
void UI_CreateGame_Menu(void);
void UI_Audio_Menu(void);
void UI_Video_Menu(void);
void UI_VidOptions_Menu(void);
void UI_VidModes_Menu(void);
void UI_CustomGame_Menu(void);
void UI_Credits_Menu(void);
void UI_Touch_Menu(void);
void UI_TouchOptions_Menu(void);
void UI_TouchButtons_Menu(void);
void UI_TouchEdit_Menu(void);
void UI_FileDialog_Menu(void);
void UI_TouchButtons_GetButtonList();
void UI_GamePad_Menu(void);
void UI_Zoo_Menu(void);
void UI_Acknowledgements_Menu(void);

bool UI_AdvUserOptions_IsAvailable(void);
void UI_AdvUserOptions_Menu(void);
bool UI_AdvServerOptions_IsAvailable(void);
void UI_AdvServerOptions_Menu(void);
void UI_InputDevices_Menu(void);

void UI_OpenUpdatePage(bool engine, bool preferstore);

void UI_ModelViewer_Menu(void);

//
//-----------------------------------------------------
//
class CMenu
{
public:
	// Game information
	GAMEINFO m_gameinfo;
};

class FileDialogGlobals
{
public:
	FileDialogGlobals();
	~FileDialogGlobals();

	static FileDialogGlobals& GlobalData();

	size_t PatternCount() const;
	void ClearPatterns();
	void AddPattern(const char* pattern);
	const char* GetPattern(size_t index) const;

	void SetResultCallback(std::function<void(const char*)> resultCallback);
	void ClearResultCallback();

	// TODO: Put this somewhere else? It's very specific to browsing for textures.
	bool ResultHasPreview() const;
	void SetResultHasPreview(bool preview);

	bool HasResult() const;
	const char* GetResult() const;
	void SetResultAndCallCallback(const char* result);

private:
	struct Impl;

	Impl& GetImpl();
	const Impl& GetImpl() const;

	Impl* m_Impl = nullptr;
};

extern CMenu gMenu;

#endif  // BASEMENU_H
