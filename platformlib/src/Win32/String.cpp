#include "PlatformLib/String.h"
#include <string.h>

PLATFORMLIB_PUBLIC(int) PlatformLib_StrCaseCmp(const char *s1, const char *s2)
{
	return _stricmp(s1, s2);
}
