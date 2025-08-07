#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "BuildPlatform/PlatformID.h"
#include "MemPool/MemPool.h"
#include "CRTLib/crtlib.h"

#define MEMPOOL_MAX_MESSAGE_LENGTH 1024
#define MEMPOOL_MAX_FILE_NAME_LENGTH 256
#define MEMHEADER_SENTINEL1 0xDEADF00DU
#define MEMHEADER_SENTINEL2 0xDFU

typedef struct memheader_s
{
	struct memheader_s* next;  // next and previous memheaders in chain belonging to pool
	struct memheader_s* prev;
	struct mempool_s* pool;  // pool this memheader belongs to
	size_t size;  // size of the memory after the header (excluding header and sentinel2)
	const char* filename;  // file name and line where Mem_Alloc was called
	int fileline;

#if !XASH_64BIT()
	uint32_t pad0;  // doesn't have value, only to make Mem_Alloc return aligned addresses on ILP32
#endif

	uint32_t sentinel1;  // should always be MEMHEADER_SENTINEL1

	// immediately followed by data, which is followed by a MEMHEADER_SENTINEL2 byte
} memheader_t;

typedef struct mempool_s
{
	uint32_t sentinel1;  // should always be MEMHEADER_SENTINEL1
	struct memheader_s* chain;  // chain of individual memory allocations
	size_t totalsize;  // total memory allocated in this pool (inside memheaders)
	size_t realsize;  // total memory allocated in this pool (actual malloc total)
	size_t lastchecksize;  // updated each time the pool is displayed by memlist
	struct mempool_s* next;  // linked into global mempool list
	const char* filename;  // file name and line where Mem_AllocPool was called
	int fileline;
	MemPool_Handle idx;
	char name[64];  // name of the pool
	uint32_t sentinel2;  // should always be MEMHEADER_SENTINEL1
} mempool_t;

static MemPool_MessageCallback g_MessageCallback = NULL;
static mempool_t* g_PoolChain = NULL;

// a1ba: due to mempool being passed with the model through reused 32-bit field
// which makes engine incompatible with 64-bit pointers I changed mempool type
// from pointer to 32-bit handle, thankfully mempool structure is private
// But! Mempools are handled through linked list so we can't index them safely
static MemPool_Handle g_Lastidx = 0;

static void EmitMessage(MemPool_ErrorLevel level, const char* format, ...)
{
	if ( !g_MessageCallback )
	{
		return;
	}

	char message[MEMPOOL_MAX_MESSAGE_LENGTH];

	va_list args;
	va_start(args, format);
	Q_vsnprintf(message, sizeof(message), format, args);
	va_end(args);

	g_MessageCallback(level, message);
}

#define EMIT_MESSAGE(level, ...) \
	do \
	{ \
		if ( g_MessageCallback ) \
		{ \
			EmitMessage(level, __VA_ARGS__); \
		} \
	} \
	while ( 0 )

static mempool_t* FindPool(MemPool_Handle poolptr)
{
	for ( mempool_t* pool = g_PoolChain; pool; pool = pool->next )
	{
		if ( pool->idx == poolptr )
		{
			return pool;
		}
	}

	EMIT_MESSAGE(MEMPOOL_ERROR, "%s: not allocated or double freed pool %d", __FUNCTION__, poolptr);

	return NULL;
}

static const char* CheckFileName(const char* filename)
{
	static const char* const DUMMY = "<corrupted>";

	if ( !filename || !(*filename) )
	{
		return DUMMY;
	}

	for ( size_t index = 0; index < MEMPOOL_MAX_FILE_NAME_LENGTH; ++index )
	{
		if ( filename[index] == '\0' )
		{
			return filename;  // valid name
		}
	}

	return DUMMY;
}

static qboolean CheckAlloc(mempool_t* pool, void* data)
{
	if ( pool )
	{
		// search only one pool
		memheader_t* target = (memheader_t*)((byte*)data - sizeof(memheader_t));

		for ( memheader_t* header = pool->chain; header; header = header->next )
		{
			if ( header == target )
			{
				return true;
			}
		}
	}
	else
	{
		// search all pools
		for ( pool = g_PoolChain; pool; pool = pool->next )
		{
			if ( CheckAlloc(pool, data) )
			{
				return true;
			}
		}
	}

	return false;
}

static void CheckHeaderSentinels(void* data, const char* filename, int fileline)
{
	memheader_t* mem;

	if ( data == NULL )
	{
		EMIT_MESSAGE(MEMPOOL_ERROR, "Mem_CheckSentinels: data == NULL (sentinel check at %s:%i)\n", filename, fileline);
	}

	mem = (memheader_t*)((byte*)data - sizeof(memheader_t));

	if ( mem->sentinel1 != MEMHEADER_SENTINEL1 )
	{
		mem->filename = CheckFileName(mem->filename);  // make sure what we don't crash var_args

		EMIT_MESSAGE(
			MEMPOOL_ERROR,
			"Mem_CheckSentinels: trashed header sentinel 1 (block allocated at %s:%i, sentinel check at %s:%i)\n",
			mem->filename,
			mem->fileline,
			filename,
			fileline);
	}

	if ( *((byte*)mem + sizeof(memheader_t) + mem->size) != MEMHEADER_SENTINEL2 )
	{
		mem->filename = CheckFileName(mem->filename);  // make sure what we don't crash var_args

		EMIT_MESSAGE(
			MEMPOOL_ERROR,
			"Mem_CheckSentinels: trashed header sentinel 2 (block allocated at %s:%i, sentinel check at %s:%i)\n",
			mem->filename,
			mem->fileline,
			filename,
			fileline);
	}
}

void MemPool_SetMessageCallback(MemPool_MessageCallback messageCallback)
{
	g_MessageCallback = messageCallback;
}

void* MemPool_Alloc(MemPool_Handle poolptr, size_t size, qboolean clear, const char* filename, int fileline)
{
	memheader_t* mem;
	mempool_t* pool;

	if ( size <= 0 )
	{
		return NULL;
	}

	if ( !poolptr )
	{
		EMIT_MESSAGE(MEMPOOL_ERROR, "Mem_Alloc: pool == NULL (alloc at %s:%i)\n", filename, fileline);
	}

	pool = FindPool(poolptr);

	pool->totalsize += size;

	// big allocations are not clumped
	pool->realsize += sizeof(memheader_t) + size + sizeof(size_t);
	mem = (memheader_t*)malloc(sizeof(memheader_t) + size + sizeof(size_t));

	if ( mem == NULL )
	{
		EMIT_MESSAGE(MEMPOOL_ERROR, "Mem_Alloc: out of memory (alloc at %s:%i)\n", filename, fileline);
	}

	mem->filename = filename;
	mem->fileline = fileline;
	mem->size = size;
	mem->pool = pool;
	mem->sentinel1 = MEMHEADER_SENTINEL1;

	// we have to use only a single byte for this sentinel, because it may not be aligned
	// and some platforms can't use unaligned accesses
	*((byte*)mem + sizeof(memheader_t) + mem->size) = MEMHEADER_SENTINEL2;

	// append to head of list
	mem->next = pool->chain;
	mem->prev = NULL;
	pool->chain = mem;

	if ( mem->next )
	{
		mem->next->prev = mem;
	}

	if ( clear )
	{
		memset((void*)((byte*)mem + sizeof(memheader_t)), 0, mem->size);
	}

	return (void*)((byte*)mem + sizeof(memheader_t));
}

static void Mem_FreeBlock(memheader_t* mem, const char* filename, int fileline)
{
	mempool_t* pool;

	if ( mem->sentinel1 != MEMHEADER_SENTINEL1 )
	{
		mem->filename = CheckFileName(mem->filename);  // make sure what we don't crash var_args

		EMIT_MESSAGE(
			MEMPOOL_ERROR,
			"Mem_Free: trashed header sentinel 1 (alloc at %s:%i, free at %s:%i)\n",
			mem->filename,
			mem->fileline,
			filename,
			fileline);
	}

	if ( *((byte*)mem + sizeof(memheader_t) + mem->size) != MEMHEADER_SENTINEL2 )
	{
		mem->filename = CheckFileName(mem->filename);  // make sure what we don't crash var_args

		EMIT_MESSAGE(
			MEMPOOL_ERROR,
			"Mem_Free: trashed header sentinel 2 (alloc at %s:%i, free at %s:%i)\n",
			mem->filename,
			mem->fileline,
			filename,
			fileline);
	}

	pool = mem->pool;

	// unlink memheader from doubly linked list
	if ( (mem->prev ? mem->prev->next != mem : pool->chain != mem) || (mem->next && mem->next->prev != mem) )
	{
		EMIT_MESSAGE(MEMPOOL_ERROR, "Mem_Free: not allocated or double freed (free at %s:%i)\n", filename, fileline);
	}

	if ( mem->prev )
	{
		mem->prev->next = mem->next;
	}
	else
	{
		pool->chain = mem->next;
	}

	if ( mem->next )
	{
		mem->next->prev = mem->prev;
	}

	// memheader has been unlinked, do the actual free now
	pool->totalsize -= mem->size;

	pool->realsize -= sizeof(memheader_t) + mem->size + sizeof(size_t);
	free(mem);
}

void MemPool_Free(void* data, const char* filename, int fileline)
{
	if ( data == NULL )
	{
		EMIT_MESSAGE(MEMPOOL_ERROR, "Mem_Free: data == NULL (called at %s:%i)\n", filename, fileline);
	}

	Mem_FreeBlock((memheader_t*)((byte*)data - sizeof(memheader_t)), filename, fileline);
}

void* MemPool_Realloc(
	MemPool_Handle poolptr,
	void* memptr,
	size_t size,
	qboolean clear,
	const char* filename,
	int fileline)
{
	memheader_t* memhdr = NULL;
	char* nb;

	if ( size <= 0 )
	{
		return memptr;  // no need to reallocate
	}

	if ( memptr )
	{
		memhdr = (memheader_t*)((byte*)memptr - sizeof(memheader_t));

		if ( size == memhdr->size )
		{
			return memptr;
		}
	}

	nb = MemPool_Alloc(poolptr, size, clear, filename, fileline);

	if ( memptr )  // first allocate?
	{
		size_t newsize = memhdr->size < size ? memhdr->size : size;  // upper data can be trucnated!
		memcpy(nb, memptr, newsize);
		MemPool_Free(memptr, filename, fileline);  // free unused old block
	}

	return (void*)nb;
}

MemPool_Handle MemPool_AllocPool(const char* name, const char* filename, int fileline)
{
	mempool_t* pool = (mempool_t*)malloc(sizeof(mempool_t));

	if ( pool == NULL )
	{
		EMIT_MESSAGE(MEMPOOL_ERROR, "Mem_AllocPool: out of memory (allocpool at %s:%i)\n", filename, fileline);
		return 0;
	}

	memset(pool, 0, sizeof(mempool_t));

	// fill header
	pool->sentinel1 = MEMHEADER_SENTINEL1;
	pool->sentinel2 = MEMHEADER_SENTINEL1;
	pool->filename = filename;
	pool->fileline = fileline;
	pool->chain = NULL;
	pool->totalsize = 0;
	pool->realsize = sizeof(mempool_t);
	Q_strcpy(pool->name, sizeof(pool->name), name);
	pool->next = g_PoolChain;
	g_PoolChain = pool;

	pool->idx = ++g_Lastidx;
	return pool->idx;
}

void MemPool_FreePool(MemPool_Handle* poolptr, const char* filename, int fileline)
{
	if ( !(*poolptr) )
	{
		return;
	}

	mempool_t* pool = FindPool(*poolptr);

	if ( pool )
	{
		mempool_t** chainaddress = NULL;

		// unlink pool from chain
		for ( chainaddress = &g_PoolChain; *chainaddress && *chainaddress != pool;
			  chainaddress = &((*chainaddress)->next) )
		{
		}

		if ( *chainaddress != pool )
		{
			EMIT_MESSAGE(MEMPOOL_ERROR, "Mem_FreePool: pool already free (freepool at %s:%i)\n", filename, fileline);
		}

		if ( pool->sentinel1 != MEMHEADER_SENTINEL1 )
		{
			EMIT_MESSAGE(
				MEMPOOL_ERROR,
				"Mem_FreePool: trashed pool sentinel 1 (allocpool at %s:%i, freepool at %s:%i)\n",
				pool->filename,
				pool->fileline,
				filename,
				fileline);
		}

		if ( pool->sentinel2 != MEMHEADER_SENTINEL1 )
		{
			EMIT_MESSAGE(
				MEMPOOL_ERROR,
				"Mem_FreePool: trashed pool sentinel 2 (allocpool at %s:%i, freepool at %s:%i)\n",
				pool->filename,
				pool->fileline,
				filename,
				fileline);
		}

		*chainaddress = pool->next;

		// free memory owned by the pool
		while ( pool->chain )
		{
			Mem_FreeBlock(pool->chain, filename, fileline);
		}

		// free the pool itself
		memset(pool, 0xBF, sizeof(mempool_t));
		free(pool);
		*poolptr = 0;
	}
}

void MemPool_EmptyPool(MemPool_Handle poolptr, const char* filename, int fileline)
{
	mempool_t* pool = FindPool(poolptr);

	if ( !poolptr )
	{
		EMIT_MESSAGE(MEMPOOL_ERROR, "Mem_EmptyPool: pool == NULL (emptypool at %s:%i)\n", filename, fileline);
	}

	if ( pool->sentinel1 != MEMHEADER_SENTINEL1 )
	{
		EMIT_MESSAGE(
			MEMPOOL_ERROR,
			"Mem_EmptyPool: trashed pool sentinel 1 (allocpool at %s:%i, emptypool at %s:%i)\n",
			pool->filename,
			pool->fileline,
			filename,
			fileline);
	}

	if ( pool->sentinel2 != MEMHEADER_SENTINEL1 )
	{
		EMIT_MESSAGE(
			MEMPOOL_ERROR,
			"Mem_EmptyPool: trashed pool sentinel 2 (allocpool at %s:%i, emptypool at %s:%i)\n",
			pool->filename,
			pool->fileline,
			filename,
			fileline);
	}

	// free memory owned by the pool
	while ( pool->chain )
	{
		Mem_FreeBlock(pool->chain, filename, fileline);
	}
}

qboolean MemPool_IsAllocatedExt(MemPool_Handle poolptr, void* data)
{
	mempool_t* pool = NULL;
	if ( poolptr )
		pool = FindPool(poolptr);

	return CheckAlloc(pool, data);
}

void MemPool_Check(const char* filename, int fileline)
{
	memheader_t* mem;
	mempool_t* pool;

	for ( pool = g_PoolChain; pool; pool = pool->next )
	{
		if ( pool->sentinel1 != MEMHEADER_SENTINEL1 )
		{
			EMIT_MESSAGE(
				MEMPOOL_ERROR,
				"Mem_CheckSentinelsGlobal: trashed pool sentinel 1 (allocpool at %s:%i, sentinel check at %s:%i)\n",
				pool->filename,
				pool->fileline,
				filename,
				fileline);
		}

		if ( pool->sentinel2 != MEMHEADER_SENTINEL1 )
		{
			EMIT_MESSAGE(
				MEMPOOL_ERROR,
				"Mem_CheckSentinelsGlobal: trashed pool sentinel 2 (allocpool at %s:%i, sentinel check at %s:%i)\n",
				pool->filename,
				pool->fileline,
				filename,
				fileline);
		}
	}

	for ( pool = g_PoolChain; pool; pool = pool->next )
	{
		for ( mem = pool->chain; mem; mem = mem->next )
		{
			CheckHeaderSentinels((void*)((byte*)mem + sizeof(memheader_t)), filename, fileline);
		}
	}
}

void MemPool_PrintStats(void)
{
	size_t count = 0, size = 0, realsize = 0;
	mempool_t* pool;

	MEMPOOL_CHECK();
	for ( pool = g_PoolChain; pool; pool = pool->next )
	{
		count++;
		size += pool->totalsize;
		realsize += pool->realsize;
	}

	EMIT_MESSAGE(MEMPOOL_MESSAGE, "^3%lu^7 memory pools, totalling: ^1%s\n", count, Q_memprint((float)size));
	EMIT_MESSAGE(MEMPOOL_MESSAGE, "total allocated size: ^1%s\n", Q_memprint((float)realsize));
}

void MemPool_PrintList(size_t minallocationsize)
{
	MEMPOOL_CHECK();

	EMIT_MESSAGE(
		MEMPOOL_MESSAGE,
		"memory pool list:\n"
		"  ^3size                          name\n");

	for ( mempool_t* pool = g_PoolChain; pool; pool = pool->next )
	{
		long changed_size = (long)pool->totalsize - (long)pool->lastchecksize;

		// poolnames can contain color symbols, make sure what color is reset
		if ( changed_size != 0 )
		{
			char sign = (changed_size < 0) ? '-' : '+';

			EMIT_MESSAGE(
				MEMPOOL_MESSAGE,
				"%10s (%10s actual) %s (^7%c%s change)\n",
				Q_memprint((float)pool->totalsize),
				Q_memprint((float)pool->realsize),
				pool->name,
				sign,
				Q_memprint((float)abs((int)changed_size)));
		}
		else
		{
			EMIT_MESSAGE(
				MEMPOOL_MESSAGE,
				"%5s (%5s actual) %s\n",
				Q_memprint((float)pool->totalsize),
				Q_memprint((float)pool->realsize),
				pool->name);
		}

		pool->lastchecksize = pool->totalsize;

		for ( memheader_t* mem = pool->chain; mem; mem = mem->next )
		{
			if ( mem->size >= minallocationsize )
			{
				EMIT_MESSAGE(
					MEMPOOL_MESSAGE,
					"%10s allocated at %s:%i\n",
					Q_memprint((float)mem->size),
					mem->filename,
					mem->fileline);
			}
		}
	}
}
