#include "PlatformLib/Time.h"

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

const struct tm* PlatformLib_LocalTime(const time_t* timep)
{
	return localtime(timep);
}

const char* PlatformLib_CTime(const time_t* timer)
{
	return ctime(timer);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
