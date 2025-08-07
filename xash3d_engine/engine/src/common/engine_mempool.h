#pragma once

#include "MemPool/MemPool.h"
#include "common/common.h"

#define Mem_Malloc(pool, size) MEMPOOL_MALLOC(pool, size)
#define Mem_Calloc(pool, size) MEMPOOL_CALLOC(pool, size)
#define Mem_Realloc(pool, ptr, size) MEMPOOL_REALLOC(pool, ptr, size)
#define Mem_Free(mem) MEMPOOL_FREE(mem)
#define Mem_AllocPool(name) MEMPOOL_ALLOCPOOL(name)
#define Mem_FreePool(pool) MEMPOOL_FREEPOOL(pool)
#define Mem_EmptyPool(pool) MEMPOOL_EMPTYPOOL(pool)
#define Mem_IsAllocated(mem) MEMPOOL_ISALLOCATED(mem)
#define Mem_Check() MEMPOOL_CHECK()

#define Mem_IsAllocatedExt(poolptr, data) MemPool_IsAllocatedExt(poolptr, data)
#define Mem_PrintList(val) MemPool_PrintList(val)
#define Mem_PrintStats() MemPool_PrintStats()

// Aliases used in some places:
#define Z_Malloc(size) Mem_Malloc(host.mempool, size)
#define Z_Calloc(size) Mem_Calloc(host.mempool, size)
#define Z_Realloc(ptr, size) Mem_Realloc(host.mempool, ptr, size)
#define Z_Free(ptr) \
	do \
	{ \
		if ( ptr != NULL ) \
		{ \
			Mem_Free(ptr); \
		} \
	} \
	while ( 0 )

void Mem_MessageFunc(MemPool_ErrorLevel errorLevel, const char* msg);
