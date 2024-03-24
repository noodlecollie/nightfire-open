/*
dir.c - caseinsensitive directory operations
Copyright (C) 2022 Alibek Omarov, Velaron
Copyright (C) 2023 Xash3D FWGS contributors

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

#include "BuildPlatform/PlatformID.h"

#if XASH_POSIX()
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#if XASH_LINUX()
#include <linux/fs.h>

#ifndef FS_CASEFOLD_FL  // for compatibility with older distros
#define FS_CASEFOLD_FL 0x40000000
#endif  // FS_CASEFOLD_FL

#endif  // XASH_LINUX()

#include "CRTLib/bitdefs.h"
#include "CRTLib/crtlib.h"
#include "PlatformLib/File.h"
#include "EngineInternalAPI/log_strings.h"
#include "filesystem_internal.h"

enum
{
	DIRENTRY_EMPTY_DIRECTORY = 0,  // don't care if it's not directory or it's empty
	DIRENTRY_NOT_SCANNED = -1,
	DIRENTRY_CASEINSENSITIVE = -2,  // directory is already caseinsensitive, just copy whatever is left
};

typedef struct dir_s
{
	string name;
	int numentries;
	struct dir_s* entries;  // sorted
} dir_t;

static qboolean Platform_GetDirectoryCaseSensitivity(const char* dir)
{
#if XASH_WIN32()
	(void)dir;
	return false;
#elif XASH_LINUX() && defined(FS_IOC_GETFLAGS)
	int flags = 0;
	int fd;

	fd = PlatformLib_Open(dir, O_RDONLY | O_NONBLOCK);
	if ( fd < 0 )
		return true;

	if ( ioctl(fd, FS_IOC_GETFLAGS, &flags) < 0 )
		return true;

	PlatformLib_Close(fd);

	return !FBitSet(flags, FS_CASEFOLD_FL);
#else
	(void)dir;
	return true;
#endif
}

static int FS_SortDirEntries(const void* _a, const void* _b)
{
	const dir_t* a = _a;
	const dir_t* b = _b;
	return Q_stricmp(a->name, b->name);
}

static void FS_FreeDirEntries(dir_t* dir)
{
	if ( dir->entries )
	{
		int i;
		for ( i = 0; i < dir->numentries; i++ )
			FS_FreeDirEntries(&dir->entries[i]);
		dir->entries = NULL;
	}

	dir->numentries = DIRENTRY_NOT_SCANNED;
}

static void FS_InitDirEntries(dir_t* dir, const stringlist_t* list)
{
	int i;

	dir->numentries = list->numstrings;
	dir->entries = Mem_Malloc(fs_mempool, sizeof(dir_t) * dir->numentries);

	for ( i = 0; i < list->numstrings; i++ )
	{
		dir_t* entry = &dir->entries[i];

		Q_strncpy(entry->name, list->strings[i], sizeof(entry->name));
		entry->numentries = DIRENTRY_NOT_SCANNED;
		entry->entries = NULL;
	}

	qsort(dir->entries, dir->numentries, sizeof(dir->entries[0]), FS_SortDirEntries);
}

static void FS_PopulateDirEntries(dir_t* dir, const char* path)
{
	stringlist_t list;

	if ( !FS_SysFolderExists(path) )
	{
		dir->numentries = DIRENTRY_EMPTY_DIRECTORY;
		dir->entries = NULL;
		return;
	}

	if ( !Platform_GetDirectoryCaseSensitivity(path) )
	{
		dir->numentries = DIRENTRY_CASEINSENSITIVE;
		dir->entries = NULL;
		return;
	}

	stringlistinit(&list);
	listdirectory(&list, path, FS_SEARCHFLAG_DIRECTORIES | FS_SEARCHFLAG_FILES);

	if ( !list.numstrings )
	{
		dir->numentries = DIRENTRY_EMPTY_DIRECTORY;
		dir->entries = NULL;
	}
	else
	{
		FS_InitDirEntries(dir, &list);
	}

	stringlistfreecontents(&list);
}

static int FS_FindDirEntry(dir_t* dir, const char* name)
{
	int left, right;

	// look for the file (binary search)
	left = 0;
	right = dir->numentries - 1;

	while ( left <= right )
	{
		int middle = (left + right) / 2;
		int diff;

		diff = Q_stricmp(dir->entries[middle].name, name);

		// found it
		if ( !diff )
		{
			return middle;
		}

		// if we're too far in the list
		if ( diff > 0 )
		{
			right = middle - 1;
		}
		else
		{
			left = middle + 1;
		}
	}

	return -1;
}

static void FS_MergeDirEntries(dir_t* dir, const stringlist_t* list)
{
	int i;
	dir_t temp;

	// glorified realloc for sorted dir entries
	// make new array and copy old entries with same name and subentries
	// everything else get freed

	FS_InitDirEntries(&temp, list);

	for ( i = 0; i < dir->numentries; i++ )
	{
		dir_t* oldentry = &dir->entries[i];
		dir_t* newentry;
		int j;

		// don't care about directories without subentries
		if ( oldentry->entries == NULL )
			continue;

		// try to find this directory in new tree
		j = FS_FindDirEntry(&temp, oldentry->name);

		// not found, free memory
		if ( j < 0 )
		{
			FS_FreeDirEntries(oldentry);
			continue;
		}

		// found directory, move all entries
		newentry = &temp.entries[j];

		newentry->numentries = oldentry->numentries;
		newentry->entries = oldentry->entries;
	}

	// now we can free old tree and replace it with temporary
	// do not add null check there! If we hit it, it's probably a logic error!
	Mem_Free(dir->entries);
	dir->numentries = temp.numentries;
	dir->entries = temp.entries;
}

static int FS_UpdateStaleDirectoryDataAndFindEntry(dir_t* dir, const char* directoryOnDisk, const char* entryname)
{
	stringlist_t list;
	int ret;

	stringlistinit(&list);
	listdirectory(&list, directoryOnDisk, FS_SEARCHFLAG_DIRECTORIES | FS_SEARCHFLAG_FILES);

	if ( list.numstrings == 0 )  // empty directory
	{
		FS_FreeDirEntries(dir);
		dir->numentries = DIRENTRY_EMPTY_DIRECTORY;
		ret = -1;
	}
	else if ( dir->numentries <= DIRENTRY_EMPTY_DIRECTORY )  // not initialized or was empty
	{
		FS_InitDirEntries(dir, &list);
		ret = FS_FindDirEntry(dir, entryname);
	}
	else if ( list.numstrings != dir->numentries )  // quick update
	{
		FS_MergeDirEntries(dir, &list);
		ret = FS_FindDirEntry(dir, entryname);
	}
	else
	{
		// The path on disk had entries in it, and the number of
		// entries was the same as the number that the struct contains.
		// Do heavy compare in case the disk now has an entry we need.

		int i;

		for ( i = 0; i < list.numstrings; i++ )
		{
			if ( !Q_stricmp(list.strings[i], entryname) )
			{
				break;
			}
		}

		if ( i < list.numstrings )
		{
			FS_MergeDirEntries(dir, &list);
			ret = FS_FindDirEntry(dir, entryname);
		}
		else
		{
			ret = -1;
		}
	}

	stringlistfreecontents(&list);
	return ret;
}

static inline qboolean
FS_AppendToPath(char* dst, size_t* pi, const size_t len, const char* src, const char* path, const char* err)
{
	size_t i = *pi;

	i += Q_strncpy(&dst[i], src, len - i);
	*pi = i;

	if ( i >= len )
	{
		Con_Printf(S_ERROR "FS_FixFileCase: overflow while searching %s (%s)\n", path, err);
		return false;
	}

	return true;
}

// This is a somewhat obscure function, but is critically important for files to be able
// to be found when running searches. Hopefully exhaustively commenting the behaviour
// here will make it easier to follow.
//   dir              - Directory in tree to use as the root.
//   inPath           - Path to evaluate. This is treated as relative to the root.
//   outPath          - Buffer to receive the evaluated and fixed path.
//   outPathMaxLength - Size of the output path buffer.
//   operation        - Type of operation this path fix is needed for.
qboolean FS_FixFileCase(
	dir_t* dir,
	const char* inPath,
	char* outPath,
	const size_t outPathMaxLength,
	DirectoryOperation_e operation)
{
	size_t computedPathLength = 0;
	const qboolean requiresExistenceCheck = operation == DIROP_ACCESS;

	// Append the base directory name to the destination buffer to start off with.
	if ( !FS_AppendToPath(outPath, &computedPathLength, outPathMaxLength, dir->name, inPath, "init") )
	{
		return false;
	}

	if ( !(*inPath) )
	{
		// Nothing to fix.
		return true;
	}

	// Keep iterating over the input path looking for directory separators.
	// Each time this loop is run, the outPath buffer holds the directory
	// prefix that we're currently concerned with.
	for ( const char *prev = inPath, *next = Q_strchrnul(prev, '/');  //
		  /*No check - we break manually*/;  //
		  prev = next + 1, next = Q_strchrnul(prev, '/') )
	{
		// Start by assuming that we will not update the struct's internal data.
		qboolean dirEntriesRefreshed = false;

		if ( dir->numentries == DIRENTRY_NOT_SCANNED )
		{
			// Lazy initialisation. The dir struct does not yet hold
			// info from the actual filesystem, so refresh it and record
			// that we did so.
			FS_PopulateDirEntries(dir, outPath);
			dirEntriesRefreshed = true;
		}

		// If this subdirectory is case insensitive, we don't need to fix any casing.
		// Add the rest of the path to the destination buffer, we can quit early here.
		if ( dir->numentries == DIRENTRY_CASEINSENSITIVE )
		{
			// Make sure the append succeeded.
			if ( !FS_AppendToPath(
					 outPath,
					 &computedPathLength,
					 outPathMaxLength,
					 prev,
					 inPath,
					 "caseinsensitive entry") )
			{
				return false;
			}

			// If we don't care whether the path exists or not, just return true.
			if ( !requiresExistenceCheck )
			{
				return true;
			}

			// If we're not creating the path, it must actually exist on disk.
			return FS_SysFileOrFolderExists(outPath);
		}

		// Next, prepare a buffer which holds the name of the next segment in
		// the path. This represents a subdirectory under the parent.
		char currentSubdirName[MAX_SYSPATH];
		Q_strncpy(currentSubdirName, prev, next - prev + 1);

		// Check if this subdirectory exists as an entry in the list that's
		// held in the struct.
		int dirEntryIndex = FS_FindDirEntry(dir, currentSubdirName);

		// If the subdirectory doesn't exist in the struct, we need to make
		// sure that this is actually the case on the filesystem itself,
		// because the struct data could be stale. We do this below.
		if ( dirEntryIndex < 0 )
		{
			// If we had scanned the directory on disk earlier, we consider
			// its data up-to-date. However, if we didn't scan it earlier
			// then the list in the struct might be stale.
			if ( !dirEntriesRefreshed )
			{
				// Do a combined, optimised operation: update the list of entries
				// in the struct, and then subsequently check to see if the
				// subdirectory we're looking for is in there.
				dirEntryIndex = FS_UpdateStaleDirectoryDataAndFindEntry(dir, outPath, currentSubdirName);
			}

			// If we really couldn't find the entry we were looking for:
			if ( dirEntryIndex < 0 )
			{
				// If we don't need to check for existence, we only need
				// to report if the append was successful.
				if ( !requiresExistenceCheck )
				{
					return FS_AppendToPath(outPath, &computedPathLength, outPathMaxLength, prev, inPath, "create path");
				}

				// If we're not creating a path, return false because the item
				// we just checked for could not be found.
				return false;
			}

			// If we get here, the directory data in the struct was updated,
			// so set the flag indicating that it's live.
			dirEntriesRefreshed = true;
		}

		// Set the struct pointer to the new entry we now know exists.
		dir = &dir->entries[dirEntryIndex];

		// Store the path length in a new variable.
		// FS_AppendToPath() modifies the length passed to it as a result
		// of the append, but we don't want to modify the official length just yet
		// as we re-use it below in some checks.
		size_t proposedNewPathLength = computedPathLength;

		// Append the name of this new directory to the output, and make sure it succeeds.
		if ( !FS_AppendToPath(outPath, &proposedNewPathLength, outPathMaxLength, dir->name, inPath, "case fix") )
		{
			return false;
		}

		// If we did not have to refresh the entries from earlier,
		// and the new path we just computed does not actually exist
		// on disk, we need to re-run the update process for the
		// struct's internal data.
		if ( !dirEntriesRefreshed && !FS_SysFileOrFolderExists(outPath) )  // file not found, rescan...
		{
			// Get rid of the subdirectory we just appended to the path.
			// We supply the path and subdirectory as two separate arguments below.
			outPath[computedPathLength] = '\0';

			// Do a combined, optimised operation: update the list of entries
			// in the struct, and then subsequently check to see if the
			// subdirectory we're looking for is in there.
			dirEntryIndex = FS_UpdateStaleDirectoryDataAndFindEntry(dir, outPath, currentSubdirName);

			// If the subdirectory was not present:
			if ( dirEntryIndex < 0 )
			{
				// If we don't need to check for existence, we only need
				// to report if the append was successful.
				if ( !requiresExistenceCheck )
				{
					return FS_AppendToPath(
						outPath,
						&computedPathLength,
						outPathMaxLength,
						prev,
						inPath,
						"create path rescan");
				}

				// Return a failure because the path does not exist.
				return false;
			}

			// Now we know that the subdirectory exists.
			// Update the struct pointer to use it.
			dir = &dir->entries[dirEntryIndex];

			// Update the full path too, and make sure the operation succeeds.
			if ( !FS_AppendToPath(
					 outPath,
					 &proposedNewPathLength,
					 outPathMaxLength,
					 dir->name,
					 inPath,
					 "case fix rescan") )
			{
				return false;
			}
		}

		// We confirmed that the new path exists, so update the main
		// path length variable to represent the new path.
		computedPathLength = proposedNewPathLength;

		// If we reached the path terminator (either a null, or a
		// separator followed by a null), we're done.
		if ( next[0] == '\0' || (next[0] == '/' && next[1] == '\0') )
		{
			break;
		}

		// On to the next section - append a separator and go round the loop again.
		if ( !FS_AppendToPath(outPath, &computedPathLength, outPathMaxLength, "/", inPath, "path separator") )
		{
			return false;
		}
	}

	return true;
}

static void FS_Close_DIR(searchpath_t* search)
{
	FS_FreeDirEntries(search->pkg.dir);
	Mem_Free(search->pkg.dir);
}

static void FS_PrintInfo_DIR(searchpath_t* search, char* dst, size_t size)
{
	Q_strncpy(dst, search->filename, size);
}

static int FS_FindFile_DIR(searchpath_t* search, const char* path, char* fixedname, size_t len)
{
	char netpath[MAX_SYSPATH];

	if ( !FS_FixFileCase(search->pkg.dir, path, netpath, sizeof(netpath), DIROP_ACCESS) )
	{
		return -1;
	}

	if ( FS_SysFileExists(netpath) )
	{
		// return fixed case file name only local for that searchpath
		if ( fixedname )
		{
			Q_strncpy(fixedname, netpath + Q_strlen(search->filename), len);
		}

		return 0;
	}

	return -1;
}

static void
FS_Search_DIR(searchpath_t* search, stringlist_t* list, const char* pattern, int caseinsensitive, uint32_t flags)
{
	string netpath, temp;
	stringlist_t dirlist;
	const char *slash, *backslash, *colon, *separator;
	int basepathlength, dirlistindex, resultlistindex;
	char* basepath;

	(void)caseinsensitive;

	slash = Q_strrchr(pattern, '/');
	backslash = Q_strrchr(pattern, '\\');
	colon = Q_strrchr(pattern, ':');

	separator = slash > backslash ? slash : backslash;
	separator = separator > colon ? separator : colon;

	basepathlength = (int)(separator ? (separator + 1 - pattern) : 0);
	basepath = Mem_Calloc(fs_mempool, basepathlength + 1);

	if ( basepathlength )
	{
		memcpy(basepath, pattern, basepathlength);
	}

	basepath[basepathlength] = '\0';

	if ( !FS_FixFileCase(search->pkg.dir, basepath, netpath, sizeof(netpath), DIROP_ACCESS) )
	{
		Mem_Free(basepath);
		return;
	}

	stringlistinit(&dirlist);
	listdirectory(&dirlist, netpath, flags);

	Q_strncpy(temp, basepath, sizeof(temp));

	for ( dirlistindex = 0; dirlistindex < dirlist.numstrings; dirlistindex++ )
	{
		Q_strncpy(&temp[basepathlength], dirlist.strings[dirlistindex], sizeof(temp) - basepathlength);

		if ( matchpattern(temp, pattern, true) )
		{
			for ( resultlistindex = 0; resultlistindex < list->numstrings; resultlistindex++ )
			{
				if ( !Q_strcmp(list->strings[resultlistindex], temp) )
				{
					break;
				}
			}

			if ( resultlistindex == list->numstrings )
			{
				stringlistappend(list, temp);
			}
		}
	}

	stringlistfreecontents(&dirlist);

	Mem_Free(basepath);
}

static int FS_FileTime_DIR(searchpath_t* search, const char* filename)
{
	char path[MAX_SYSPATH];

	Q_snprintf(path, sizeof(path), "%s%s", search->filename, filename);
	return FS_SysFileTime(path);
}

static file_t* FS_OpenFile_DIR(searchpath_t* search, const char* filename, const char* mode, int pack_ind)
{
	char path[MAX_SYSPATH];

	(void)pack_ind;

	Q_snprintf(path, sizeof(path), "%s%s", search->filename, filename);
	return FS_SysOpen(path, mode);
}

void FS_InitDirectorySearchpath(searchpath_t* search, const char* path, int flags)
{
	memset(search, 0, sizeof(searchpath_t));

	Q_strncpy(search->filename, path, sizeof(search->filename));
	search->type = SEARCHPATH_PLAIN;
	search->flags = flags;
	search->pfnPrintInfo = FS_PrintInfo_DIR;
	search->pfnClose = FS_Close_DIR;
	search->pfnOpenFile = FS_OpenFile_DIR;
	search->pfnFileTime = FS_FileTime_DIR;
	search->pfnFindFile = FS_FindFile_DIR;
	search->pfnSearch = FS_Search_DIR;

	// create cache root
	search->pkg.dir = Mem_Malloc(fs_mempool, sizeof(dir_t));
	Q_strncpy(search->pkg.dir->name, search->filename, sizeof(search->pkg.dir->name));
	FS_PopulateDirEntries(search->pkg.dir, path);
}

searchpath_t* FS_AddDir_Fullpath(const char* path, qboolean* already_loaded, int flags)
{
	searchpath_t* search;

	for ( search = fs_searchpaths; search; search = search->next )
	{
		if ( search->type == SEARCHPATH_PLAIN && !Q_stricmp(search->filename, path) )
		{
			if ( already_loaded )
				*already_loaded = true;
			return search;
		}
	}

	if ( already_loaded )
		*already_loaded = false;

	search = (searchpath_t*)Mem_Calloc(fs_mempool, sizeof(searchpath_t));
	FS_InitDirectorySearchpath(search, path, flags);

	search->next = fs_searchpaths;
	fs_searchpaths = search;

	Con_Printf("Adding directory: %s\n", path);

	return search;
}
