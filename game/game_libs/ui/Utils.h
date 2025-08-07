/*
utils.h - draw helper
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

#ifndef UTILS_H
#define UTILS_H

// extern ui_enginefuncs_t g_engfuncs;
// extern ui_textfuncs_t g_textfuncs;

#include "enginecallback_menu.h"
#include "EngineInternalAPI/gameinfo.h"
#include "EnginePublicAPI/keydefs.h"
#include "FontManager.h"
#include "BMPUtils.h"
#include "miniutl.h"
#if 0
#include <tgmath.h>
#endif

#define FILE_GLOBAL static
#define DLL_GLOBAL

#define MAX_INFO_STRING 256  // engine limit

//
// How did I ever live without ASSERT?
//
#ifdef _DEBUG
void DBG_AssertFunction(bool fExpr, const char* szExpr, const char* szFile, int szLine, const char* szMessage);
#define ASSERT(f) DBG_AssertFunction(f, #f, __FILE__, __LINE__, NULL)
#define ASSERTSZ(f, sz) DBG_AssertFunction(f, #f, __FILE__, __LINE__, sz)
#else
#define ASSERT(f)
#define ASSERTSZ(f, sz)
#endif

extern ui_globalvars_t* gpGlobals;

// exports
extern int UI_VidInit(void);
extern void UI_Init(void);
extern void UI_Shutdown(void);
extern void UI_UpdateMenu(float flTime);
extern void UI_KeyEvent(int key, int down);
extern void UI_MouseMove(int x, int y);
extern void UI_SetActiveMenu(int fActive);
extern void UI_AddServerToList(netadr_t adr, const char* info);
extern void UI_GetCursorPos(int* pos_x, int* pos_y);
extern void UI_SetCursorPos(int pos_x, int pos_y);
extern void UI_ShowCursor(int show);
extern void UI_CharEvent(int key);
extern int UI_MouseInRect(void);
extern int UI_IsVisible(void);
extern int UI_CreditsActive(void);
extern void UI_FinalCredits(void);

// extended exports
void UI_MenuResetPing_f(void);
void UI_ConnectionWarning_f(void);
void UI_ShowMessageBox(const char* text);
void UI_UpdateDialog(int preferStore);
void UI_ConnectionProgress_Disconnect(void);
void UI_ConnectionProgress_Download(
	const char* pszFileName,
	const char* pszServerName,
	int iCurrent,
	int iTotal,
	const char* comment);
void UI_ConnectionProgress_DownloadEnd(void);
void UI_ConnectionProgress_Precache(void);
void UI_ConnectionProgress_Connect(const char* server);
void UI_ConnectionProgress_ChangeLevel(void);
void UI_ConnectionProgress_ParseServerInfo(const char* server);

// defined as exported to keep compatibility with old interface
extern "C" EXPORT void
AddTouchButtonToList(const char* name, const char* texture, const char* command, unsigned char* color, int flags);

#include "EnginePublicAPI/cvardef.h"

// ScreenHeight returns the height of the screen, in ppos.xels
#define ScreenHeight ((float)(gpGlobals->scrHeight))
// ScreenWidth returns the width of the screen, in ppos.xels
#define ScreenWidth ((float)(gpGlobals->scrWidth))

#define Alpha(x) (((x)&0xFF000000) >> 24)
#define Red(x) (((x)&0xFF0000) >> 16)
#define Green(x) (((x)&0xFF00) >> 8)
#define Blue(x) (((x)&0xFF) >> 0)

inline unsigned int PackRGBA(const unsigned int r, const unsigned int g, const unsigned int b, const unsigned int a)
{
	return ((a) << 24 | (r) << 16 | (g) << 8 | (b));
}

inline unsigned int PackRGB(const unsigned int r, const unsigned int g, const unsigned int b)
{
	return PackRGBA(r, g, b, 0xFF);
}

inline void UnpackRGB(int& r, int& g, int& b, const unsigned int ulRGB)
{
	r = (ulRGB & 0xFF0000) >> 16;
	g = (ulRGB & 0xFF00) >> 8;
	b = (ulRGB & 0xFF) >> 0;
}

inline void UnpackRGBA(int& r, int& g, int& b, int& a, const unsigned int ulRGBA)
{
	a = (ulRGBA & 0xFF000000) >> 24;
	UnpackRGB(r, g, b, ulRGBA);
}

inline unsigned int PackAlpha(const unsigned int ulRGB, const unsigned int ulAlpha)
{
	return (ulRGB & 0x00FFFFFF) | (ulAlpha << 24);
}

inline unsigned int UnpackAlpha(const unsigned int ulRGBA)
{
	return ((ulRGBA & 0xFF000000) >> 24);
}

inline float InterpVal(const float from, const float to, const float frac)
{
	return from + (to - from) * frac;
}

inline unsigned int InterpColor(const unsigned int from, const unsigned int to, const float frac)
{
	return PackRGBA(
		static_cast<unsigned int>(InterpVal(static_cast<float>(Red(from)), static_cast<float>(Red(to)), frac)),
		static_cast<unsigned int>(InterpVal(static_cast<float>(Green(from)), static_cast<float>(Green(to)), frac)),
		static_cast<unsigned int>(InterpVal(static_cast<float>(Blue(from)), static_cast<float>(Blue(to)), frac)),
		static_cast<unsigned int>(InterpVal(static_cast<float>(Alpha(from)), static_cast<float>(Alpha(to)), frac)));
}

int colorstricmp(const char* a, const char* b);
int colorstrcmp(const char* a, const char* b);
extern int ColorStrlen(const char* str);  // returns string length without color symbols
extern int ColorPrexfixCount(const char* str);
extern const unsigned int g_iColorTable[8];
extern void COM_FileBase(const char* in, char* out, size_t outBufferSize);  // ripped out from hlsdk 2.3
extern int UI_FadeAlpha(int starttime, int endtime);
extern const char* Info_ValueForKey(const char* s, const char* key);
extern int KEY_GetKey(const char* binding);  // ripped out from engine
extern char* StringCopy(const char* input);  // copy string into new memory
extern int COM_CompareSaves(const void** a, const void** b);
extern void Com_EscapeCommand(char* newCommand, const char* oldCommand, int len);
extern void UI_EnableTextInput(bool enable);

void UI_LoadCustomStrings(void);
const char* L(const char* szStr);  // L means Localize!
void UI_FreeCustomStrings(void);

#ifdef __APPLE__
#define register
#endif  // __APPLE__

inline size_t Q_strncpy(char* dst, const char* src, size_t size)
{
	char* d = dst;
	const char* s = src;
	size_t n = size;

	if ( !dst || !src || !size )
		return 0;

	// copy as many bytes as will fit
	if ( n != 0 && --n != 0 )
	{
		do
		{
			if ( (*d++ = *s++) == 0 )
				break;
		}
		while ( --n != 0 );
	}

	// not enough room in dst, add NULL and traverse rest of src
	if ( n == 0 )
	{
		if ( size != 0 )
			*d = '\0';  // NULL-terminate dst
		while ( *s++ )
			;
	}
	return (s - src - 1);  // count does not include NULL
}

#ifdef register
#undef register
#endif  // register

#define CS_SIZE 64  // size of one config string
#define CS_TIME 16  // size of time string

#define MAX_SCOREBOARDNAME 32  // engine and dlls allows only 32 chars

// color strings
#define ColorIndex(c) (((c) - '0') & 7)
#define IsColorString(p) (p && *(p) == '^' && *((p) + 1) && *((p) + 1) >= '0' && *((p) + 1) <= '9')

// stringize utilites
#define STR(x) #x
#define STR2(x) STR(x)

namespace UI
{

	namespace Key
	{
#ifndef K_A_BUTTON
#define K_A_BUTTON K_AUX1
#define K_B_BUTTON K_AUX2
#endif  // K_A_BUTTON

		inline bool IsEscape(int key)
		{
			switch ( key )
			{
				case K_ESCAPE:
				case K_B_BUTTON:
					return true;
			}
			return false;
		}

		inline bool IsEnter(int key)
		{
			switch ( key )
			{
				case K_ENTER:
				case K_KP_ENTER:
				case K_A_BUTTON:
					return true;
			}
			return false;
		}

		inline bool IsBackspace(int key)
		{
			switch ( key )
			{
				case K_BACKSPACE:
				case K_X_BUTTON:
					return true;
			}
			return false;
		}

		inline bool IsDelete(int key, bool ignoreBackspace = false)
		{
			if ( key == K_DEL )
				return true;
			if ( !ignoreBackspace )
				return UI::Key::IsBackspace(key);
			return false;
		}

		inline bool IsHome(int key)
		{
			switch ( key )
			{
				case K_HOME:
				case K_KP_HOME:
					return true;
			}
			return false;
		}

		inline bool IsEnd(int key)
		{
			switch ( key )
			{
				case K_HOME:
				case K_KP_HOME:
					return true;
			}
			return false;
		}

		inline bool IsLeftMouse(int key)
		{
			return key == K_MOUSE1;
		}

		inline bool IsRightMouse(int key)
		{
			return key == K_MOUSE2;
		}

		inline bool IsMouse(int key)
		{
			switch ( key )
			{
				case K_MOUSE1:
				case K_MOUSE2:
				case K_MOUSE3:
				case K_MOUSE4:
				case K_MOUSE5:
					return true;
			}
			return false;
		}

		inline bool IsUpArrow(int key)
		{
			switch ( key )
			{
				case K_UPARROW:
				case K_KP_UPARROW:
				case K_DPAD_UP:
					return true;
			}
			return false;
		}

		inline bool IsDownArrow(int key)
		{
			switch ( key )
			{
				case K_DOWNARROW:
				case K_KP_DOWNARROW:
				case K_DPAD_DOWN:
					return true;
			}
			return false;
		}

		inline bool IsLeftArrow(int key)
		{
			switch ( key )
			{
				case K_LEFTARROW:
				case K_KP_LEFTARROW:
				case K_DPAD_LEFT:
					return true;
			}
			return false;
		}

		inline bool IsRightArrow(int key)
		{
			switch ( key )
			{
				case K_RIGHTARROW:
				case K_KP_RIGHTARROW:
				case K_DPAD_RIGHT:
					return true;
			}
			return false;
		}

		inline bool IsNavigationKey(int key)
		{
			return IsUpArrow(key) || IsDownArrow(key) || IsLeftArrow(key) || IsRightArrow(key) || key == K_TAB;
		}

		inline bool IsPageUp(int key)
		{
			switch ( key )
			{
				case K_PGUP:
				case K_KP_PGUP:
				case K_L1_BUTTON:
					return true;
			}
			return false;
		}

		inline bool IsPageDown(int key)
		{
			switch ( key )
			{
				case K_PGDN:
				case K_KP_PGDN:
				case K_R1_BUTTON:
					return true;
			}
			return false;
		}

		inline bool IsConsole(int key)
		{
			switch ( key )
			{
				case '`':
				case '~':
					return true;
			}
			return false;
		}

		inline bool IsInsert(int key)
		{
			switch ( key )
			{
				case K_INS:
				case K_KP_INS:
					return true;
			}
			return false;
		}

	}  // namespace Key

	namespace Names
	{
		bool CheckIsNameValid(const char* name);
	}
}  // namespace UI
extern const int table_cp1251[64];
int Con_UtfProcessChar(int in);
int Con_UtfMoveLeft(const char* str, int pos);
int Con_UtfMoveRight(const char* str, int pos, int length);

#endif  // UTILS_H
