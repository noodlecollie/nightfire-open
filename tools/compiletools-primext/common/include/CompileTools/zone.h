#pragma once

#include <stddef.h>

enum MemAllocZone
{
	C_COMMON = 0,
	C_TEMPORARY,
	C_SAFEALLOC,
	C_FILESYSTEM,
	C_WINDING,
	C_BRUSHSIDE,
	C_BSPBRUSH,
	C_LEAFNODE,
	C_SURFACE,
	C_BSPTREE,
	C_PORTAL,
	C_STRING,
	C_EPAIR,
	C_PATCH,
	C_MAXSTAT,
};

void* Mem_Alloc(size_t sizeInBytes, MemAllocZone target = C_COMMON);
void* Mem_Realloc(void* ptr, size_t sizeInBytes, MemAllocZone target = C_COMMON);
void Mem_Free(void* ptr, MemAllocZone target = C_COMMON);
size_t Mem_Size(void* ptr);
void Mem_Check(void);
void Mem_Peak(void);
