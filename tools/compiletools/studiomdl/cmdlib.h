#ifndef __CMDLIB__
#define __CMDLIB__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#include "win32fix.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __CMDUTIL__
#define __CMDUTIL__
#define _NOENUMQBOOL
#ifndef _NOENUMQBOOL
typedef enum {false, true} qboolean;
#else
typedef int qboolean;
#undef true
#undef false
#define true 1
#define false 0
#endif

typedef unsigned char byte;
#endif

// the dec offsetof macro doesn't work very well...
#define myoffsetof(type,identifier) ((size_t)&((type *)0)->identifier)


// set these before calling CheckParm
extern int myargc;
extern char **myargv;

char *strupper (char *in);
char *strlower (char *in);
int Q_strncasecmp (char *s1, char *s2, int n);
int Q_strcasecmp (char *s1, char *s2);
void Q_getwd (char *out, size_t length);
qboolean CDECL FORMAT_PRINTF(3,4) safe_snprintf(char* const dest, const size_t count, const char* const args, ...);
qboolean safe_strncpy(char* const dest, const char* const src, const size_t count);
qboolean safe_strncat(char* const dest, const char* const src, const size_t count);

qboolean IsFileOrDirectory(const char* path);

// Ensures that all characters in the path are correctly capitalised
// as per the actual file and directory names on the system.
// If the path does not exist, returns false; otherwise, returns true.
// Assumes path separators are "/", not "\"
qboolean MakePathCorrectCase(char* buffer);

int filelength(FILE *f);
int	FileTime(const char* path);

void	Q_mkdir (char *path);

extern	char		qdir[1024];
extern	char		gamedir[1024];
void SetQdirFromPath (char *path);
char *ExpandArg (char *path);	// from cmd line
char *ExpandPath (char *path);	// from scripts
char *ExpandPathAndArchive (char *path);


double I_FloatTime (void);

void	Error (const char *error, ...);
int		CheckParm (char *check);

FILE	*SafeOpenWrite (char *filename);
FILE	*SafeOpenRead (char *filename);
void	SafeRead (FILE *f, void *buffer, int count);
void	SafeWrite (FILE *f, void *buffer, int count);

int		LoadFile (char *filename, void **bufferptr);
void	SaveFile (char *filename, void *buffer, int count);

void 	DefaultExtension (char *path, const char *extension);
void 	DefaultPath (char *path, char *basepath);
void 	StripFilename (char *path);
void 	StripExtension (char *path);

void 	ExtractFilePath (char *path, char *dest);
void 	ExtractFileBase (char *path, char *dest);
void	ExtractFileExtension (const char *path, char *dest);

int 	ParseNum (char *str);

short	BigShort (short l);
short	LittleShort (short l);
int		BigLong (int l);
int		LittleLong (int l);
float	BigFloat (float l);
float	LittleFloat (float l);

long flen(FILE* f);



char *COM_Parse (char *data);

extern	char		com_token[1024];
extern	qboolean	com_eof;

char *copystring(const char *s);


void CRC_Init(unsigned short *crcvalue);
void CRC_ProcessByte(unsigned short *crcvalue, byte data);
unsigned short CRC_Value(unsigned short crcvalue);

void	CreatePath (char *path);
void	QCopyFile (char *from, char *to);

extern	qboolean		archive;
extern	char			archivedir[1024];


extern	qboolean verbose;
void qprintf (char *format, ...);


typedef struct
{
	char	name[56];
	int		filepos, filelen;
} packfile_t;

typedef struct
{
	char	id[4];
	int		dirofs;
	int		dirlen;
} packheader_t;


void ListPak(char* pakname);

#endif
#ifdef __cplusplus
}
#endif
