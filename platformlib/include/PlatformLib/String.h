#ifndef PLATFORMLIB_STRING_H
#define PLATFORMLIB_STRING_H

#include "PlatformLib/LibExport.h"
#include <stdlib.h>

PLATFORMLIB_PUBLIC(int) PlatformLib_StrCaseCmp(const char *s1, const char *s2);
PLATFORMLIB_PUBLIC(int) PlatformLib_StrNCaseCmp(const char *s1, const char *s2, size_t n);

#endif // PLATFORMLIB_STRING_H
