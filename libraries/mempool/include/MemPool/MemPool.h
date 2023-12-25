#pragma once

#include <stdint.h>
#include <stddef.h>
#include "BuildPlatform/Typedefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint32_t MemPool_Handle;

typedef enum MemPool_ErrorLevel
{
	MEMPOOL_ERROR = 0,
	MEMPOOL_MESSAGE
} MemPool_ErrorLevel;

typedef void (*MemPool_MessageCallback)(MemPool_ErrorLevel /*errorLevel*/, const char* /*msg*/);

void MemPool_SetMessageCallback(MemPool_MessageCallback errorHandler);
void* MemPool_Realloc(
	MemPool_Handle poolptr,
	void* memptr,
	size_t size,
	qboolean clear,
	const char* filename,
	int fileline);
void* MemPool_Alloc(MemPool_Handle poolptr, size_t size, qboolean clear, const char* filename, int fileline);
MemPool_Handle MemPool_AllocPool(const char* name, const char* filename, int fileline);
void MemPool_FreePool(MemPool_Handle* poolptr, const char* filename, int fileline);
void MemPool_EmptyPool(MemPool_Handle poolptr, const char* filename, int fileline);
void MemPool_Free(void* data, const char* filename, int fileline);
void MemPool_Check(const char* filename, int fileline);
qboolean MemPool_IsAllocatedExt(MemPool_Handle poolptr, void* data);
void MemPool_PrintList(size_t minallocationsize);
void MemPool_PrintStats(void);

#define MEMPOOL_MALLOC(pool, size) MemPool_Alloc(pool, size, false, __FILE__, __LINE__)
#define MEMPOOL_CALLOC(pool, size) MemPool_Alloc(pool, size, true, __FILE__, __LINE__)
#define MEMPOOL_REALLOC(pool, ptr, size) MemPool_Realloc(pool, ptr, size, true, __FILE__, __LINE__)
#define MEMPOOL_FREE(mem) MemPool_Free(mem, __FILE__, __LINE__)
#define MEMPOOL_ALLOCPOOL(name) MemPool_AllocPool(name, __FILE__, __LINE__)
#define MEMPOOL_FREEPOOL(pool) MemPool_FreePool(pool, __FILE__, __LINE__)
#define MEMPOOL_EMPTYPOOL(pool) MemPool_EmptyPool(pool, __FILE__, __LINE__)
#define MEMPOOL_ISALLOCATED(mem) MemPool_IsAllocatedExt(NULL, mem)
#define MEMPOOL_CHECK() MemPool_Check(__FILE__, __LINE__)

#ifdef __cplusplus
}  // extern "C"
#endif
