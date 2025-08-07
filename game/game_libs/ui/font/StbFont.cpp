/*
StbFont.cpp -- stb_truetype font renderer
Copyright (C) 2017 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/
#if defined(MAINUI_USE_STB)
#include <stdarg.h>

#ifndef _WIN32
#include <stdint.h>
#include <unistd.h>
#endif

#include "FontManager.h"
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#include "StbFont.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include "Utils.h"
#include "PlatformLib/String.h"

CStbFont::CStbFont() :
	CBaseFont(),
	m_szRealFontFile(),
	m_pFontData(NULL)
{
}

CStbFont::~CStbFont()
{
	delete[] m_pFontData;
}

bool CStbFont::FindFontDataFile(const char* name, int tall, int weight, int flags, char* dataFile, int dataFileChars)
{
#if defined(__ANDROID__)
	const char *fontFileName, *fontFileNamePost = NULL;

	// Silly code to load fonts on Android.
	// TODO: Find a way to find fonts on Android
	if ( !strcmp(name, "Roboto") || !strcmp(name, "RobotoCondensed") )
	{
		fontFileName = name;
		if ( weight > 500 )
		{
			if ( flags & FONT_ITALIC )
				fontFileNamePost = "BoldItalic";
			else
				fontFileNamePost = "Bold";
		}
		else if ( weight < 400 )
		{
			if ( flags & FONT_ITALIC )
				fontFileNamePost = "LightItalic";
			else
				fontFileNamePost = "Light";
		}
		else
		{
			if ( flags & FONT_ITALIC )
				fontFileNamePost = "Italic";
			else
				fontFileNamePost = "Regular";
		}
	}
	else  // DroidSans
	{
		fontFileName = "DroidSans";
		if ( weight > 500 )
			fontFileNamePost = "Bold";
	}

	if ( fontFileNamePost )
		PlatformLib_SNPrintF(dataFile, dataFileChars, "/system/fonts/%s-%s.ttf", fontFileName, fontFileNamePost);
	else
		PlatformLib_SNPrintF(dataFile, dataFileChars, "/system/fonts/%s.ttf", fontFileName);

	if ( access(dataFile, R_OK) != 0 )
	{
		fontFileNamePost = NULL;
		// fallback to droid sans, if requested font is not droid sans
		if ( strcmp(name, "DroidSans") )
		{
			fontFileName = "DroidSans";
			if ( weight > 500 )
				fontFileNamePost = "Bold";

			if ( fontFileNamePost )
				PlatformLib_SNPrintF(dataFile, dataFileChars, "/system/fonts/%s-%s.ttf", fontFileName, fontFileNamePost);
			else
				PlatformLib_SNPrintF(dataFile, dataFileChars, "/system/fonts/%s.ttf", fontFileName);

			if ( access(dataFile, R_OK) != 0 )
			{
				return false;  // can't even fallback to droid sans
			}
		}
		else
		{
			return false;  // can't even fallback to droid sans
		}
	}

	return true;
#elif defined __linux__  // call fontconfig
	char cmd[256];
	int len = 0;
	FILE* fp;
	char* retval;

	(void)retval;

	len = PlatformLib_SNPrintF(cmd, sizeof(cmd) - len, "fc-match -f %%{file} \"%s\"", name);
	if ( flags & FONT_ITALIC )
		len += PlatformLib_SNPrintF(cmd + len, sizeof(cmd) - len, ":style=Italic");

	if ( weight > 500 )
		len += PlatformLib_SNPrintF(cmd + len, sizeof(cmd) - len, ":weight=%d", weight);

	cmd[len] = 0;

	if ( (fp = popen(cmd, "r")) == NULL )
	{
		Con_Printf("fontconfig: Error opening pipe!\n");
		return false;
	}

	retval = fgets(dataFile, dataFileChars, fp);

	if ( pclose(fp) )
	{
		Con_Printf("fontconfig: Command not found or exited with error status\n");
		return false;
	}

	// fallback with empty fontname if font not found
	if ( strlen(dataFile) < 2 )
	{
		if ( name[0] )
			return FindFontDataFile("", tall, weight, flags, dataFile, dataFileChars);
		else
			return false;
	}

	return true;
#elif defined(__APPLE__)
	const char *fontFileName, *fontFileNamePost = NULL;

	// Silly code to load fonts on OSX.
	// TODO: Find a way to find fonts on OSX
	if ( !strcmp(name, "Trebuchet MS") )
	{
		fontFileName = name;
		if ( weight > 500 )
		{
			if ( flags & FONT_ITALIC )
				fontFileNamePost = "Bold Italic";
			else
				fontFileNamePost = "Bold";
		}
		else
		{
			if ( flags & FONT_ITALIC )
				fontFileNamePost = "Italic";
		}
	}
	else  // DroidSans
	{
		fontFileName = "Tahoma";
		if ( weight > 500 )
			fontFileNamePost = "Bold";
	}

	if ( fontFileNamePost )
		PlatformLib_SNPrintF(dataFile, dataFileChars, "/Library/Fonts/%s %s.ttf", fontFileName, fontFileNamePost);
	else
		PlatformLib_SNPrintF(dataFile, dataFileChars, "/Library/Fonts/%s.ttf", fontFileName);

	if ( access(dataFile, R_OK) != 0 )
	{
		fontFileNamePost = NULL;
		// fallback to Tahoma, if requested font is not droid sans
		if ( strcmp(name, "Tahoma") )
		{
			fontFileName = "Tahoma";
			if ( weight > 500 )
				fontFileNamePost = "Bold";

			if ( fontFileNamePost )
				PlatformLib_SNPrintF(dataFile, dataFileChars, "/Library/Fonts/%s %s.ttf", fontFileName, fontFileNamePost);
			else
				PlatformLib_SNPrintF(dataFile, dataFileChars, "/Library/Fonts/%s.ttf", fontFileName);

			if ( access(dataFile, R_OK) != 0 )
			{
				return false;  // can't even fallback to Tahoma
			}
		}
		else
		{
			return false;  // can't even fallback to Tahoma
		}
	}

	return true;
#elif defined _WIN32
	if ( !strcmp(name, "Arial") )
		PlatformLib_SNPrintF(dataFile, dataFileChars, "%s\\Fonts\\arial.ttf", PlatformLib_GetEnv("WINDIR"));
	else
		PlatformLib_SNPrintF(dataFile, dataFileChars, "%s\\Fonts\\trebucbd.ttf", PlatformLib_GetEnv("WINDIR"));
	return true;
#else
	// strcpy( dataFile, "/usr/share/fonts/truetype/droid/DroidSans.ttf");
	// return true;
#error "Can't find fonts!"
#endif
}

bool CStbFont::Create(
	const char* name,
	int tall,
	int weight,
	int blur,
	float brighten,
	int outlineSize,
	int scanlineOffset,
	float scanlineScale,
	int flags)
{
	Q_strncpy(m_szName, name, sizeof(m_szName));
	m_iTall = tall;
	m_iWeight = weight;
	m_iFlags = flags;

	m_iBlur = blur;
	m_fBrighten = brighten;

	m_iOutlineSize = outlineSize;

	m_iScanlineOffset = scanlineOffset;
	m_fScanlineScale = scanlineScale;

	if ( !FindFontDataFile(name, tall, weight, flags, m_szRealFontFile, 4096) )
	{
		Con_Printf("Unable to find font named %s\n", name);
		m_szName[0] = 0;
		return false;
	}

	// EngFuncs::COM_LoadFile does not allow open files from /
	FILE* fd = fopen(m_szRealFontFile, "rb");
	if ( !fd )
	{
		Con_Printf("Unable to open font %s!\n", m_szRealFontFile);
		return false;
	}

	fseek(fd, 0, SEEK_END);
	size_t len = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	m_pFontData = new byte[len + 1];
	size_t red = fread(m_pFontData, 1, len, fd);
	fclose(fd);
	if ( red != len )
	{
		Con_Printf("Unable to read font file %s!\n", m_szRealFontFile);
		return false;
	}

	if ( !stbtt_InitFont(&m_fontInfo, m_pFontData, 0) )
	{
		Con_Printf("Unable to create font %s!\n", m_szRealFontFile);
		m_szName[0] = 0;
		return false;
	}

	// HACKHACK: for some reason size scales between ft2 and stbtt are different
	scale = stbtt_ScaleForPixelHeight(&m_fontInfo, tall + 2);
	int x0, y0, x1, y1;

	stbtt_GetFontVMetrics(&m_fontInfo, &m_iAscent, NULL, NULL);
	m_iAscent *= scale;

	stbtt_GetFontBoundingBox(&m_fontInfo, &x0, &y0, &x1, &y1);
	m_iHeight = ((y1 - y0) * scale);  // maybe wrong!
	m_iMaxCharWidth = ((x1 - x0) * scale);  // maybe wrong!

	return true;
}

void CStbFont::GetCharRGBA(int ch, Point, Size sz, unsigned char* rgba, Size& drawSize)
{
	byte *buf, *dst;
	int a, b, c;

	GetCharABCWidths(ch, a, b, c);  // speed up cache

	int bm_top = 0;
	int bm_left = 0;
	int bm_rows = 0;
	int bm_width = 0;

	buf = stbtt_GetCodepointBitmap(&m_fontInfo, scale, scale, ch, &bm_width, &bm_rows, &bm_left, &bm_top);

	// see where we should start rendering
	const int pushDown = m_iAscent + bm_top;
	const int pushLeft = bm_left;

	// set where we start copying from
	int ystart = 0;
	if ( pushDown < 0 )
		ystart = -pushDown;

	int xstart = 0;
	if ( pushLeft < 0 )
		xstart = -pushLeft;

	int yend = bm_rows;
	if ( pushDown + yend > sz.h )
		yend += sz.h - (pushDown + yend);

	int xend = bm_width;
	if ( pushLeft + xend > sz.w )
		xend += sz.w - (pushLeft + xend);

	buf = &buf[ystart * bm_width];
	dst = rgba + 4 * sz.w * (ystart + pushDown);

	// iterate through copying the generated dib into the texture
	for ( int j = ystart; j < yend; j++, dst += 4 * sz.w, buf += bm_width )
	{
		unsigned int* xdst = (unsigned int*)(dst + 4 * (m_iBlur + m_iOutlineSize));
		for ( int i = xstart; i < xend; i++, xdst++ )
		{
			if ( buf[i] > 0 )
			{
				// paint white and alpha
				*xdst = PackRGBA(0xFF, 0xFF, 0xFF, buf[i]);
			}
			else
			{
				// paint black and null alpha
				*xdst = 0;
			}
		}
	}

	drawSize.w = xend - xstart + m_iBlur * 2 + m_iOutlineSize * 2;
	drawSize.h = yend - ystart + m_iBlur * 2 + m_iOutlineSize * 2;

	ApplyBlur(sz, rgba);
	ApplyOutline(Point(xstart, ystart), sz, rgba);
	ApplyScanline(sz, rgba);
	ApplyStrikeout(sz, rgba);
}

void CStbFont::GetCharABCWidthsNoCache(int ch, int& a, int& b, int& c)
{
	int glyphId = stbtt_FindGlyphIndex(&m_fontInfo, ch);

	int x0 = 0;
	int x1 = 0;
	int width, horiBearingX, horiAdvance;

	stbtt_GetGlyphBox(&m_fontInfo, glyphId, &x0, NULL, &x1, NULL);
	stbtt_GetCodepointHMetrics(&m_fontInfo, ch, &horiAdvance, &horiBearingX);
	width = x1 - x0;

	a = horiBearingX * scale;

	// HACKHACK: stbtt does not support hinting,
	// so we add 1 pixel margin here and stbtt
	// won't look bad on too small screen resolutions
	b = width * scale + 1;

	c = (horiAdvance - horiBearingX - width) * scale;
}

bool CStbFont::HasChar(int) const
{
	return true;
}

#endif  // WIN32 && MAINUI_USE_FREETYPE
