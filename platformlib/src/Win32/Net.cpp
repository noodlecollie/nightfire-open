#include "PlatformLib/Net.h"
#include <Ws2tcpip.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

unsigned long PlatformLib_Inet_Addr(const char* ipv4String)
{
	unsigned long out = 0;

	return InetPton(AF_INET, ipv4String, &out) == 1
		? out
		: 0;
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
