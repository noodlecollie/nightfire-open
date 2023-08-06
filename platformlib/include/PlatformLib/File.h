#ifndef PLATFORMLIB_FILE_H
#define PLATFORMLIB_FILE_H

#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

size_t PlatformLib_LSeek(int fileHandle, size_t offset, int from);
int PlatformLib_Open(const char* filePath, int flags);
int PlatformLib_OpenWithPermissions(const char* filePath, int flags, int permissions);
int PlatformLib_Dup(int fileHandle);
int PlatformLib_Close(int fileHandle);
int PlatformLib_Read(int fileHandle, void* dstBuf, unsigned int maxCharCount);
int PlatformLib_Write(int fileHandle, const void* buf, unsigned int maxCharCount);
int PlatformLib_FileNo(FILE* file);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // PLATFORMLIB_FILE_H
