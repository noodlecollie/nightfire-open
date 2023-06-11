#ifndef PLATFORMLIB_STRING_H
#define PLATFORMLIB_STRING_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int PlatformLib_StrCaseCmp(const char *s1, const char *s2);
int PlatformLib_StrNCaseCmp(const char *s1, const char *s2, size_t n);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // PLATFORMLIB_STRING_H
