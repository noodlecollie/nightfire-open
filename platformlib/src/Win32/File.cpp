#include "PlatformLib/File.h"
#include <io.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

size_t PlatformLib_LSeek(int fileHandle, size_t offset, int from)
{
	return static_cast<size_t>(_lseek(fileHandle, static_cast<long>(offset), from));
}

int PlatformLib_Open(const char* filePath, int flags)
{
	return _open(filePath, flags);
}

int PlatformLib_OpenWithPermissions(const char* filePath, int flags, int permissions)
{
	return _open(filePath, flags, permissions);
}

int PlatformLib_Dup(int fileHandle)
{
	return _dup(fileHandle);
}

int PlatformLib_Close(int fileHandle)
{
	return _close(fileHandle);
}

int PlatformLib_Read(int fileHandle, void* dstBuf, unsigned int maxCharCount)
{
	return _read(fileHandle, dstBuf, maxCharCount);
}

int PlatformLib_Write(int fileHandle, const void* buf, unsigned int maxCharCount)
{
	return _write(fileHandle, buf, maxCharCount);
}

int PlatformLib_FileNo(FILE* file)
{
	return _fileno(file);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
