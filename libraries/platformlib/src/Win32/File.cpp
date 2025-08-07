// For vfscanf:
#define _CRT_SECURE_NO_WARNINGS

#include "PlatformLib/File.h"
#include <io.h>
#include <sys/stat.h>
#include <direct.h>

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
	return PlatformLib_OpenWithPermissions(filePath, flags, _S_IREAD | _S_IWRITE);
}

FILE* PlatformLib_FOpen(const char* filename, const char* mode)
{
	FILE* outPtr = nullptr;
	errno_t result = fopen_s(&outPtr, filename, mode);
	return result == 0 ? outPtr : nullptr;
}

int PlatformLib_FClose(FILE* stream)
{
	return fclose(stream);
}

int PlatformLib_OpenWithPermissions(const char* filePath, int flags, int permissions)
{
	// MSVC has deprecated _open(), and recommends _sopen_s instead:
	// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/sopen-s-wsopen-s?view=msvc-170
	// We set the shared flags to _SH_DENYRW, because I don't think sharing is
	// required by anything we do here if there's no way for the caller to
	// actually specify the type of sharing they want.

	int handle = -1;
	errno_t result = _sopen_s(&handle, filePath, flags, _SH_DENYRW, permissions);
	return result == 0 ? handle : -1;
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

int PlatformLib_MkDir(const char* path)
{
	return _mkdir(path);
}

int PlatformLib_Unlink(const char* filename)
{
	return _unlink(filename);
}

int PlatformLib_VFScanF(FILE* stream, const char* format, va_list args)
{
	return vfscanf(stream, format, args);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
