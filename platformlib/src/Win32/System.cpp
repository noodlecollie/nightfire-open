#include "PlatformLib/System.h"
#include <process.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int PlatformLib_ExecV(const char* path, char* const* argv)
{
	return static_cast<int>(_execv(path, argv));
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
