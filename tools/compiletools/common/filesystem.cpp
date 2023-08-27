/*
filesystem.c - game filesystem based on DP fs
Copyright (C) 2007 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include <fcntl.h>
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#include <io.h>
#else
#include <dirent.h>
#define O_BINARY 0
#endif
#include <sys/stat.h>
#include <time.h>
#include "cmdlib.h"
#include "messages.h"
#include "log.h"
#include "stringlib.h"
#include "mathtypes.h"
#include "mathlib.h"
#include "blockmem.h"
#include "filelib.h"

/*
========================================================================
PAK FILES

The .pak files are just a linear collapse of a directory tree
========================================================================
*/
// header
#define IDPACKV1HEADER	(('K'<<24)+('C'<<16)+('A'<<8)+'P')	// little-endian "PACK"

#define MAX_SYSPATH		1024	// system filepath
#define MAX_FILES_IN_PACK	65536	// pak

typedef struct
{
	int		ident;
	int		dirofs;
	int		dirlen;
} dpackheader_t;

typedef struct
{
	char		name[56];		// total 64 bytes
	int		filepos;
	int		filelen;
} dpackfile_t;

#define FILE_COPY_SIZE		(1024 * 1024)
#define FILE_BUFF_SIZE		(65535)

// PAK errors
#define PAK_LOAD_OK			0
#define PAK_LOAD_COULDNT_OPEN		1
#define PAK_LOAD_BAD_HEADER		2
#define PAK_LOAD_BAD_FOLDERS		3
#define PAK_LOAD_TOO_MANY_FILES	4
#define PAK_LOAD_NO_FILES		5
#define PAK_LOAD_CORRUPTED		6

// filesystem flags
#define FS_GAMEDIR_PATH		1	// just a marker for gamedir path

typedef struct stringlist_s
{
	// maxstrings changes as needed, causing reallocation of strings[] array
	int		maxstrings;
	int		numstrings;
	char		**strings;
} stringlist_t;

typedef struct wadtype_s
{
	char		*ext;
	char		type;
} wadtype_t;

typedef struct file_s
{
	int		handle;			// file descriptor
	long		real_length;		// uncompressed file size (for files opened in "read" mode)
	long		position;			// current position in the file
	long		offset;			// offset into the package (0 if external file)
	int		ungetc;			// single stored character from ungetc, cleared to EOF when read
	time_t		filetime;			// pak, wad or real filetime
						// contents buffer
	long		buff_ind, buff_len;		// buffer current index and length
	byte		buff[FILE_BUFF_SIZE];	// intermediate buffer
} file_t;

typedef struct pack_s
{
	char		filename[256];
	int		handle;
	int		numfiles;
	time_t		filetime;	// common for all packed files
	dpackfile_t	*files;
} pack_t;

typedef struct searchpath_s
{
	char		filename[256];
	pack_t		*pack;
	int		flags;
	struct searchpath_s *next;
} searchpath_t;

searchpath_t		*fs_searchpaths = NULL;	// chain
searchpath_t		fs_directpath;		// static direct path
char			fs_rootdir[MAX_SYSPATH];	// engine root directory
char			fs_basedir[MAX_SYSPATH];	// base directory of game
char			fs_falldir[MAX_SYSPATH];	// game falling directory
char			fs_gamedir[MAX_SYSPATH];	// game current directory
char			gs_basedir[MAX_SYSPATH];	// initial dir before loading gameinfo.txt (used for compilers too)

const char *FS_FileExtension( const char *in );
static searchpath_t *FS_FindFile( const char *name, int *index, bool gamedironly );
static dpackfile_t* FS_AddFileToPack( const char* name, pack_t *pack, long offset, long size );
static bool FS_SysFileExists( const char *path );
static bool FS_SysFolderExists( const char *path );
static long FS_SysFileTime( const char *filename );
static void FS_Purge( file_t* file );

/*
=============================================================================

FILEMATCH COMMON SYSTEM

=============================================================================
*/
static int matchpattern( const char *str, const char *cmp, bool caseinsensitive )
{
	int	c1, c2;

	while( *cmp )
	{
		switch( *cmp )
		{
		case 0:   return 1; // end of pattern
		case '?': // match any single character
			if( *str == 0 || *str == '/' || *str == '\\' || *str == ':' )
				return 0; // no match
			str++;
			cmp++;
			break;
		case '*': // match anything until following string
			if( !*str ) return 1; // match
			cmp++;
			while( *str )
			{
				if( *str == '/' || *str == '\\' || *str == ':' )
					break;
				// see if pattern matches at this offset
				if( matchpattern( str, cmp, caseinsensitive ))
					return 1;
				// nope, advance to next offset
				str++;
			}
			break;
		default:
			if( *str != *cmp )
			{
				if( !caseinsensitive )
					return 0; // no match
				c1 = Q_tolower( *str );
				c2 = Q_tolower( *cmp );
				if( c1 != c2 ) return 0; // no match
			}

			str++;
			cmp++;
			break;
		}
	}

	// reached end of pattern but not end of input?
	return (*str) ? 0 : 1;
}

static void stringlistinit( stringlist_t *list )
{
	memset( list, 0, sizeof( *list ));
}

static void stringlistfreecontents( stringlist_t *list )
{
	int	i;

	for( i = 0; i < list->numstrings; i++ )
	{
		if( list->strings[i] )
			Free( list->strings[i] );
		list->strings[i] = NULL;
	}

	if( list->strings )
		Free( list->strings );

	list->numstrings = 0;
	list->maxstrings = 0;
	list->strings = NULL;
}

static void stringlistappend( stringlist_t *list, char *text )
{
	char	**oldstrings;
	size_t	textlen;

	if( !Q_stricmp( text, "." ) || !Q_stricmp( text, ".." ))
		return; // ignore the virtual directories

	if( list->numstrings >= list->maxstrings )
	{
		oldstrings = list->strings;
		list->maxstrings += 4096;
		list->strings = (char **)Alloc( list->maxstrings * sizeof( *list->strings ));
		if( list->numstrings ) memcpy( list->strings, oldstrings, list->numstrings * sizeof( *list->strings ));
		if( oldstrings ) Free( oldstrings );
	}

	textlen = Q_strlen( text ) + 1;
	list->strings[list->numstrings] = (char *)Alloc( textlen );
	memcpy( list->strings[list->numstrings], text, textlen );
	list->numstrings++;
}

static void stringlistsort( stringlist_t *list )
{
	char	*temp;
	int	i, j;

	// this is a selection sort (finds the best entry for each slot)
	for( i = 0; i < list->numstrings - 1; i++ )
	{
		for( j = i + 1; j < list->numstrings; j++ )
		{
			if( Q_strcmp( list->strings[i], list->strings[j] ) > 0 )
			{
				temp = list->strings[i];
				list->strings[i] = list->strings[j];
				list->strings[j] = temp;
			}
		}
	}
}

static void listdirectory( stringlist_t *list, const char *path)
{
    int		i;
    signed char *c;
#ifdef _WIN32
    char pattern[4096];
    struct _finddata_t	n_file;
    int		hFile;
#else
    DIR *dir;
    struct dirent *entry;
#endif

#ifdef _WIN32
    Q_snprintf( pattern, sizeof( pattern ), "%s*", path );

    // ask for the directory listing handle
    hFile = _findfirst( pattern, &n_file );
    if( hFile == -1 ) return;

    // start a new chain with the the first name
    stringlistappend( list, n_file.name );
    // iterate through the directory
    while( _findnext( hFile, &n_file ) == 0 )
	stringlistappend( list, n_file.name );
    _findclose( hFile );
#else
    if( !( dir = opendir( path ) ) )
	return;

    // iterate through the directory
    while( ( entry = readdir( dir ) ))
	stringlistappend( list, entry->d_name );
    closedir( dir );
#endif

    // convert names to lowercase because windows doesn't care, but pattern matching code often does
    //if( lowercase )
    {
	for( i = 0; i < list->numstrings; i++ )
	{
	    for( c = (signed char *)list->strings[i]; *c; c++ )
	    {
		if( *c >= 'A' && *c <= 'Z' )
		    *c += 'a' - 'A';
	    }
	}
    }
}
#if 0
static void listdirectory( stringlist_t *list, const char *path )
{
	char		pattern[4096];
	struct _finddata_t	n_file;
	long		hFile;
	char		*c;
	int		i;

	Q_strncpy( pattern, path, sizeof( pattern ));
	Q_strncat( pattern, "*", sizeof( pattern ));

	// ask for the directory listing handle
	hFile = _findfirst( pattern, &n_file );
	if( hFile == -1 ) return;

	// start a new chain with the the first name
	stringlistappend( list, n_file.name );
	// iterate through the directory
	while( _findnext( hFile, &n_file ) == 0 )
		stringlistappend( list, n_file.name );
	_findclose( hFile );

	// convert names to lowercase because windows doesn't care, but pattern matching code often does
	for( i = 0; i < list->numstrings; i++ )
	{
		for( c = list->strings[i]; *c; c++ )
			*c = Q_tolower( *c );
	}
}
#endif

/*
=============================================================================

OTHER PRIVATE FUNCTIONS

=============================================================================
*/
/*
====================
FS_AddFileToPack

Add a file to the list of files contained into a package
====================
*/
static dpackfile_t* FS_AddFileToPack( const char* name, pack_t* pack, long offset, long size )
{
	int		left, right, middle;
	dpackfile_t	*pfile;

	// look for the slot we should put that file into (binary search)
	left = 0;
	right = pack->numfiles - 1;

	while( left <= right )
	{
		int diff;

		middle = (left + right) / 2;
		diff = Q_stricmp( pack->files[middle].name, name );

		// If we found the file, there's a problem
		if( !diff ) Warning( "Package %s contains the file %s several times\n", pack->filename, name );

		// If we're too far in the list
		if( diff > 0 ) right = middle - 1;
		else left = middle + 1;
	}

	// We have to move the right of the list by one slot to free the one we need
	pfile = &pack->files[left];
	memmove( pfile + 1, pfile, (pack->numfiles - left) * sizeof( *pfile ));
	pack->numfiles++;

	Q_strncpy( pfile->name, name, sizeof( pfile->name ));
	pfile->filepos = offset;
	pfile->filelen = size;

	return pfile;
}

/*
============
FS_FileBase

Extracts the base name of a file (no path, no extension, assumes '/' as path separator)
============
*/
void FS_FileBase( const char *in, char *out )
{
	int	len, start, end;

	len = Q_strlen( in );
	if( !len ) return;

	// scan backward for '.'
	end = len - 1;

	while( end && in[end] != '.' && in[end] != '/' && in[end] != '\\' )
		end--;

	if( in[end] != '.' )
		end = len-1; // no '.', copy to end
	else end--; // found ',', copy to left of '.'


	// scan backward for '/'
	start = len - 1;

	while( start >= 0 && in[start] != '/' && in[start] != '\\' )
		start--;

	if( start < 0 || ( in[start] != '/' && in[start] != '\\' ))
		start = 0;
	else start++;

	// length of new sting
	len = end - start + 1;

	// Copy partial string
	Q_strncpy( out, &in[start], len + 1 );
	out[len] = 0;
}

/*
=================
FS_LoadPackPAK

Takes an explicit (not game tree related) path to a pak file.

Loads the header and directory, adding the files at the beginning
of the list so they override previous pack files.
=================
*/
pack_t *FS_LoadPackPAK( const char *packfile, int *error )
{
	dpackheader_t	header;
	int		packhandle;
	int		i, numpackfiles;
	pack_t		*pack;
	dpackfile_t	*info;

	packhandle = open( packfile, O_RDONLY|O_BINARY );

	if( packhandle < 0 )
	{
		Verbose( "%s couldn't open\n", packfile );
		if( error ) *error = PAK_LOAD_COULDNT_OPEN;
		return NULL;
	}

	read( packhandle, (void *)&header, sizeof( header ));

	if( header.ident != IDPACKV1HEADER )
	{
		Verbose( "%s is not a packfile. Ignored.\n", packfile );
		if( error ) *error = PAK_LOAD_BAD_HEADER;
		close( packhandle );
		return NULL;
	}

	if( header.dirlen % sizeof( dpackfile_t ))
	{
		Log( "Error: %s has an invalid directory size. Ignored.\n", packfile );
		if( error ) *error = PAK_LOAD_BAD_FOLDERS;
		close( packhandle );
		return NULL;
	}

	numpackfiles = header.dirlen / sizeof( dpackfile_t );

	if( numpackfiles > MAX_FILES_IN_PACK )
	{
		Log( "Error: %s has too many files ( %i ). Ignored.\n", packfile, numpackfiles );
		if( error ) *error = PAK_LOAD_TOO_MANY_FILES;
		close( packhandle );
		return NULL;
	}

	if( numpackfiles <= 0 )
	{
		Verbose( "%s has no files. Ignored.\n", packfile );
		if( error ) *error = PAK_LOAD_NO_FILES;
		close( packhandle );
		return NULL;
	}

	info = (dpackfile_t *)Alloc( sizeof( *info ) * numpackfiles );
	lseek( packhandle, header.dirofs, SEEK_SET );

	if( header.dirlen != read( packhandle, (void *)info, header.dirlen ))
	{
		Verbose( "%s is an incomplete PAK, not loading\n", packfile );
		if( error ) *error = PAK_LOAD_CORRUPTED;
		close( packhandle );
		Free( info );
		return NULL;
	}

	pack = (pack_t *)Alloc( sizeof( pack_t ));
	Q_strncpy( pack->filename, packfile, sizeof( pack->filename ));
	pack->files = (dpackfile_t *)Alloc( numpackfiles * sizeof( dpackfile_t ));
	pack->filetime = FS_SysFileTime( packfile );
	pack->handle = packhandle;
	pack->numfiles = 0;

	// parse the directory
	for( i = 0; i < numpackfiles; i++ )
		FS_AddFileToPack( info[i].name, pack, info[i].filepos, info[i].filelen );

	Verbose( "Adding packfile: %s (%i files)\n", packfile, numpackfiles );
	if( error ) *error = PAK_LOAD_OK;
	Free( info );

	return pack;
}

/*
================
FS_AddPak_Fullpath

Adds the given pack to the search path.
The pack type is autodetected by the file extension.

Returns true if the file was successfully added to the
search path or if it was already included.

If keep_plain_dirs is set, the pack will be added AFTER the first sequence of
plain directories.
================
*/
static bool FS_AddPak_Fullpath( const char *pakfile, bool *already_loaded, bool keep_plain_dirs, int flags )
{
	searchpath_t	*search;
	pack_t		*pak = NULL;
	const char	*ext = FS_FileExtension( pakfile );
	int		errorcode = PAK_LOAD_COULDNT_OPEN;

	for( search = fs_searchpaths; search; search = search->next )
	{
		if( search->pack && !Q_stricmp( search->pack->filename, pakfile ))
		{
			if( already_loaded ) *already_loaded = true;
			return true; // already loaded
		}
	}

	if( already_loaded ) *already_loaded = false;

	if( !Q_stricmp( ext, "pak" )) pak = FS_LoadPackPAK( pakfile, &errorcode );
	else Log( "Error: \"%s\" does not have a pack extension\n", pakfile );

	if( pak )
	{
		if( keep_plain_dirs )
		{
			// find the first item whose next one is a pack or NULL
			searchpath_t *insertion_point = NULL;
			if( fs_searchpaths && !fs_searchpaths->pack )
			{
				insertion_point = fs_searchpaths;
				while( 1 )
				{
					if( !insertion_point->next ) break;
					if( insertion_point->next->pack ) break;
					insertion_point = insertion_point->next;
				}
			}

			// if insertion_point is NULL, this means that either there is no
			// item in the list yet, or that the very first item is a pack. In
			// that case, we want to insert at the beginning...
			if( !insertion_point )
			{
				search = (searchpath_t *)Alloc( sizeof( searchpath_t ));
				search->pack = pak;
				search->next = fs_searchpaths;
				search->flags |= flags;
				fs_searchpaths = search;
			}
			else // otherwise we want to append directly after insertion_point.
			{
				search = (searchpath_t *)Alloc( sizeof( searchpath_t ));
				search->pack = pak;
				search->next = insertion_point->next;
				search->flags |= flags;
				insertion_point->next = search;
			}
		}
		else
		{
			search = (searchpath_t *)Alloc( sizeof( searchpath_t ));
			search->pack = pak;
			search->next = fs_searchpaths;
			search->flags |= flags;
			fs_searchpaths = search;
		}

		return true;
	}
	else
	{
		if( errorcode != PAK_LOAD_NO_FILES )
			Log( "Error: FS_AddPak_Fullpath: unable to load pak \"%s\"\n", pakfile );
		return false;
	}
}

/*
================
FS_AddGameDirectory

Sets fs_gamedir, adds the directory to the head of the path,
then loads and adds pak1.pak pak2.pak ...
================
*/
void FS_AddGameDirectory( const char *dir, int flags )
{
	stringlist_t	list;
	searchpath_t	*search;
	char		fullpath[256];
	int		i;

	stringlistinit( &list );
	listdirectory( &list, dir );
	stringlistsort( &list );

	// add any PAK package in the directory
	for( i = 0; i < list.numstrings; i++ )
	{
		if( !Q_stricmp( FS_FileExtension( list.strings[i] ), "pak" ))
		{
			Q_sprintf( fullpath, "%s%s", dir, list.strings[i] );
			FS_AddPak_Fullpath( fullpath, NULL, false, flags );
		}
	}

	stringlistfreecontents( &list );

	// add the directory to the search path
	// (unpacked files have the priority over packed files)
	search = (searchpath_t *)Alloc( sizeof( searchpath_t ));
	Q_strncpy( search->filename, dir, sizeof ( search->filename ));
	search->next = fs_searchpaths;
	search->flags = flags;
	fs_searchpaths = search;


}

/*
================
FS_AddGameHierarchy
================
*/
void FS_AddGameHierarchy( const char *dir, int flags )
{
	// Add the common game directory
	if( dir && *dir ) FS_AddGameDirectory( va( "%s%s/", fs_rootdir, dir ), flags );
}

/*
============
FS_FileExtension
============
*/
const char *FS_FileExtension( const char *in )
{
	const char *separator, *backslash, *colon, *dot;

	separator = Q_strrchr( in, '/' );
	backslash = Q_strrchr( in, '\\' );

	if( !separator || separator < backslash )
		separator = backslash;

	colon = Q_strrchr( in, ':' );

	if( !separator || separator < colon )
		separator = colon;

	dot = Q_strrchr( in, '.' );

	if( dot == NULL || ( separator && ( dot < separator )))
		return "";

	return dot + 1;
}

/*
============
FS_FileWithoutPath
============
*/
const char *FS_FileWithoutPath( const char *in )
{
	const char *separator, *backslash, *colon;

	separator = Q_strrchr( in, '/' );
	backslash = Q_strrchr( in, '\\' );

	if( !separator || separator < backslash )
		separator = backslash;

	colon = Q_strrchr( in, ':' );

	if( !separator || separator < colon )
		separator = colon;

	return separator ? separator + 1 : in;
}

/*
============
FS_ExtractFilePath
============
*/
void FS_ExtractFilePath( const char *path, char *dest )
{
	const char *src = path + Q_strlen( path ) - 1;

	// back up until a \ or the start
	while( src != path && !(*(src - 1) == '\\' || *(src - 1) == '/' ))
		src--;

	if( src != path )
	{
		memcpy( dest, path, src - path );
		dest[src - path - 1] = 0; // cutoff backslash
	}
	else Q_strcpy( dest, "" ); // file without path
}

/*
================
FS_ClearSearchPath
================
*/
void FS_ClearSearchPath( void )
{
	while( fs_searchpaths )
	{
		searchpath_t	*search = fs_searchpaths;

		if( !search ) break;

		fs_searchpaths = search->next;

		if( search->pack )
		{
			if( search->pack->files )
				Free( search->pack->files );
			Free( search->pack );
		}
		Free( search );
	}

	fs_searchpaths = NULL;
}

/*
================
FS_ParseLiblistGam
================
*/
static bool FS_ParseLiblistGam( const char *filename, const char *gamedir )
{
	char	*afile, *pfile;
	char	token[256];

	afile = (char *)FS_LoadFile( filename, NULL, false );
	if( !afile ) return false;

	Q_strncpy( fs_basedir, "valve", sizeof( fs_basedir ));
	FS_FileBase( gamedir, fs_gamedir );

	pfile = afile;

	while(( pfile = COM_ParseFile( pfile, token )) != NULL )
	{
		if( !Q_stricmp( token, "gamedir" ))
		{
			pfile = COM_ParseFile( pfile, token );
			if( Q_stricmp( token, fs_basedir ) || Q_stricmp( token, fs_gamedir ))
				Q_strncpy( fs_gamedir, token, sizeof( fs_gamedir ));
		}
		if( !Q_stricmp( token, "fallback_dir" ))
		{
			pfile = COM_ParseFile( pfile, token );
			if( Q_stricmp( token, fs_basedir ) || Q_stricmp( token, fs_falldir ))
				Q_strncpy( fs_falldir, token, sizeof( fs_falldir ));
		}
	}

	if( afile != NULL )
		Free( afile );

	return true;
}

/*
================
FS_ParseGameInfo
================
*/
static bool FS_ParseGameInfo( const char *filename, const char *gamedir )
{
	char	*afile, *pfile;
	char	token[256];

	afile = (char *)FS_LoadFile( filename, NULL, false );
	if( !afile ) return false;

	// setup default values
	FS_FileBase( gamedir, fs_gamedir );

	pfile = afile;

	while(( pfile = COM_ParseFile( pfile, token )) != NULL )
	{
		if( !Q_stricmp( token, "basedir" ))
		{
			pfile = COM_ParseFile( pfile, token );
			if( Q_stricmp( token, fs_basedir ) || Q_stricmp( token, fs_gamedir ))
				Q_strncpy( fs_basedir, token, sizeof( fs_basedir ));
		}
		else if( !Q_stricmp( token, "fallback_dir" ))
		{
			pfile = COM_ParseFile( pfile, token );
			if( Q_stricmp( token, fs_basedir ) || Q_stricmp( token, fs_falldir ))
				Q_strncpy( fs_falldir, token, sizeof( fs_falldir ));
		}
		else if( !Q_stricmp( token, "gamedir" ))
		{
			pfile = COM_ParseFile( pfile, token );
			if( Q_stricmp( token, fs_basedir ) || Q_stricmp( token, fs_gamedir ))
				Q_strncpy( fs_gamedir, token, sizeof( fs_gamedir ));
		}
	}

	if( afile != NULL )
		Free( afile );

	return true;
}

void FS_ReadGameInfo( const char *gamedir )
{
	char	liblist[256], gameinfo[256];

	Q_strncpy( gameinfo, "gameinfo.txt", sizeof( gameinfo ));
	Q_strncpy( liblist, "liblist.gam", sizeof( liblist ));

	// if user change liblist.gam update the gameinfo.txt
	if( FS_FileTime( liblist, false ) > FS_FileTime( gameinfo, false ))
		FS_ParseLiblistGam( liblist, gamedir );
	else FS_ParseGameInfo( gameinfo, gamedir );
}

/*
================
FS_Rescan
================
*/
void FS_Rescan( void )
{
	FS_ClearSearchPath();

	if( Q_stricmp( fs_basedir, fs_gamedir ))
		FS_AddGameHierarchy( fs_basedir, 0 );
	if( Q_stricmp( fs_basedir, fs_falldir ) && Q_stricmp( fs_gamedir, fs_falldir ))
		FS_AddGameHierarchy( fs_falldir, 0 );
	FS_AddGameHierarchy( fs_gamedir, FS_GAMEDIR_PATH );
}

/*
================
FS_LoadGameInfo

can be passed null arg
================
*/
void FS_LoadGameInfo( const char *rootfolder )
{
	int	i;

	Q_strcpy( gs_basedir, rootfolder );
	FS_Rescan(); // create new filesystem
}

/*
================
FS_ExpandArg
================
*/
char *FS_ExpandArg( const char *path )
{
	static char	full[1024];

	if( path[0] != '/' && path[0] != '\\' && path[1] != ':' )
	{
		Q_getwd( full, sizeof( full ));
		Q_strncat( full, path, sizeof( full ));
	}
	else Q_strncpy( full, path, sizeof( full ));

	return full;
}

/*
================
FS_Init
================
*/
void FS_Init( void )
{
	char	workdir[_MAX_PATH];
	char	mapdir[_MAX_PATH];
	char	*pathend;

	fs_searchpaths = NULL;

	// skip the unneeded separator
	if( g_Mapname[0] == '.' && g_Mapname[1] == '/' || g_Mapname[1] == '\\' )
		Q_snprintf( mapdir, sizeof( mapdir ), "%s.map", g_Mapname + 2 );
	else Q_snprintf( mapdir, sizeof( mapdir ), "%s.map", g_Mapname );

	// create full path to a map
	char* mapDirExpanded = FS_ExpandArg( mapdir );
	Q_strncpy( workdir, mapDirExpanded, sizeof( workdir ));

	// search for 'maps' in path
	pathend = Q_stristr( workdir, "maps" );

	if( !pathend )
	{
		Log( "Error: FS_Init: couldn't init game directory from proposed directory %s\n", workdir );
		return;
	}

	// create gamedir path
	Q_strncpy( fs_rootdir, workdir, pathend - workdir );

	Log( "workdir: %s\n", fs_rootdir );

	FS_AddGameDirectory( va( "%s\\", fs_rootdir ), 0 );
	FS_ReadGameInfo( fs_rootdir );

	Log( "gamedir: %s, basedir %s, falldir %s\n", fs_gamedir, fs_basedir, fs_falldir );
	ExtractFilePath( fs_rootdir, fs_rootdir );

	if( !Q_strcmp( fs_rootdir, "" ))
		Q_strncpy( fs_rootdir, "..\\", sizeof( fs_rootdir ));

	Log( "rootdir %s\n", fs_rootdir );
	FS_Rescan(); // create new filesystem

	Verbose( "FS_Init: done\n" );

	searchpath_t	*s;

	Log( "Current search path:\n" );
	for( s = fs_searchpaths; s; s = s->next )
	{
		if( s->pack ) Log( "%s (%i files)\n", s->pack->filename, s->pack->numfiles );
		else Log( "%s\n", s->filename );
	}
}

/*
================
FS_Shutdown
================
*/
void FS_Shutdown( void )
{
	FS_ClearSearchPath();
}

/*
====================
FS_SysFileTime

Internal function used to determine filetime
====================
*/
static long FS_SysFileTime( const char *filename )
{
	struct stat buf;

	if( stat( filename, &buf ) == -1 )
		return -1;

	return buf.st_mtime;
}

/*
====================
FS_SysOpen

Internal function used to create a file_t and open the relevant non-packed file on disk
====================
*/
static file_t* FS_SysOpen( const char* filepath, const char* mode )
{
	file_t		*file;
	int		mod, opt;
	unsigned int	ind;

	// Parse the mode string
	switch( mode[0] )
	{
	case 'r':	// read
		mod = O_RDONLY;
		opt = 0;
		break;
	case 'w': // write
		mod = O_WRONLY;
		opt = O_CREAT | O_TRUNC;
		break;
	case 'a': // append
		mod = O_WRONLY;
		opt = O_CREAT | O_APPEND;
		break;
	case 'e': // edit
		mod = O_WRONLY;
		opt = O_CREAT;
		break;
	default:
		Log( "Error: FS_SysOpen(%s, %s): invalid mode\n", filepath, mode );
		return NULL;
	}

	for( ind = 1; mode[ind] != '\0'; ind++ )
	{
		switch( mode[ind] )
		{
		case '+':
			mod = O_RDWR;
			break;
		case 'b':
			opt |= O_BINARY;
			break;
		default:
			Log( "Error: FS_SysOpen: %s: unknown char in mode (%c)\n", filepath, mode[ind] );
			break;
		}
	}

	file = (file_t *)Alloc( sizeof( *file ));
	file->filetime = FS_SysFileTime( filepath );
	file->ungetc = EOF;

	file->handle = open( filepath, mod|opt, 0666 );
	if( file->handle < 0 )
	{
		Free( file );
		return NULL;
	}

	file->real_length = lseek( file->handle, 0, SEEK_END );

	// For files opened in append mode, we start at the end of the file
	if( mod & O_APPEND ) file->position = file->real_length;
	else lseek( file->handle, 0, SEEK_SET );

	return file;
}


/*
===========
FS_OpenPackedFile

Open a packed file using its package file descriptor
===========
*/
file_t *FS_OpenPackedFile( pack_t *pack, int pack_ind )
{
	dpackfile_t	*pfile;
	int		dup_handle;
	file_t		*file;

	pfile = &pack->files[pack_ind];

	if( lseek( pack->handle, pfile->filepos, SEEK_SET ) == -1 )
		return NULL;

	dup_handle = dup( pack->handle );

	if( dup_handle < 0 )
		return NULL;

	file = (file_t *)Alloc( sizeof( *file ));
	file->handle = dup_handle;
	file->real_length = pfile->filelen;
	file->offset = pfile->filepos;
	file->position = 0;
	file->ungetc = EOF;

	return file;
}

/*
==================
FS_SysFileExists

Look for a file in the filesystem only
==================
*/
bool FS_SysFileExists( const char *path )
{
	int desc;

	if(( desc = open( path, O_RDONLY|O_BINARY )) < 0 )
		return false;

	close( desc );
	return true;
}

/*
==================
FS_SysFolderExists

Look for a existing folder
==================
*/
bool FS_SysFolderExists( const char *path )
{
#ifdef _WIN32
    DWORD	dwFlags = GetFileAttributes( path );

    return ( dwFlags != -1 ) && ( dwFlags & FILE_ATTRIBUTE_DIRECTORY );
#else
    DIR *dir = opendir(path);

    if(dir)
    {
	closedir(dir);
	return 1;
    }
    else if((errno == ENOENT) || (errno == ENOTDIR))
    {
	return 0;
    }
    else
    {
	return 0;
    }
#endif
}

/*
====================
FS_FindFile

Look for a file in the packages and in the filesystem

Return the searchpath where the file was found (or NULL)
and the file index in the package if relevant
====================
*/
static searchpath_t *FS_FindFile( const char *name, int* index, bool gamedironly )
{
	searchpath_t	*search;
	char		*pEnvPath;
	pack_t		*pak;

	// search through the path, one element at a time
	for( search = fs_searchpaths; search; search = search->next )
	{
		if( gamedironly & !( search->flags & FS_GAMEDIR_PATH ))
			continue;

		// is the element a pak file?
		if( search->pack )
		{
			int left, right, middle;

			pak = search->pack;

			// look for the file (binary search)
			left = 0;
			right = pak->numfiles - 1;
			while( left <= right )
			{
				int diff;

				middle = (left + right) / 2;
				diff = Q_stricmp( pak->files[middle].name, name );

				// Found it
				if( !diff )
				{
					if( index ) *index = middle;
					return search;
				}

				// if we're too far in the list
				if( diff > 0 )
					right = middle - 1;
				else left = middle + 1;
			}
		}
		else
		{
			char	netpath[MAX_SYSPATH];
			Q_sprintf( netpath, "%s%s", search->filename, name );
			if( FS_SysFileExists( netpath ))
			{
				if( index != NULL ) *index = -1;
				return search;
			}
		}
	}

	if( index != NULL )
		*index = -1;

	return NULL;
}


/*
===========
FS_OpenReadFile

Look for a file in the search paths and open it in read-only mode
===========
*/
file_t *FS_OpenReadFile( const char *filename, const char *mode, bool gamedironly )
{
	searchpath_t	*search;
	int		pack_ind;

	search = FS_FindFile( filename, &pack_ind, gamedironly );

	// not found?
	if( search == NULL )
		return NULL;

	if( search->pack )
		return FS_OpenPackedFile( search->pack, pack_ind );
	else if( pack_ind < 0 )
	{
		char	path [MAX_SYSPATH];

		// found in the filesystem?
		Q_sprintf( path, "%s%s", search->filename, filename );
		return FS_SysOpen( path, mode );
	}
	return NULL;
}

/*
=============================================================================

MAIN PUBLIC FUNCTIONS

=============================================================================
*/
/*
====================
FS_Open

Open a file. The syntax is the same as fopen
====================
*/
file_t *FS_Open( const char *filepath, const char *mode, bool gamedironly )
{
	// some stupid mappers used leading '/' or '\' in path to models or sounds
	if( filepath[0] == '/' || filepath[0] == '\\' ) filepath++;
	if( filepath[0] == '/' || filepath[0] == '\\' ) filepath++;

	if( mode[0] == 'w' || mode[0] == 'a'|| mode[0] == 'e' || Q_strchr( mode, '+' ))
		Error( "FS_Open: trying to open for a write file\n" );

	// else, we look at the various search paths and open the file in read-only mode
	return FS_OpenReadFile( filepath, mode, gamedironly );
}

/*
====================
FS_Close

Close a file
====================
*/
int FS_Close( file_t *file )
{
	if( close( file->handle ))
		return EOF;

	Free( file );
	return 0;
}

/*
====================
FS_Read

Read up to "buffersize" bytes from a file
====================
*/
long FS_Read( file_t *file, void *buffer, size_t buffersize )
{
	long	count, done;
	long	nb;

	// nothing to copy
	if( buffersize == 0 ) return 1;

	// Get rid of the ungetc character
	if( file->ungetc != EOF )
	{
		((char*)buffer)[0] = file->ungetc;
		buffersize--;
		file->ungetc = EOF;
		done = 1;
	}
	else done = 0;

	// first, we copy as many bytes as we can from "buff"
	if( file->buff_ind < file->buff_len )
	{
		count = file->buff_len - file->buff_ind;

		done += ((long)buffersize > count ) ? count : (long)buffersize;
		memcpy( buffer, &file->buff[file->buff_ind], done );
		file->buff_ind += done;

		buffersize -= done;
		if( buffersize == 0 )
			return done;
	}

	// NOTE: at this point, the read buffer is always empty

	// we must take care to not read after the end of the file
	count = file->real_length - file->position;

	// if we have a lot of data to get, put them directly into "buffer"
	if( buffersize > sizeof( file->buff ) / 2 )
	{
		if( count > (long)buffersize )
			count = (long)buffersize;
		lseek( file->handle, file->offset + file->position, SEEK_SET );
		nb = read (file->handle, &((byte *)buffer)[done], count );

		if( nb > 0 )
		{
			done += nb;
			file->position += nb;
			// purge cached data
			FS_Purge( file );
		}
	}
	else
	{
		if( count > (long)sizeof( file->buff ))
			count = (long)sizeof( file->buff );
		lseek( file->handle, file->offset + file->position, SEEK_SET );
		nb = read( file->handle, file->buff, count );

		if( nb > 0 )
		{
			file->buff_len = nb;
			file->position += nb;

			// copy the requested data in "buffer" (as much as we can)
			count = (long)buffersize > file->buff_len ? file->buff_len : (long)buffersize;
			memcpy( &((byte *)buffer)[done], file->buff, count );
			file->buff_ind = count;
			done += count;
		}
	}

	return done;
}

/*
====================
FS_Seek

Move the position index in a file
====================
*/
int FS_Seek( file_t *file, long offset, int whence )
{
	// compute the file offset
	switch( whence )
	{
	case SEEK_CUR:
		offset += file->position - file->buff_len + file->buff_ind;
		break;
	case SEEK_SET:
		break;
	case SEEK_END:
		offset += file->real_length;
		break;
	default:
		return -1;
	}

	if( offset < 0 || offset > file->real_length )
		return -1;

	// if we have the data in our read buffer, we don't need to actually seek
	if( file->position - file->buff_len <= offset && offset <= file->position )
	{
		file->buff_ind = offset + file->buff_len - file->position;
		return 0;
	}

	// Purge cached data
	FS_Purge( file );

	if( lseek( file->handle, file->offset + offset, SEEK_SET ) == -1 )
		return -1;
	file->position = offset;

	return 0;
}

/*
====================
FS_Tell

Give the current position in a file
====================
*/
long FS_Tell( file_t* file )
{
	if( !file ) return 0;
	return file->position - file->buff_len + file->buff_ind;
}

/*
====================
FS_Eof

indicates at reached end of file
====================
*/
bool FS_Eof( file_t* file )
{
	if( !file ) return true;
	return (( file->position - file->buff_len + file->buff_ind ) == file->real_length ) ? true : false;
}

/*
====================
FS_Purge

Erases any buffered input or output data
====================
*/
void FS_Purge( file_t* file )
{
	file->buff_len = 0;
	file->buff_ind = 0;
	file->ungetc = EOF;
}

/*
============
FS_LoadFile

Filename are relative to the xash directory.
Always appends a 0 byte.
============
*/
byte *FS_LoadFile( const char *path, long *filesizeptr, bool gamedironly )
{
	file_t	*file;
	byte	*buf = NULL;
	long	filesize = 0;

	file = FS_Open( path, "rb", gamedironly );

	if( file )
	{
		filesize = file->real_length;
		buf = (byte *)Alloc( filesize + 1 );
		buf[filesize] = '\0';
		FS_Read( file, buf, filesize );
		FS_Close( file );
	}

	if( filesizeptr )
		*filesizeptr = filesize;

	return buf;
}

/*
=============================================================================

OTHERS PUBLIC FUNCTIONS

=============================================================================
*/
/*
============
FS_StripExtension
============
*/
void FS_StripExtension( char *path )
{
	size_t	length;

	length = Q_strlen( path ) - 1;
	while( length > 0 && path[length] != '.' )
	{
		length--;
		if( path[length] == '/' || path[length] == '\\' || path[length] == ':' )
			return; // no extension
	}
	if( length ) path[length] = 0;
}

/*
==================
FS_DefaultExtension
==================
*/
void FS_DefaultExtension( char *path, const char *extension )
{
	const char *src;

	// if path doesn't have a .EXT, append extension
	// (extension should include the .)
	src = path + Q_strlen( path ) - 1;

	while( *src != '/' && src != path )
	{
		// it has an extension
		if( *src == '.' ) return;
		src--;
	}
	Q_strcat( path, extension );
}

/*
==================
FS_FileExists

Look for a file in the packages and in the filesystem
==================
*/
bool FS_FileExists( const char *filename, bool gamedironly )
{
	if( FS_FindFile( filename, NULL, gamedironly ))
		return true;
	return false;
}

/*
==================
FS_GetDiskPath

Build direct path for file in the filesystem
return NULL for file in pack
==================
*/
const char *FS_GetDiskPath( const char *name, bool gamedironly )
{
	int		index;
	searchpath_t	*search;

	search = FS_FindFile( name, &index, gamedironly );

	if( search )
	{
		if( index != -1 ) // file in pack or wad
			return NULL;
		return va( "%s%s", search->filename, name );
	}

	return NULL;
}

/*
==================
FS_FileSize

return size of file in bytes
==================
*/
long FS_FileSize( const char *filename, bool gamedironly )
{
	file_t	*fp;
	int	length = 0;

	fp = FS_Open( filename, "rb", gamedironly );

	if( fp )
	{
		// it exists
		FS_Seek( fp, 0, SEEK_END );
		length = FS_Tell( fp );
		FS_Close( fp );
	}
	return length;
}

/*
==================
FS_FileLength

return size of file in bytes
==================
*/
long FS_FileLength( file_t *f )
{
	if( !f ) return 0;
	return f->real_length;
}

/*
==================
FS_FileTime

return time of creation file in seconds
==================
*/
long FS_FileTime( const char *filename, bool gamedironly )
{
	searchpath_t	*search;
	int		pack_ind;

	search = FS_FindFile( filename, &pack_ind, gamedironly );
	if( !search ) return -1; // doesn't exist

	if( search->pack ) // grab pack filetime
		return search->pack->filetime;
	else if( pack_ind < 0 )
	{
		char	path [MAX_SYSPATH];

		// found in the filesystem?
		Q_sprintf( path, "%s%s", search->filename, filename );
		return FS_SysFileTime( path );
	}

	return -1; // doesn't exist
}

static DirectoryEntry_t* CreateNewDirectoryEntry(const char* name, bool isDirectory)
{
	DirectoryEntry_t* newItem = (DirectoryEntry_t*)Alloc(sizeof(DirectoryEntry_t));
	memset(newItem, 0, sizeof(*newItem));

	Q_strncpy(newItem->name, name, sizeof(newItem->name));
	newItem->isDirectory = isDirectory;

	return newItem;
}

static void AppendEntryToList(DirectoryEntry_t* newItem, DirectoryEntry_t*& listHead, DirectoryEntry_t*& lastCreated)
{
	if ( !listHead )
	{
		listHead = newItem;
	}
	else
	{
		newItem->prev = lastCreated;
		lastCreated->next = newItem;
	}

	lastCreated = newItem;
}

DirectoryEntry_t* FS_ListDirectory(const char* dirPath, size_t* count)
{
	DirectoryEntry_t* list = NULL;
	DirectoryEntry_t* lastCreated = NULL;

#ifndef _WIN32
	DIR* directory = opendir(dirPath);

	if ( directory )
	{
		for ( struct dirent* entry = readdir(directory); entry; entry = readdir(directory) )
		{
			DirectoryEntry_t* newItem = CreateNewDirectoryEntry(entry->d_name, entry->d_type == DT_DIR);
			AppendEntryToList(newItem, list, lastCreated);

			if ( count )
			{
				++(*count);
			}
		}

		closedir(directory);
	}
#else
	// This is slightly duplicated from listdirectory(), but
	// we need to be more flexible than that function
	// (we need to check item attributes).
	char pattern[4096];
	pattern[0] = '\0';

	Q_strncpy(pattern, dirPath, sizeof(pattern));
	size_t length = strlen(pattern);

	if ( length > 0 && pattern[length - 1] == '\\' )
	{
		pattern[length - 1] = '\0';
	}

	Q_strncat(pattern, "\\*", sizeof(pattern));
	pattern[sizeof(pattern) - 1] = '\0';

	struct _finddata_t fileData = { 0 };
	intptr_t fileHandle = _findfirst(pattern, &fileData);

	if( fileHandle != -1 )
	{
		do
		{
			DirectoryEntry_t* newItem = CreateNewDirectoryEntry(fileData.name, (fileData.attrib & _A_SUBDIR) != 0);
			AppendEntryToList(newItem, list, lastCreated);

			if ( count )
			{
				++(*count);
			}
		}
		while ( _findnext(fileHandle, &fileData) == 0 );

		_findclose(fileHandle);
	}

#endif

	return list;
}

void FS_FreeDirectoryEntries(DirectoryEntry_t* entryList)
{
	while ( entryList )
	{
		DirectoryEntry_t* next = entryList->next;
		Free(entryList);
		entryList = next;
	}
}
