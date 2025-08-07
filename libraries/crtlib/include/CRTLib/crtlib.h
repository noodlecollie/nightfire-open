/*
crtlib.h - internal stdlib
Copyright (C) 2011 Uncle Mike

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

#include <string.h>
#include <stdarg.h>
#include "BuildPlatform/PlatformID.h"
#include "BuildPlatform/Decorators.h"
#include "BuildPlatform/Typedefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

// timestamp modes
enum
{
	TIME_FULL = 0,
	TIME_DATE_ONLY,
	TIME_TIME_ONLY,
	TIME_NO_SECONDS,
	TIME_YEAR_ONLY,
	TIME_FILENAME,
};

// a1ba: not using BIT macro, so flags can be copypasted into
// exported APIs headers and will get nice warning in case of changing values
#define PFILE_IGNOREBRACKET (1 << 0)
#define PFILE_HANDLECOLON (1 << 1)
#define PFILE_TOKEN_MAX_LENGTH 1024
#define PFILE_FS_TOKEN_MAX_LENGTH 512

#define IsColorString(p) (p && *(p) == '^' && *((p) + 1) && *((p) + 1) >= '0' && *((p) + 1) <= '9')
#define ColorIndex(c) (((c) - '0') & 7)

//
// crtlib.c
//
#define Q_strlwr(in, out) Q_strnlwr(in, out, (size_t)~0)
void Q_strnlwr(const char* in, char* out, size_t size_out);
#define Q_strlen(str) ((str) ? strlen((str)) : 0)
size_t Q_colorstr(const char* string);
char Q_toupper(const char in);
char Q_tolower(const char in);
size_t Q_strncat(char* dst, const char* src, size_t siz);
size_t Q_strncpy(char* dst, const char* src, size_t siz);
qboolean Q_isdigit(const char* str);
qboolean Q_isspace(const char* str);
int Q_atoi(const char* str);
float Q_atof(const char* str);
void Q_atov(float* vec, const char* str, size_t siz);
#define Q_strchr strchr
#define Q_strrchr strrchr
qboolean Q_stricmpext(const char* pattern, const char* text);
qboolean Q_strnicmpext(const char* pattern, const char* text, size_t minimumlen);
const byte* Q_memmem(const byte* haystack, size_t haystacklen, const byte* needle, size_t needlelen);
const char* Q_timestamp(int format);
int Q_vsnprintf(char* buffer, size_t buffersize, const char* format, va_list args);
int Q_snprintf(char* buffer, size_t buffersize, const char* format, ...) _format(3);
#define Q_strpbrk strpbrk
void COM_StripColors(const char* in, char* out, size_t outBufferLength);
#define Q_memprint(val) Q_pretifymem(val, 2)
char* Q_pretifymem(float value, int digitsafterdecimal);
void Q_timestring(int seconds, char* msg, size_t size);
void COM_FileBase(const char* in, char* out, size_t outBufferSize);
const char* COM_FileExtension(const char* in);
void COM_DefaultExtension(char* path, size_t pathBufferLength, const char* extension);
void COM_ReplaceExtension(char* path, size_t pathBufferLength, const char* extension);
void COM_ExtractFilePath(const char* path, char* dest, size_t destBufferSize);
const char* COM_FileWithoutPath(const char* in);
void COM_StripExtension(char* path);
void COM_RemoveLineFeed(char* str);
void COM_FixSlashes(char* pname);
qboolean COM_PathSlashFix(char* path, size_t pathBufferLength);
// return 0 on empty or null string, 1 otherwise
#define COM_CheckString(string) ((!string || !*string) ? 0 : 1)
#define COM_CheckStringEmpty(string) ((!*string) ? 0 : 1)
char* COM_ParseFileSafe(char* data, char* token, const int size, unsigned int flags, int* len, qboolean* quoted);
#define COM_ParseFile(data, token, size) COM_ParseFileSafe(data, token, size, 0, NULL, NULL)
int matchpattern(const char* in, const char* pattern, qboolean caseinsensitive);
int matchpattern_with_separator(
	const char* in,
	const char* pattern,
	qboolean caseinsensitive,
	const char* separators,
	qboolean wildcard_least_one);

// String pointer must be valid.
qboolean COM_StringIsTerminated(const char* str, size_t maxLength);

// libc implementations
int Q_strcmp(const char* s1, const char* s2);
int Q_strncmp(const char* s1, const char* s2, size_t n);
char* Q_strstr(const char* s1, const char* s2);

// libc extensions, be careful
int Q_stricmp(const char* s1, const char* s2);
int Q_strnicmp(const char* s1, const char* s2, size_t n);
char* Q_strcpy(char* dest, size_t destSize, const char* src);
char* Q_strcat(char* dest, size_t destSize, const char* src);

char* Q_stristr(const char* s1, const char* s2);

#if defined(HAVE_STRCHRNUL)
#define Q_strchrnul strchrnul
#else
const char* Q_strchrnul(const char* s, int c);
#endif

// NFTODO: There's probably a better place to put these.

static inline void RGBA_Copy(const rgba_t in, rgba_t out)
{
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
	out[3] = in[3];
}

static inline void RGBA_Set(rgba_t rgba, byte r, byte g, byte b, byte a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}

#ifdef __cplusplus
}
#endif
