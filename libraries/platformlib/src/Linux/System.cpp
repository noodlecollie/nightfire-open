#include "PlatformLib/System.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int PlatformLib_ExecV(const char* path, char* const* argv)
{
	return execv(path, argv);
}

char* PlatformLib_GetCWD(char* buffer, size_t maxlen)
{
	return getcwd(buffer, maxlen);
}

const char* PlatformLib_GetEnv(const char* envVarName)
{
	return getenv(envVarName);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
