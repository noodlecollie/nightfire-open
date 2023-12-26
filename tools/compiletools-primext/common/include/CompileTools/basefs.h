#pragma once

#include "BuildPlatform/Typedefs.h"

// normal file
typedef struct file_s file_t;

void FS_Init(const char* source);
byte* FS_LoadFile(const char* path, size_t* filesizeptr, bool gamedironly);
bool FS_FileExists(const char* filename, bool gamedironly);
void FS_Shutdown(void);
file_t* FS_Open(const char* filepath, const char* mode, bool gamedironly);
size_t FS_Read(file_t* file, void* buffer, size_t buffersize);
int FS_Write(file_t* file, const void* data, size_t datasize);
size_t FS_Tell(file_t* file);
int FS_Seek(file_t* file, int64_t offset, int whence);
int FS_Gets(file_t* file, byte* string, size_t bufsize);
void FS_AllowDirectPaths(bool enable);
size_t FS_FileLength(file_t* f);
int FS_Close(file_t* file);
