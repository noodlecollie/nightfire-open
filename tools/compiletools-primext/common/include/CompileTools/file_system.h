/*
file_system.h - simple version of game engine filesystem for tools
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

#pragma once

#include <time.h>
#include <stdint.h>
#include <stdarg.h>
#include "CompileTools/engine/wadfile.h"
#include "BuildPlatform/Typedefs.h"
#include "basefs.h"

#define FILE_BUFF_SIZE (65535)
#define HINT_NAMELEN 5  // e.g. _mask, _norm
#define MAX_FILES_IN_WAD 65535  // real limit as above <2Gb size not a lumpcount

// replace lumps in a wad
#define REP_IGNORE 0
#define REP_NORMAL 1
#define REP_FORCE 2

// PAK errors
#define PAK_LOAD_OK 0
#define PAK_LOAD_COULDNT_OPEN 1
#define PAK_LOAD_BAD_HEADER 2
#define PAK_LOAD_BAD_FOLDERS 3
#define PAK_LOAD_TOO_MANY_FILES 4
#define PAK_LOAD_NO_FILES 5
#define PAK_LOAD_CORRUPTED 6

// WAD errors
#define WAD_LOAD_OK 0
#define WAD_LOAD_COULDNT_OPEN 1
#define WAD_LOAD_BAD_HEADER 2
#define WAD_LOAD_BAD_FOLDERS 3
#define WAD_LOAD_TOO_MANY_FILES 4
#define WAD_LOAD_NO_FILES 5
#define WAD_LOAD_CORRUPTED 6

typedef struct stringlist_s
{
	// maxstrings changes as needed, causing reallocation of strings[] array
	int maxstrings;
	int numstrings;
	char** strings;
} stringlist_t;

typedef struct
{
	int numfilenames;
	char** filenames;
	char* filenamesbuffer;
} search_t;

typedef struct vfile_s
{
	byte* buff;
	size_t buffsize;
	size_t length;
	size_t offset;
} vfile_t;

search_t* FS_Search(const char* pattern, int caseinsensitive, int gamedironly);
byte* COM_LoadFile(const char* filepath, size_t* filesize, bool safe = true);
bool COM_SaveFile(const char* filepath, void* buffer, size_t filesize, bool safe = true);
int COM_FileTime(const char* filename);
bool COM_FolderExists(const char* path);
bool COM_FileExists(const char* path);
void COM_CreatePath(char* path);

// virtual filesystem
vfile_t* VFS_Create(const byte* buffer = NULL, size_t buffsize = 0);
size_t VFS_Read(vfile_t* file, void* buffer, size_t buffersize);
size_t VFS_Write(vfile_t* file, const void* buf, size_t size);
size_t VFS_Insert(vfile_t* file, const void* buf, size_t size);
byte* VFS_GetBuffer(vfile_t* file);
size_t VFS_GetSize(vfile_t* file);
size_t VFS_Tell(vfile_t* file);
bool VFS_Eof(vfile_t* file);
size_t VFS_Print(vfile_t* file, const char* msg);
size_t VFS_IPrint(vfile_t* file, const char* msg);
size_t VFS_VPrintf(vfile_t* file, const char* format, va_list ap);
size_t VFS_VIPrintf(vfile_t* file, const char* format, va_list ap);
size_t VFS_Printf(vfile_t* file, const char* format, ...);
size_t VFS_IPrintf(vfile_t* file, const char* format, ...);
int VFS_Seek(vfile_t* file, int64_t offset, int whence);
char VFS_Getc(vfile_t* file);
int VFS_Gets(vfile_t* file, byte* string, size_t bufsize);
void VFS_Close(vfile_t* file);

// compare routines
int matchpattern(const char* str, const char* cmp, bool caseinsensitive);

// search routines
void stringlistinit(stringlist_t* list);
void stringlistfreecontents(stringlist_t* list);
void stringlistappend(stringlist_t* list, char* text);
void stringlistsort(stringlist_t* list);
void listdirectory(stringlist_t* list, const char* path, bool tolower = false);

// replace lumps protect
void SetReplaceLevel(int level);
int GetReplaceLevel(void);

int SafeOpenWrite(const char* filename);
int SafeOpenRead(const char* filename);
void SafeReadExt(int handle, void* buffer, int count, const char* file, const int line);
void SafeWriteExt(int handle, void* buffer, int count, const char* file, const int line);

#define SafeRead(file, buffer, count) SafeReadExt(file, buffer, count, __FILE__, __LINE__)
#define SafeWrite(file, buffer, count) SafeWriteExt(file, buffer, count, __FILE__, __LINE__)
