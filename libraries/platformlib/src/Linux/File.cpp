#include "PlatformLib/File.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

size_t PlatformLib_LSeek(int fileHandle, size_t offset, int from)
{
	return lseek(fileHandle, offset, from);
}

int PlatformLib_Open(const char* filePath, int flags)
{
	return open(filePath, flags, 0666);
}

FILE* PlatformLib_FOpen(const char* filename, const char* mode)
{
	return fopen(filename, mode);
}

int PlatformLib_FClose(FILE* stream)
{
	return fclose(stream);
}

int PlatformLib_OpenWithPermissions(const char* filePath, int flags, int permissions)
{
	return open(filePath, flags, permissions);
}

int PlatformLib_Dup(int fileHandle)
{
	return dup(fileHandle);
}

int PlatformLib_Close(int fileHandle)
{
	return close(fileHandle);
}

int PlatformLib_Read(int fileHandle, void* dstBuf, unsigned int maxCharCount)
{
	return read(fileHandle, dstBuf, maxCharCount);
}

int PlatformLib_Write(int fileHandle, const void* buf, unsigned int maxCharCount)
{
	return write(fileHandle, buf, maxCharCount);
}

int PlatformLib_FileNo(FILE* file)
{
	return fileno(file);
}

int PlatformLib_MkDir(const char* path)
{
	return mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

int PlatformLib_Unlink(const char* filename)
{
	return unlink(filename);
}

int PlatformLib_VFScanF(FILE* stream, const char* format, va_list args)
{
	return vfscanf(stream, format, args);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
