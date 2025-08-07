/*
file_system.cpp - simple version of game engine filesystem for tools
Copyright (C) 2015 Uncle Mike

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

#if XASH_WIN32()
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <direct.h>
#include <io.h>
#else
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#define O_BINARY 0
#endif

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include "CRTLib/crtlib.h"
#include "CompileTools/conprint.h"
#include "CompileTools/file_system.h"
#include "CompileTools/zone.h"
#include "CompileTools/cmdlib.h"
#include "CompileTools/wadfileoperations.h"
#include "PlatformLib/System.h"
#include "PlatformLib/File.h"
#include "MathLib/mathdefs.h"

/*
=============================================================================

FILEMATCH COMMON SYSTEM

=============================================================================
*/
int matchpattern(const char* str, const char* cmp, bool caseinsensitive)
{
	int c1, c2;

	while ( *cmp )
	{
		switch ( *cmp )
		{
			case 0:
				return 1;  // end of pattern
			case '?':  // match any single character
				if ( *str == 0 || *str == '/' || *str == '\\' || *str == ':' )
					return 0;  // no match
				str++;
				cmp++;
				break;
			case '*':  // match anything until following string
				if ( !*str )
					return 1;  // match
				cmp++;
				while ( *str )
				{
					if ( *str == '/' || *str == '\\' || *str == ':' )
						break;
					// see if pattern matches at this offset
					if ( matchpattern(str, cmp, caseinsensitive) )
						return 1;
					// nope, advance to next offset
					str++;
				}
				break;
			default:
				if ( *str != *cmp )
				{
					if ( !caseinsensitive )
						return 0;  // no match
					c1 = Q_tolower(*str);
					c2 = Q_tolower(*cmp);
					if ( c1 != c2 )
						return 0;  // no match
				}

				str++;
				cmp++;
				break;
		}
	}

	// reached end of pattern but not end of input?
	return (*str) ? 0 : 1;
}

void stringlistinit(stringlist_t* list)
{
	memset(list, 0, sizeof(*list));
}

void stringlistfreecontents(stringlist_t* list)
{
	int i;

	for ( i = 0; i < list->numstrings; i++ )
	{
		if ( list->strings[i] )
			Mem_Free(list->strings[i], C_STRING);
		list->strings[i] = NULL;
	}

	if ( list->strings )
		Mem_Free(list->strings, C_STRING);

	list->numstrings = 0;
	list->maxstrings = 0;
	list->strings = NULL;
}

void stringlistappend(stringlist_t* list, char* text)
{
	size_t textlen;
	char** oldstrings;

	if ( !Q_stricmp(text, ".") || !Q_stricmp(text, "..") )
		return;  // ignore the virtual directories

	if ( list->numstrings >= list->maxstrings )
	{
		oldstrings = list->strings;
		list->maxstrings += 4096;
		list->strings = (char**)Mem_Alloc(list->maxstrings * sizeof(*list->strings), C_STRING);
		if ( list->numstrings )
			memcpy(list->strings, oldstrings, list->numstrings * sizeof(*list->strings));
		if ( oldstrings )
			Mem_Free(oldstrings, C_STRING);
	}

	textlen = Q_strlen(text) + 1;
	list->strings[list->numstrings] = (char*)Mem_Alloc(textlen, C_STRING);
	memcpy(list->strings[list->numstrings], text, textlen);
	list->numstrings++;
}

void stringlistsort(stringlist_t* list)
{
	char* temp;
	int i, j;

	// this is a selection sort (finds the best entry for each slot)
	for ( i = 0; i < list->numstrings - 1; i++ )
	{
		for ( j = i + 1; j < list->numstrings; j++ )
		{
			if ( Q_strcmp(list->strings[i], list->strings[j]) > 0 )
			{
				temp = list->strings[i];
				list->strings[i] = list->strings[j];
				list->strings[j] = temp;
			}
		}
	}
}

void listdirectory(stringlist_s* list, const char* path, bool lowercase)
{
#if XASH_WIN32()
	char pattern[4096];
	struct _finddata_t n_file;
	intptr_t hFile;
#else
	DIR* dir;
	struct dirent* entry;
#endif

#if XASH_WIN32()
	Q_snprintf(pattern, sizeof(pattern), "%s*", path);

	// ask for the directory listing handle
	hFile = _findfirst(pattern, &n_file);
	if ( hFile == -1 )
		return;

	// start a new chain with the the first name
	stringlistappend(list, n_file.name);
	// iterate through the directory
	while ( _findnext(hFile, &n_file) == 0 )
	{
		stringlistappend(list, n_file.name);
	}
	_findclose(hFile);
#else
	if ( !(dir = opendir(path)) )
		return;

	// iterate through the directory
	while ( (entry = readdir(dir)) )
		stringlistappend(list, entry->d_name);
	closedir(dir);
#endif

	// convert names to lowercase because windows doesn't care, but pattern matching code often does
	if ( lowercase )
	{
		for ( int i = 0; i < list->numstrings; i++ )
		{
			for ( char* c = (char*)list->strings[i]; *c; c++ )
			{
				if ( *c >= 'A' && *c <= 'Z' )
					*c += 'a' - 'A';
			}
		}
	}
}
/*
=============================================================================

FILESYSTEM PUBLIC BASE FUNCTIONS

=============================================================================
*/

byte* COM_LoadFile(const char* filepath, size_t* filesize, bool safe)
{
	int handle;
	size_t size;
	unsigned char* buf;

	handle = PlatformLib_OpenWithPermissions(filepath, O_RDONLY | O_BINARY, 0666);

	if ( filesize )
		*filesize = 0;
	if ( handle < 0 )
	{
		if ( safe )
		{
			COM_FatalError("Couldn't open %s\n", filepath);
		}

		return NULL;
	}

	size = PlatformLib_LSeek(handle, 0, SEEK_END);
	PlatformLib_LSeek(handle, 0, SEEK_SET);

	buf = (unsigned char*)Mem_Alloc(size + 1, C_FILESYSTEM);
	buf[size] = '\0';
	PlatformLib_Read(handle, (unsigned char*)buf, (unsigned int)size);
	PlatformLib_Close(handle);

	if ( filesize )
	{
		*filesize = size;
	}

	return buf;
}

bool COM_SaveFile(const char* filepath, void* buffer, size_t filesize, bool safe)
{
	int handle;
	int size;

	if ( buffer == NULL || filesize <= 0 )
		return false;

	COM_CreatePath((char*)filepath);

	handle = PlatformLib_OpenWithPermissions(filepath, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC, 0666);
	if ( handle < 0 )
	{
		if ( safe )
			COM_FatalError("Couldn't write %s\n", filepath);
		return false;
	}

	size = PlatformLib_Write(handle, buffer, (unsigned int)filesize);
	PlatformLib_Close(handle);

	if ( size < 0 )
	{
		return false;
	}

	return true;
}

/*
==================
COM_CreatePath
==================
*/
void COM_CreatePath(char* path)
{
	char *ofs, save;

	for ( ofs = path + 1; *ofs; ofs++ )
	{
		if ( *ofs == '/' || *ofs == '\\' )
		{
			// create the directory
			save = *ofs;
			*ofs = 0;
			PlatformLib_MkDir(path);
			*ofs = save;
		}
	}
}

/*
==================
COM_FileExists
==================
*/
bool COM_FileExists(const char* path)
{
#if XASH_WIN32()
	int desc;

	if ( (desc = PlatformLib_OpenWithPermissions(path, O_RDONLY | O_BINARY, 0666)) < 0 )
		return false;

	PlatformLib_Close(desc);
	return true;
#else
	int ret;
	struct stat buf;

	ret = stat(path, &buf);

	if ( ret < 0 )
		return false;

	return S_ISREG(buf.st_mode);
#endif
}

/*
==================
COM_FolderExists
==================
*/
bool COM_FolderExists(const char* path)
{
#if XASH_WIN32()
	DWORD dwFlags = GetFileAttributes(path);

	return (dwFlags != -1) && (dwFlags & FILE_ATTRIBUTE_DIRECTORY);
#elif XASH_POSIX()
	DIR* dir = opendir(path);

	if ( dir )
	{
		closedir(dir);
		return true;
	}
	else if ( (errno == ENOENT) || (errno == ENOTDIR) )
	{
		return false;
	}
	else
	{
		MsgDev(D_ERROR, "FS_SysFolderExists: problem while opening dir: %s\n", strerror(errno));
		return false;
	}
#else
#error "Implement me"
#endif
}

/*
====================
COM_FileTime

Internal function used to determine filetime
====================
*/
int COM_FileTime(const char* filename)
{
	struct stat buf;

	if ( stat(filename, &buf) == -1 )
	{
		return -1;
	}

	return (int)buf.st_mtime;
}

/*
=============================================================================

			OBSOLETE FUNCTIONS

=============================================================================
*/
int SafeOpenWrite(const char* filename)
{
	int handle = PlatformLib_OpenWithPermissions(filename, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0666);
	if ( handle < 0 )
	{
		COM_FatalError("couldn't open %s\n", filename);
	}
	return handle;
}

int SafeOpenRead(const char* filename)
{
	int handle = PlatformLib_OpenWithPermissions(filename, O_RDONLY | O_BINARY, 0666);
	if ( handle < 0 )
	{
		COM_FatalError("couldn't open %s\n", filename);
	}
	return handle;
}

void SafeReadExt(int handle, void* buffer, int count, const char* file, const int line)
{
	int read_count = PlatformLib_Read(handle, buffer, count);
	if ( read_count != count )
	{
		COM_FatalError("file read failure ( %i != %i ) at %s:%i\n", read_count, count, file, line);
	}
}

void SafeWriteExt(int handle, void* buffer, int count, const char* file, const int line)
{
	int write_count = PlatformLib_Write(handle, buffer, count);
	if ( write_count != count )
	{
		COM_FatalError("file write failure ( %i != %i ) at %s:%i\n", write_count, count, file, line);
	}
}
