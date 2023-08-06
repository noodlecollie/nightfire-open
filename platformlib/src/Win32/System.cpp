#include "PlatformLib/System.h"
#include <process.h>
#include <direct.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int PlatformLib_ExecV(const char* path, char* const* argv)
{
	return static_cast<int>(_execv(path, argv));
}

char* PlatformLib_GetCWD(char* buffer, size_t maxlen)
{
	return _getcwd(buffer, static_cast<int>(maxlen));
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
