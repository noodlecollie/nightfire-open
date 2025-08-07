#ifndef PLATFORMLIB_SYSTEM_H
#define PLATFORMLIB_SYSTEM_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int PlatformLib_ExecV(const char* path, char* const* argv);
char* PlatformLib_GetCWD(char* buffer, size_t maxlen);
const char* PlatformLib_GetEnv(const char* envVarName);

int PlatformLib_GetExecutablePath(char* out, int capacity, int* dirname_length);
int PlatformLib_GetModulePath(char* out, int capacity, int* dirname_length);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // PLATFORMLIB_SYSTEM_H
