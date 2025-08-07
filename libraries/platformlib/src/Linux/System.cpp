#include "PlatformLib/System.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "whereami/whereami.h"

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

int PlatformLib_GetExecutablePath(char* out, int capacity, int* dirname_length)
{
	return wai_getExecutablePath(out, capacity, dirname_length);
}

int PlatformLib_GetModulePath(char* out, int capacity, int* dirname_length)
{
	return wai_getModulePath(out, capacity, dirname_length);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
