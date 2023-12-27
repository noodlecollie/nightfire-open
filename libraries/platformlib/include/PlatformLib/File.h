#ifndef PLATFORMLIB_FILE_H
#define PLATFORMLIB_FILE_H

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef _MSC_VER
// Defined away for portability.
// See https://stackoverflow.com/questions/2266992/no-o-binary-and-o-text-flags-in-linux
#define O_BINARY 0
#define O_TEXT 0
#endif

size_t PlatformLib_LSeek(int fileHandle, size_t offset, int from);
int PlatformLib_Open(const char* filePath, int flags);
FILE* PlatformLib_FOpen(const char* filename, const char* mode);
int PlatformLib_FClose(FILE* stream);
int PlatformLib_OpenWithPermissions(const char* filePath, int flags, int permissions);
int PlatformLib_Dup(int fileHandle);
int PlatformLib_Close(int fileHandle);
int PlatformLib_Read(int fileHandle, void* dstBuf, unsigned int maxCharCount);
int PlatformLib_Write(int fileHandle, const void* buf, unsigned int maxCharCount);
int PlatformLib_FileNo(FILE* file);
int PlatformLib_MkDir(const char* path);
int PlatformLib_Unlink(const char* filename);

// This is known to be unsafe, but some of the code uses it and it'd be a pain to
// rip out right now. Should be refactored in future.
int PlatformLib_VFScanF(FILE* stream, const char* format, va_list args);

static inline int PlatformLib_FScanF(FILE* stream, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	const int result = PlatformLib_VFScanF(stream, format, args);
	va_end(args);

	return result;
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // PLATFORMLIB_FILE_H
