#include "PlatformLib/String.h"
#include <strings.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int PlatformLib_StrCaseCmp(const char *s1, const char *s2)
{
	return strcasecmp(s1, s2);
}

int PlatformLib_StrNCaseCmp(const char *s1, const char *s2, size_t n)
{
	return strncasecmp(s1, s2, n);
}

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
