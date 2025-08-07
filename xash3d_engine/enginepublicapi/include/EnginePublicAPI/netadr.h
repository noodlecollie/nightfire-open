/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/

#pragma once

#include <stdint.h>
#include "BuildPlatform/PlatformID.h"
#include "BuildPlatform/StaticAssert.h"

typedef enum
{
	NA_UNUSED = 0,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX,
	NA_IP6,
	NA_MULTICAST_IP6,  // all nodes multicast
} netadrtype_t;

// Original structure:
// typedef struct netadr_s
// {
// 	netadrtype_t	type;
// 	unsigned char	ip[4];
// 	unsigned char	ipx[10];
// 	unsigned short	port;
// } netadr_t;

// Xash then converted this to:
// typedef struct netadr_s
// {
// 	union
// 	{
// 		struct
// 		{
// 			uint32_t type;
// 			union
// 			{
// 				uint8_t   ip[4];
// 				uint32_t  ip4;
// 			};
// 			uint8_t  ipx[10];
// 		};
// 		struct
// 		{
// #if XASH_LITTLE_ENDIAN()
// 			uint16_t type6;
// 			uint8_t ip6[16];
// #elif XASH_BIG_ENDIAN()
// 			uint8_t ip6_0[2];
// 			uint16_t type6;
// 			uint8_t ip6_2[14];
// #endif
// 		};
// 	};
// 	uint16_t port;
// } netadr_t;

#pragma pack(push, 1)
typedef union netadr_ipv4_ip_u
{
	uint8_t bytes[4];
	uint32_t full;
} netadr_ipv4_ip_u;  // 4 bytes

typedef struct netadr_ipv4_s
{
	uint32_t type;  // 4 bytes
	netadr_ipv4_ip_u ip;  // 4 bytes
	uint8_t ipx[10];  // 10 bytes
} netadr_ipv4_t;  // 18 bytes

#if XASH_LITTLE_ENDIAN()
typedef struct netadr_ipv6_s
{
	uint16_t type6;  // 2 bytes
	uint8_t ip6[16];  // 16 bytes
} netadr_ipv6_t;  // 18 bytes
#elif XASH_BIG_ENDIAN()
typedef struct netadr_ipv6_s
{
	uint8_t ip6_0[2];  // 2 bytes
	uint16_t type6;  // 2 bytes
	uint8_t ip6_2[14];  // 14 bytes
} netadr_ipv6_t;  // 18 bytes
#endif

typedef union netadr_ip_u
{
	netadr_ipv4_t ip4;  // 18 bytes
	netadr_ipv6_t ip6;  // 18 bytes
} netadr_ip_u;  // 18 bytes

typedef struct netadr_s
{
	netadr_ip_u ip;  // 18 bytes
	uint16_t port;  // 2 bytes
} netadr_t;  // 20 bytes
#pragma pack(pop)

STATIC_ASSERT(sizeof(netadr_t) == 20, "invalid netadr_t size");
