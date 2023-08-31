#include "PlatformLib/Net.h"
#include <Ws2tcpip.h>
#include <winsock2.h>

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

unsigned long PlatformLib_NToHL(unsigned long netlong)
{
	return ntohl(netlong);
}

unsigned long PlatformLib_HToNL(unsigned long hostlong)
{
	return htonl(hostlong);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
