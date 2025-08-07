/*
crtlib.c - internal stdlib
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

#include <math.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include "CRTLib/crtlib.h"
#include "CRTLib/bitdefs.h"
#include "PlatformLib/Time.h"
#include "PlatformLib/String.h"

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

void Q_strnlwr(const char* in, char* out, size_t size_out)
{
	if ( size_out == 0 )
		return;

	while ( *in && size_out > 1 )
	{
		if ( *in >= 'A' && *in <= 'Z' )
			*out++ = *in++ + 'a' - 'A';
		else
			*out++ = *in++;
		size_out--;
	}
	*out = '\0';
}

qboolean Q_isdigit(const char* str)
{
	if ( str && *str )
	{
		while ( isdigit(*str) )
			str++;
		if ( !*str )
			return true;
	}
	return false;
}

qboolean Q_isspace(const char* str)
{
	if ( str && *str )
	{
		while ( isspace(*str) )
			str++;
		if ( !*str )
			return true;
	}
	return false;
}

size_t Q_colorstr(const char* string)
{
	size_t len;
	const char* p;

	if ( !string )
		return 0;

	len = 0;
	p = string;
	while ( *p )
	{
		if ( IsColorString(p) )
		{
			len += 2;
			p += 2;
			continue;
		}
		p++;
	}

	return len;
}

char Q_toupper(const char in)
{
	char out;

	if ( in >= 'a' && in <= 'z' )
		out = in + 'A' - 'a';
	else
		out = in;

	return out;
}

char Q_tolower(char in)
{
	if ( in >= 'A' && in <= 'Z' )
	{
		return in + 'a' - 'A';
	}
	else
	{
		return in;
	}
}

size_t Q_strncat(char* dst, const char* src, size_t size)
{
	register char* d = dst;
	register const char* s = src;
	register size_t n = size;
	size_t dlen;

	if ( !dst || !src || !size )
		return 0;

	// find the end of dst and adjust bytes left but don't go past end
	while ( n-- != 0 && *d != '\0' )
		d++;
	dlen = d - dst;
	n = size - dlen;

	if ( n == 0 )
		return (dlen + Q_strlen(s));

	while ( *s != '\0' )
	{
		if ( n != 1 )
		{
			*d++ = *s;
			n--;
		}
		s++;
	}

	*d = '\0';
	return (dlen + (s - src));  // count does not include NULL
}

size_t Q_strncpy(char* dst, const char* src, size_t size)
{
	register char* d = dst;
	register const char* s = src;
	register size_t n = size;

	if ( !dst || !size )
	{
		return 0;
	}

	if ( !src )
	{
		*dst = '\0';
		return 0;
	}

	// copy as many bytes as will fit
	if ( n != 0 && --n != 0 )
	{
		do
		{
			if ( (*d++ = *s++) == 0 )
			{
				break;
			}
		}
		while ( --n != 0 );
	}

	// not enough room in dst, add NULL and traverse rest of src
	if ( n == 0 )
	{
		if ( size != 0 )
		{
			*d = '\0';  // NULL-terminate dst
		}

		while ( *s++ )
		{
			// Seek to end
		}
	}

	return (s - src - 1);  // count does not include NULL
}

int Q_atoi(const char* str)
{
	int val = 0;
	int c, sign;

	if ( !str )
		return 0;

	// check for empty charachters in string
	while ( str && *str == ' ' )
		str++;

	if ( !str )
		return 0;

	if ( *str == '-' )
	{
		sign = -1;
		str++;
	}
	else
		sign = 1;

	// check for hex
	if ( str[0] == '0' && (str[1] == 'x' || str[1] == 'X') )
	{
		str += 2;
		while ( 1 )
		{
			c = *str++;
			if ( c >= '0' && c <= '9' )
				val = (val << 4) + c - '0';
			else if ( c >= 'a' && c <= 'f' )
				val = (val << 4) + c - 'a' + 10;
			else if ( c >= 'A' && c <= 'F' )
				val = (val << 4) + c - 'A' + 10;
			else
				return val * sign;
		}
	}

	// check for character
	if ( str[0] == '\'' )
		return sign * str[1];

	// assume decimal
	while ( 1 )
	{
		c = *str++;
		if ( c < '0' || c > '9' )
			return val * sign;
		val = val * 10 + c - '0';
	}
	return 0;
}

float Q_atof(const char* str)
{
	float val = 0;
	int c, sign, decimal, total;

	if ( !str )
		return 0.0f;

	// check for empty charachters in string
	while ( str && *str == ' ' )
		str++;

	if ( !str )
		return 0.0f;

	if ( *str == '-' )
	{
		sign = -1;
		str++;
	}
	else
		sign = 1;

	// check for hex
	if ( str[0] == '0' && (str[1] == 'x' || str[1] == 'X') )
	{
		str += 2;
		while ( 1 )
		{
			c = *str++;
			if ( c >= '0' && c <= '9' )
			{
				val = (val * 16) + c - '0';
			}
			else if ( c >= 'a' && c <= 'f' )
			{
				val = (val * 16) + c - 'a' + 10;
			}
			else if ( c >= 'A' && c <= 'F' )
			{
				val = (val * 16) + c - 'A' + 10;
			}
			else
			{
				return val * sign;
			}
		}
	}

	// check for character
	if ( str[0] == '\'' )
	{
		return (float)(sign * str[1]);
	}

	// assume decimal
	decimal = -1;
	total = 0;

	while ( 1 )
	{
		c = *str++;
		if ( c == '.' )
		{
			decimal = total;
			continue;
		}

		if ( c < '0' || c > '9' )
			break;
		val = val * 10 + c - '0';
		total++;
	}

	if ( decimal == -1 )
		return val * sign;

	while ( total > decimal )
	{
		val /= 10;
		total--;
	}

	return val * sign;
}

void Q_atov(float* vec, const char* str, size_t siz)
{
	string buffer;
	char *pstr, *pfront;
	size_t j;

	Q_strncpy(buffer, str, sizeof(buffer));
	memset(vec, 0, sizeof(vec_t) * siz);
	pstr = pfront = buffer;

	for ( j = 0; j < siz; j++ )
	{
		vec[j] = Q_atof(pfront);

		// valid separator is space
		while ( *pstr && *pstr != ' ' )
		{
			pstr++;
		}

		if ( !*pstr )
		{
			break;
		}

		pstr++;
		pfront = pstr;
	}
}

static qboolean Q_starcmp(const char* pattern, const char* text)
{
	char c, c1;
	const char *p = pattern, *t = text;

	while ( (c = *p++) == '?' || c == '*' )
	{
		if ( c == '?' && *t++ == '\0' )
			return false;
	}

	if ( c == '\0' )
		return true;

	for ( c1 = ((c == '\\') ? *p : c);; )
	{
		if ( Q_tolower(*t) == c1 && Q_stricmpext(p - 1, t) )
			return true;
		if ( *t++ == '\0' )
			return false;
	}
}

qboolean Q_strnicmpext(const char* pattern, const char* text, size_t minimumlength)
{
	size_t i = 0;
	char c;

	while ( (c = *pattern++) != '\0' )
	{
		i++;

		switch ( c )
		{
			case '?':
				if ( *text++ == '\0' )
					return false;
				break;
			case '\\':
				if ( Q_tolower(*pattern++) != Q_tolower(*text++) )
					return false;
				break;
			case '*':
				return Q_starcmp(pattern, text);
			default:
				if ( Q_tolower(c) != Q_tolower(*text++) )
					return false;
		}
	}
	return (*text == '\0') || i == minimumlength;
}

qboolean Q_stricmpext(const char* pattern, const char* text)
{
	return Q_strnicmpext(pattern, text, ~((size_t)0));
}

const byte* Q_memmem(const byte* haystack, size_t haystacklen, const byte* needle, size_t needlelen)
{
	const byte* cursor;

	if ( !haystack || haystacklen < 1 || !needle || needlelen < 1 || needlelen > haystacklen )
	{
		return NULL;
	}

	for ( cursor = haystack; cursor < haystack + haystacklen; ++cursor )
	{
		const size_t remaining = haystacklen - (cursor - haystack);

		// If there is not enough space to find the needle, we can exit.
		if ( remaining < needlelen )
		{
			return NULL;
		}

		// Find the first needle character from where we currently are.
		cursor = memchr(cursor, needle[0], remaining);

		if ( !cursor )
		{
			// We reached the end without encountering anything that could have been the needle.
			return NULL;
		}

		// We found the first character - see if the rest matches.
		if ( memcmp(cursor, needle, needlelen) == 0 )
		{
			// We found the entire needle.
			return cursor;
		}

		// The entire needle did not match, so go round again.
		// This will advance the cursor by one.
	}

	return NULL;
}

const char* Q_timestamp(int format)
{
	static string timestamp;
	time_t crt_time;
	const struct tm* crt_tm;
	string timestring;

	time(&crt_time);
	crt_tm = PlatformLib_LocalTime(&crt_time);

	switch ( format )
	{
		case TIME_FULL:
			// Build the full timestamp (ex: "Apr03 2007 [23:31.55]");
			strftime(timestring, sizeof(timestring), "%b%d %Y [%H:%M.%S]", crt_tm);
			break;
		case TIME_DATE_ONLY:
			// Build the date stamp only (ex: "Apr03 2007");
			strftime(timestring, sizeof(timestring), "%b%d %Y", crt_tm);
			break;
		case TIME_TIME_ONLY:
			// Build the time stamp only (ex: "23:31.55");
			strftime(timestring, sizeof(timestring), "%H:%M.%S", crt_tm);
			break;
		case TIME_NO_SECONDS:
			// Build the time stamp exclude seconds (ex: "13:46");
			strftime(timestring, sizeof(timestring), "%H:%M", crt_tm);
			break;
		case TIME_YEAR_ONLY:
			// Build the date stamp year only (ex: "2006");
			strftime(timestring, sizeof(timestring), "%Y", crt_tm);
			break;
		case TIME_FILENAME:
			// Build a timestamp that can use for filename (ex: "Nov2006-26 (19.14.28)");
			strftime(timestring, sizeof(timestring), "%b%Y-%d_%H.%M.%S", crt_tm);
			break;
		default:
			return NULL;
	}

	Q_strncpy(timestamp, timestring, sizeof(timestamp));

	return timestamp;
}

char* Q_stristr(const char* string, const char* string2)
{
	int c;
	size_t len;

	if ( !string || !string2 )
		return NULL;

	c = Q_tolower(*string2);
	len = Q_strlen(string2);

	while ( string )
	{
		for ( ; *string && Q_tolower(*string) != c; string++ )
		{
		}

		if ( *string )
		{
			if ( !Q_strnicmp(string, string2, len) )
			{
				break;
			}

			string++;
		}
		else
		{
			return NULL;
		}
	}
	return (char*)string;
}

int Q_vsnprintf(char* buffer, size_t buffersize, const char* format, va_list args)
{
	int result;

#ifndef _MSC_VER
	result = PlatformLib_VSNPrintF(buffer, buffersize, format, args);
#else
	__try
	{
		result = PlatformLib_VSNPrintF(buffer, buffersize, format, args);
	}

	// to prevent crash while output
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		Q_strncpy(buffer, "^1vsnprintf threw exception^7\n", buffersize);
		result = (int)buffersize;
	}
#endif

	if ( result < 0 )
	{
		if ( buffer && buffersize > 0 )
		{
			buffer[0] = '\0';
		}
	}
	else if ( (size_t)result >= buffersize )
	{
		buffer[buffersize - 1] = '\0';
		return -1;
	}

	return result;
}

int Q_snprintf(char* buffer, size_t buffersize, const char* format, ...)
{
	va_list args;
	int result;

	va_start(args, format);
	result = Q_vsnprintf(buffer, buffersize, format, args);
	va_end(args);

	return result;
}

void COM_StripColors(const char* in, char* out, size_t outBufferLength)
{
	char* lastOutChar = out + outBufferLength - 1;

	while ( *in && out < lastOutChar )
	{
		if ( IsColorString(in) )
		{
			in += 2;
		}
		else
		{
			*out++ = *in++;
		}
	}

	*out = '\0';
}

char* Q_pretifymem(float value, int digitsafterdecimal)
{
	static char output[8][32];
	static int current;
	float onekb = 1024.0f;
	float onemb = onekb * onekb;
	char suffix[8];
	char* out = output[current];
	char val[32], *i, *o, *dot;
	int pos;

	current = (current + 1) & (8 - 1);

	// first figure out which bin to use
	if ( value > onemb )
	{
		value /= onemb;
		Q_strcpy(suffix, sizeof(suffix), " Mb");
	}
	else if ( value > onekb )
	{
		value /= onekb;
		Q_strcpy(suffix, sizeof(suffix), " Kb");
	}
	else
		Q_strcpy(suffix, sizeof(suffix), " bytes");

	// clamp to >= 0
	if ( digitsafterdecimal < 0 )
	{
		digitsafterdecimal = 0;
	}

	// if it's basically integral, don't do any decimals
	if ( fabs(value - (int)value) < 0.00001f )
	{
		Q_snprintf(val, sizeof(val), "%i%s", (int)value, suffix);
	}
	else
	{
		char fmt[32];

		// otherwise, create a format string for the decimals
		Q_snprintf(fmt, sizeof(fmt), "%%.%if%s", digitsafterdecimal, suffix);
		Q_snprintf(val, sizeof(val), fmt, (double)value);
	}

	// copy from in to out
	i = val;
	o = out;

	// search for decimal or if it was integral, find the space after the raw number
	dot = Q_strchr(i, '.');
	if ( !dot )
		dot = Q_strchr(i, ' ');

	pos = (int)(dot - i);  // compute position of dot
	pos -= 3;  // don't put a comma if it's <= 3 long

	while ( *i )
	{
		// if pos is still valid then insert a comma every third digit, except if we would be
		// putting one in the first spot
		if ( pos >= 0 && !(pos % 3) )
		{
			// never in first spot
			if ( o != out )
				*o++ = ',';
		}

		pos--;  // count down comma position
		*o++ = *i++;  // copy rest of data as normal
	}
	*o = 0;  // terminate

	return out;
}

void Q_timestring(int seconds, char* msg, size_t size)
{
	int nMin = seconds / 60;
	int nSec = seconds - nMin * 60;
	int nHour = nMin / 60;
	const char* ext[2] = {"", "s"};

	nMin -= nHour * 60;

	if ( nHour > 0 )
	{
		Q_snprintf(
			msg,
			size,
			"%d hour%s, %d minute%s, %d second%s",
			nHour,
			ext[nHour != 1],
			nMin,
			ext[nMin != 1],
			nSec,
			ext[nSec != 1]);
	}
	else if ( nMin > 0 )
	{
		Q_snprintf(msg, size, "%d minute%s, %d second%s", nMin, ext[nMin != 1], nSec, ext[nSec != 1]);
	}
	else
	{
		Q_snprintf(msg, size, "%d second%s", nSec, ext[nSec != 1]);
	}
}

/*
============
COM_FileBase

Extracts the base name of a file (no path, no extension, assumes '/' as path separator)
============
*/
void COM_FileBase(const char* in, char* out, size_t outBufferSize)
{
	int len;
	int start;
	int end;

	len = (int)Q_strlen(in);

	if ( len < 1 )
	{
		return;
	}

	// scan backward for '.'
	end = len - 1;

	while ( end && in[end] != '.' && in[end] != '/' && in[end] != '\\' )
	{
		end--;
	}

	if ( in[end] != '.' )
	{
		end = len - 1;  // no '.', copy to end
	}
	else
	{
		end--;  // found ',', copy to left of '.'
	}

	// scan backward for '/'
	start = len - 1;

	while ( start >= 0 && in[start] != '/' && in[start] != '\\' )
	{
		start--;
	}

	if ( start < 0 || (in[start] != '/' && in[start] != '\\') )
	{
		start = 0;
	}
	else
	{
		start++;
	}

	// length of new sting
	len = end - start + 1;

	// Copy partial string
	PlatformLib_StrNCpy(out, outBufferSize, &in[start], len);
}

/*
============
COM_FileExtension
============
*/
const char* COM_FileExtension(const char* in)
{
	const char *separator, *backslash, *colon, *dot;

	separator = Q_strrchr(in, '/');
	backslash = Q_strrchr(in, '\\');

	if ( !separator || separator < backslash )
		separator = backslash;

	colon = Q_strrchr(in, ':');

	if ( !separator || separator < colon )
		separator = colon;

	dot = Q_strrchr(in, '.');

	if ( dot == NULL || (separator && (dot < separator)) )
		return "";

	return dot + 1;
}

/*
============
COM_FileWithoutPath
============
*/
const char* COM_FileWithoutPath(const char* in)
{
	const char *separator, *backslash, *colon;

	separator = Q_strrchr(in, '/');
	backslash = Q_strrchr(in, '\\');

	if ( !separator || separator < backslash )
		separator = backslash;

	colon = Q_strrchr(in, ':');

	if ( !separator || separator < colon )
		separator = colon;

	return separator ? separator + 1 : in;
}

/*
============
COM_ExtractFilePath
============
*/
void COM_ExtractFilePath(const char* path, char* dest, size_t destBufferSize)
{
	const char* src;

	if ( !dest || destBufferSize < 1 )
	{
		return;
	}

	src = path + Q_strlen(path) - 1;

	// back up until a \ or the start
	while ( src != path && !(*(src - 1) == '\\' || *(src - 1) == '/') )
	{
		src--;
	}

	if ( src != path )
	{
		PlatformLib_StrNCpy(dest, destBufferSize, path, src - path);
		dest[src - path - 1] = 0;  // cutoff backslash
	}
	else
	{
		dest[0] = 0;  // file without path
	}
}

/*
============
COM_StripExtension
============
*/
void COM_StripExtension(char* path)
{
	size_t length = Q_strlen(path);

	if ( length > 0 )
	{
		length--;
	}

	while ( length > 0 && path[length] != '.' )
	{
		length--;

		if ( path[length] == '/' || path[length] == '\\' || path[length] == ':' )
		{
			return;  // no extension
		}
	}

	if ( length )
	{
		path[length] = 0;
	}
}

/*
==================
COM_DefaultExtension
==================
*/
void COM_DefaultExtension(char* path, size_t pathBufferLength, const char* extension)
{
	const char* src;
	size_t len;

	// if path doesn't have a .EXT, append extension
	// (extension should include the .)
	len = Q_strlen(path);
	src = path + len - 1;

	while ( *src != '/' && src != path )
	{
		// it has an extension
		if ( *src == '.' )
		{
			return;
		}

		src--;
	}

	PlatformLib_StrCat(path, pathBufferLength, extension);
}

/*
==================
COM_ReplaceExtension
==================
*/
void COM_ReplaceExtension(char* path, size_t pathBufferLength, const char* extension)
{
	COM_StripExtension(path);
	COM_DefaultExtension(path, pathBufferLength, extension);
}

/*
============
COM_RemoveLineFeed
============
*/
void COM_RemoveLineFeed(char* str)
{
	while ( *str != '\0' )
	{
		if ( *str == '\r' || *str == '\n' )
		{
			*str = '\0';
		}

		++str;
	}
}

/*
============
COM_FixSlashes

Changes all '\' characters into '/' characters, in place.
============
*/
void COM_FixSlashes(char* pname)
{
	for ( ; *pname; pname++ )
	{
		if ( *pname == '\\' )
		{
			*pname = '/';
		}
	}
}

/*
============
COM_PathSlashFix
============
*/
qboolean COM_PathSlashFix(char* path, size_t pathBufferLength)
{
	size_t len;

	if ( !path || pathBufferLength < 1 )
	{
		return false;
	}

	len = Q_strlen(path);

	// Don't overflow the buffer.
	if ( len + 2 > pathBufferLength )
	{
		return false;
	}

	if ( path[len - 1] != '\\' && path[len - 1] != '/' )
	{
		PlatformLib_StrCpy(path + len, pathBufferLength - len, "/");
	}

	return true;
}

/*
==============
COM_IsSingleChar

interpert this character as single
==============
*/
static int COM_IsSingleChar(unsigned int flags, char c)
{
	if ( c == '{' || c == '}' || c == '\'' || c == ',' )
	{
		return true;
	}

	if ( !FBitSet(flags, PFILE_IGNOREBRACKET) && (c == ')' || c == '(') )
	{
		return true;
	}

	if ( FBitSet(flags, PFILE_HANDLECOLON) && c == ':' )
	{
		return true;
	}

	return false;
}

/*
==============
COM_ParseFile

text parser
==============
*/
char* COM_ParseFileSafe(char* data, char* token, const int size, unsigned int flags, int* plen, qboolean* quoted)
{
	char c;
	int len = 0;
	qboolean overflow = false;

	if ( quoted )
		*quoted = false;

	if ( !token || !size )
	{
		if ( plen )
			*plen = 0;
		return NULL;
	}

	token[0] = 0;

	if ( !data )
		return NULL;
// skip whitespace
skipwhite:
	while ( (c = ((byte)*data)) <= ' ' )
	{
		if ( c == 0 )
		{
			if ( plen )
				*plen = overflow ? -1 : len;
			return NULL;  // end of file;
		}
		data++;
	}

	// skip // comments
	if ( c == '/' && data[1] == '/' )
	{
		while ( *data && *data != '\n' )
			data++;
		goto skipwhite;
	}

	// handle quoted strings specially
	if ( c == '\"' )
	{
		if ( quoted )
			*quoted = true;

		data++;
		while ( 1 )
		{
			c = (byte)*data;

			// unexpected line end
			if ( !c )
			{
				token[len] = 0;
				if ( plen )
					*plen = overflow ? -1 : len;
				return data;
			}
			data++;

			if ( c == '\\' && *data == '"' )
			{
				if ( len + 1 < size )
				{
					token[len] = (byte)*data;
					len++;
				}
				else
					overflow = true;

				data++;
				continue;
			}

			if ( c == '\"' )
			{
				token[len] = 0;
				if ( plen )
					*plen = overflow ? -1 : len;
				return data;
			}

			if ( len + 1 < size )
			{
				token[len] = c;
				len++;
			}
			else
				overflow = true;
		}
	}

	// parse single characters
	if ( COM_IsSingleChar(flags, c) )
	{
		if ( size >= 2 )  // char and \0
		{
			token[len] = c;
			len++;
			token[len] = 0;
			if ( plen )
				*plen = overflow ? -1 : len;
			return data + 1;
		}
		else
		{
			// couldn't pass anything
			token[0] = 0;
			if ( plen )
				*plen = overflow ? -1 : len;
			return data;
		}
	}

	// parse a regular word
	do
	{
		if ( len + 1 < size )
		{
			token[len] = c;
			len++;
		}
		else
			overflow = true;

		data++;
		c = ((byte)*data);

		if ( COM_IsSingleChar(flags, c) )
			break;
	}
	while ( c > 32 );

	token[len] = 0;

	if ( plen )
		*plen = overflow ? -1 : len;

	return data;
}

int matchpattern(const char* in, const char* pattern, qboolean caseinsensitive)
{
	return matchpattern_with_separator(in, pattern, caseinsensitive, "/\\:", false);
}

// wildcard_least_one: if true * matches 1 or more characters
//                     if false * matches 0 or more characters
int matchpattern_with_separator(
	const char* in,
	const char* pattern,
	qboolean caseinsensitive,
	const char* separators,
	qboolean wildcard_least_one)
{
	int c1, c2;

	while ( *pattern )
	{
		switch ( *pattern )
		{
			case 0:
			{
				return 1;  // end of pattern
			}

			case '?':  // match any single character
			{
				if ( *in == 0 || Q_strchr(separators, *in) )
				{
					return 0;  // no match
				}

				in++;
				pattern++;

				break;
			}

			case '*':  // match anything until following string
			{
				if ( wildcard_least_one )
				{
					if ( *in == 0 || Q_strchr(separators, *in) )
					{
						return 0;  // no match
					}

					in++;
				}

				pattern++;

				while ( *in )
				{
					if ( Q_strchr(separators, *in) )
					{
						break;
					}

					// see if pattern matches at this offset
					if ( matchpattern_with_separator(in, pattern, caseinsensitive, separators, wildcard_least_one) )
					{
						return 1;
					}

					// nope, advance to next offset
					in++;
				}

				break;
			}

			default:
			{
				if ( *in != *pattern )
				{
					if ( !caseinsensitive )
					{
						return 0;  // no match
					}

					c1 = *in;

					if ( c1 >= 'A' && c1 <= 'Z' )
					{
						c1 += 'a' - 'A';
					}

					c2 = *pattern;

					if ( c2 >= 'A' && c2 <= 'Z' )
					{
						c2 += 'a' - 'A';
					}

					if ( c1 != c2 )
					{
						return 0;  // no match
					}
				}

				in++;
				pattern++;
				break;
			}
		}
	}

	if ( *in )
	{
		return 0;  // reached end of pattern but not end of input
	}

	return 1;  // success
}

qboolean COM_StringIsTerminated(const char* str, size_t maxLength)
{
	if ( !str )
	{
		return false;
	}

	const char* end = str + maxLength;

	for ( ; str < end; ++str )
	{
		if ( !(*str) )
		{
			return true;
		}
	}

	return false;
}

int Q_strcmp(const char* s1, const char* s2)
{
	return unlikely(!s1) ? (!s2 ? 0 : -1) : (unlikely(!s2) ? 1 : strcmp(s1, s2));
}

int Q_strncmp(const char* s1, const char* s2, size_t n)
{
	return unlikely(!s1) ? (!s2 ? 0 : -1) : (unlikely(!s2) ? 1 : strncmp(s1, s2, n));
}

char* Q_strstr(const char* s1, const char* s2)
{
	return unlikely(!s1 || !s2) ? NULL : (char*)strstr(s1, s2);
}

// libc extensions, be careful

int Q_stricmp(const char* s1, const char* s2)
{
	return unlikely(!s1) ? (!s2 ? 0 : -1) : (unlikely(!s2) ? 1 : PlatformLib_StrCaseCmp(s1, s2));
}

int Q_strnicmp(const char* s1, const char* s2, size_t n)
{
	return unlikely(!s1) ? (!s2 ? 0 : -1) : (unlikely(!s2) ? 1 : PlatformLib_StrNCaseCmp(s1, s2, n));
}

char* Q_strcpy(char* dest, size_t destSize, const char* src)
{
	return PlatformLib_StrCpy(dest, destSize, src);
}

char* Q_strcat(char* dest, size_t destSize, const char* src)
{
	return PlatformLib_StrCat(dest, destSize, src);
}

#if !defined(HAVE_STRCHRNUL)
const char* Q_strchrnul(const char* s, int c)
{
	const char* p = Q_strchr(s, c);

	if ( p )
	{
		return p;
	}

	return s + Q_strlen(s);
}
#endif
