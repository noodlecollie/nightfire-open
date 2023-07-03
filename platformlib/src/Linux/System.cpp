#include "PlatformLib/System.h"
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int PlatformLib_ExecV(const char* path, char* const* argv)
{
	return execv(path, argv);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
