#include "PlatformLib/Net.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

unsigned long PlatformLib_Inet_Addr(const char* ipv4String)
{
	return inet_addr(ipv4String);
}

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
