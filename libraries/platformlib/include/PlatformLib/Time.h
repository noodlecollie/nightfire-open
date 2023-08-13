#ifndef PLATFORMLIB_TIME_H
#define PLATFORMLIB_TIME_H

#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

const struct tm* PlatformLib_LocalTime(const time_t* timep);
const char* PlatformLib_CTime(const time_t* timer);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // PLATFORMLIB_TIME_H
