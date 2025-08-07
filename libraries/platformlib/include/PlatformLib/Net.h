#ifndef PLATFORMLIB_NET_H
#define PLATFORMLIB_NET_H

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

unsigned long PlatformLib_Inet_Addr(const char* ipv4String);
unsigned long PlatformLib_NToHL(unsigned long netlong);
unsigned long PlatformLib_HToNL(unsigned long hostlong);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // PLATFORMLIB_NET_H
