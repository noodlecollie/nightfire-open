/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/
//
// cl_util.h
//
#ifndef CL_UTIL_H
#define CL_UTIL_H
#include "exportdef.h"
#include "EnginePublicAPI/cvardef.h"

#include <limits>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

// Macros to hook function calls into the HUD object

#define HOOK_MESSAGE(x) gEngfuncs.pfnHookUserMsg(#x, __MsgFunc_##x);

#define DECLARE_MESSAGE(y, x) \
	int __MsgFunc_##x(const char* pszName, int iSize, void* pbuf) \
	{ \
		return gHUD.y.MsgFunc_##x(pszName, iSize, pbuf); \
	}

#define HOOK_COMMAND(x, y) gEngfuncs.pfnAddCommand(x, __CmdFunc_##y);
#define DECLARE_COMMAND(y, x) \
	void __CmdFunc_##x(void) \
	{ \
		gHUD.y.UserCmd_##x(); \
	}

inline float CL_CvarGetFloat(const char* x)
{
	return gEngfuncs.pfnGetCvarFloat(x);
}
inline const char* CL_CvarGetString(const char* x)
{
	return gEngfuncs.pfnGetCvarString(x);
}
inline struct cvar_s* CL_CvarCreate(const char* cv, const char* val, const int flags)
{
	return gEngfuncs.pfnRegisterVariable(cv, val, flags);
}

#define SPR_Load (*gEngfuncs.pfnSPR_Load)
#define SPR_Set (*gEngfuncs.pfnSPR_Set)
#define SPR_Frames (*gEngfuncs.pfnSPR_Frames)
#define SPR_GetList (*gEngfuncs.pfnSPR_GetList)

// SPR_Draw  draws a the current sprite as solid
#define SPR_Draw (*gEngfuncs.pfnSPR_Draw)
// SPR_DrawHoles  draws the current sprites, with color index255 not drawn (transparent)
#define SPR_DrawHoles (*gEngfuncs.pfnSPR_DrawHoles)
// SPR_DrawAdditive  adds the sprites RGB values to the background  (additive transulency)
#define SPR_DrawAdditive (*gEngfuncs.pfnSPR_DrawAdditive)
// SPR_DrawCustom  draws the current sprite with all parameters supplied.
#define SPR_DrawCustom (*gEngfuncs.pfnSPR_DrawCustom)

// SPR_EnableScissor  sets a clipping rect for HUD sprites. (0,0) is the top-left hand corner of the screen.
#define SPR_EnableScissor (*gEngfuncs.pfnSPR_EnableScissor)
// SPR_DisableScissor  disables the clipping rect
#define SPR_DisableScissor (*gEngfuncs.pfnSPR_DisableScissor)
//
#define FillRGBA (*gEngfuncs.pfnFillRGBA)

// ScreenHeight returns the height of the screen, in pixels
#define ScreenHeight (gHUD.m_scrinfo.iHeight)
// ScreenWidth returns the width of the screen, in pixels
#define ScreenWidth (gHUD.m_scrinfo.iWidth)

// Use this to set any co-ords in 640x480 space
#define XRES(x) ((int)(float(x) * ((float)ScreenWidth / 640.0f) + 0.5f))
#define YRES(y) ((int)(float(y) * ((float)ScreenHeight / 480.0f) + 0.5f))

// use this to project world coordinates to screen coordinates
#define XPROJECT(x) ((1.0f + (x)) * ScreenWidth * 0.5f)
#define YPROJECT(y) ((1.0f - (y)) * ScreenHeight * 0.5f)

#define GetScreenInfo (*gEngfuncs.pfnGetScreenInfo)
#define ServerCmd (*gEngfuncs.pfnServerCmd)
#define ClientCmd (*gEngfuncs.pfnClientCmd)
#define SetCrosshair (*gEngfuncs.pfnSetCrosshair)
extern cvar_t* hud_textmode;
extern float g_hud_text_color[3];
inline void DrawSetTextColor(float r, float g, float b)
{
	if ( hud_textmode->value == 1 )
		g_hud_text_color[0] = r, g_hud_text_color[1] = g, g_hud_text_color[2] = b;
	else
		gEngfuncs.pfnDrawSetTextColor(r, g, b);
}

// Gets the height & width of a sprite,  at the specified frame
inline int SPR_Height(HSPRITE x, int f)
{
	return gEngfuncs.pfnSPR_Height(x, f);
}
inline int SPR_Width(HSPRITE x, int f)
{
	return gEngfuncs.pfnSPR_Width(x, f);
}

inline client_textmessage_t* TextMessageGet(const char* pName)
{
	return gEngfuncs.pfnTextMessageGet(pName);
}

inline int TextMessageDrawChar(int x, int y, int number, int r, int g, int b)
{
	return gEngfuncs.pfnDrawCharacter(x, y, number, r, g, b);
}

inline int DrawConsoleString(int x, int y, const char* string)
{
	if ( hud_textmode->value == 1 )
	{
		return gHUD.DrawHudString(
			x,
			y,
			std::numeric_limits<int>::max(),
			string,
			static_cast<int>(255 * g_hud_text_color[0]),
			static_cast<int>(255 * g_hud_text_color[1]),
			static_cast<int>(255 * g_hud_text_color[2]));
	}

	return gEngfuncs.pfnDrawConsoleString(x, y, string);
}

inline void GetConsoleStringSize(const char* string, int* width, int* height)
{
	if ( hud_textmode->value == 1 )
	{
		*height = 13, *width = gHUD.DrawHudStringLen(string);
	}
	else
	{
		gEngfuncs.pfnDrawConsoleStringLen(string, width, height);
	}
}

int DrawUtfString(int xpos, int ypos, int iMaxX, const char* szIt, int r, int g, int b);

inline int ConsoleStringLen(const char* string)
{
	int _width = 0, _height = 0;
	if ( hud_textmode->value == 1 )
	{
		return gHUD.DrawHudStringLen(string);
	}

	GetConsoleStringSize(string, &_width, &_height);
	return _width;
}

inline void ConsolePrint(const char* string)
{
	gEngfuncs.pfnConsolePrint(string);
}

inline void CenterPrint(const char* string)
{
	gEngfuncs.pfnCenterPrint(string);
}

// returns the players name of entity no.
#define GetPlayerInfo (*gEngfuncs.pfnGetPlayerInfo)

// sound functions
inline void PlaySound(const char* szSound, float vol)
{
	gEngfuncs.pfnPlaySoundByName(szSound, vol);
}
inline void PlaySound(int iSound, float vol)
{
	gEngfuncs.pfnPlaySoundByIndex(iSound, vol);
}

// NFTODO:
// This seems to break the Windows compile in mysterious ways if it's defined.
// Just use the standard library fabs(), people, none of this macro bullshittery.
#if !defined(WIN32) && !defined(fabs)
#define fabs(x) ((x) > 0 ? (x) : 0 - (x))
#endif

void ScaleColors(int& r, int& g, int& b, int a);

inline void UnpackRGB(int& r, int& g, int& b, unsigned long ulRGB)
{
	r = (ulRGB & 0xFF0000) >> 16;
	g = (ulRGB & 0xFF00) >> 8;
	b = ulRGB & 0xFF;
}

HSPRITE LoadSprite(const char* pszName);

bool HUD_MessageBox(const char* msg);
bool IsXashFWGS();
#endif
