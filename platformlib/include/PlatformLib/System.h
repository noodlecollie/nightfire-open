#ifndef PLATFORMLIB_SYSTEM_H
#define PLATFORMLIB_SYSTEM_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int PlatformLib_ExecV(const char* path, char* const* argv);
char* PlatformLib_GetCWD(char* buffer, size_t maxlen);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // PLATFORMLIB_SYSTEM_H
