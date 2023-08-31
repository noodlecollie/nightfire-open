#ifndef PLATFORMLIB_FILE_H
#define PLATFORMLIB_FILE_H

#include <stddef.h>
#include <stdio.h>

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

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // PLATFORMLIB_FILE_H
