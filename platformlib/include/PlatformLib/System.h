#ifndef PLATFORMLIB_SYSTEM_H
#define PLATFORMLIB_SYSTEM_H

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int PlatformLib_ExecV(const char* path, char* const* argv);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // PLATFORMLIB_SYSTEM_H
