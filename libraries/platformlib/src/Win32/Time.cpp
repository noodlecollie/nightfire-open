#include "PlatformLib/Time.h"

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

const struct tm* PlatformLib_LocalTime(const time_t* timep)
{
	static struct tm timeStruct;

	errno_t result = localtime_s(&timeStruct, timep);
	return result == 0 ? &timeStruct : nullptr;
}

const char* PlatformLib_CTime(const time_t* timer)
{
	static char buffer[128];

	errno_t result = ctime_s(buffer, sizeof(buffer), timer);
	return result == 0 ? buffer : nullptr;
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
