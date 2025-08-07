/*
hpak.c - custom user package to send other clients
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/
#ifndef HPAK_H
#define HPAK_H

#include "EnginePublicAPI/custom.h"
#include "BuildPlatform/StaticAssert.h"

/*
========================================================================
.HPK archive format	(Hash PAK - HPK)

List of compressed files, that can be identify only by TYPE_*

<format>
header:	dwadinfo_t[dwadinfo_t]
file_1:	byte[dwadinfo_t[num]->disksize]
file_2:	byte[dwadinfo_t[num]->disksize]
file_3:	byte[dwadinfo_t[num]->disksize]
...
file_n:	byte[dwadinfo_t[num]->disksize]
infotable	dlumpinfo_t[dwadinfo_t->numlumps]
========================================================================
*/

#define IDHPAKHEADER (('K' << 24) + ('A' << 16) + ('P' << 8) + 'H')  // little-endian "HPAK"
#define IDHPAK_VERSION 1

// a1ba: because Valve for some reason writes resource_t to file
// I had to make it crossplatform version
#pragma pack(push, 8)
typedef struct dresource_s
{
	char szFileName[64];  // 0, 64

	// cacheline 1 boundary (64 bytes)

	resourcetype_t type;  // 64, 4
	int nIndex;  // 68, 4
	int nDownloadSize;  // 72, 4
	unsigned char ucFlags;  // 76, 1
	unsigned char rgucMD5_hash[16];  // 77, 16
	unsigned char playernum;  // 93, 1
	unsigned char rguc_reserved[32];  // 94, 32

	// 2 bytes hole, try to pack

	// cacheline 2 boundary (128 bytes)
	uint32_t pNext;  // 128, 4
	uint32_t pPrev;  // 132, 4

	// size: 136, cachelines: 3, members: 10
	// sum members: 134, holes: 1, sum holes: 2
	// last cacheline: 8 bytes
} dresource_t;
#pragma pack(pop)

STATIC_ASSERT(
	sizeof(dresource_t) == 136,
	"invalid dresource_t size, HPAKs won't be compatible (no custom logo in multiplayer!)");

typedef struct
{
	int ident;  // should be equal HPAK
	int version;
	int infotableofs;
} hpak_header_t;

typedef struct
{
	dresource_t resource;
	int filepos;
	int disksize;
} hpak_lump_t;

typedef struct
{
	int count;
	hpak_lump_t* entries;  // variable sized.
} hpak_info_t;

#endif  // HPAK_H
