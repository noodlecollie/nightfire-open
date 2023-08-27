#ifndef FILELIB_H__
#define FILELIB_H__
#include "cmdlib.h" //--vluzacn

#if _MSC_VER >= 1000
#pragma once
#endif

typedef struct _DirectoryEntry
{
	char name[_MAX_PATH + 1];
	bool isDirectory;	// True = dir, false = file

	struct _DirectoryEntry* prev;
	struct _DirectoryEntry* next;
} DirectoryEntry_t;

extern time_t   getfiletime(const char* const filename);
extern long     getfilesize(const char* const filename);
extern long     getfiledata(const char* const filename, char* buffer, const int buffersize);
extern bool     q_exists(const char* const filename);
extern int      q_filelength(FILE* f);

extern FILE*    SafeOpenWrite(const char* const filename);
extern FILE*    SafeOpenRead(const char* const filename);
extern void     SafeRead(FILE* f, void* buffer, int count);
extern void     SafeWrite(FILE* f, const void* const buffer, int count);

extern int      LoadFile(const char* const filename, char** bufferptr);
extern void     SaveFile(const char* const filename, const void* const buffer, int count);

// new filesystem funcs
void FS_Init( void );
void FS_Shutdown( void );
void FS_FileBase( const char *in, char *out );
const char *FS_FileExtension( const char *in );
void FS_DefaultExtension( char *path, const char *extension );
void FS_ExtractFilePath( const char *path, char *dest );
const char *FS_FileWithoutPath( const char *in );
byte *FS_LoadFile( const char *path, long *filesizeptr, bool gamedironly );
bool FS_FileExists( const char *filename, bool gamedironly );
long FS_FileTime( const char *filename, bool gamedironly );

DirectoryEntry_t* FS_ListDirectory(const char* dirPath, size_t* count);
void FS_FreeDirectoryEntries(DirectoryEntry_t* entryList);

#endif //**/ FILELIB_H__
