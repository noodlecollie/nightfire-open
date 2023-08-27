#include "cppmemory.h"

#include <memory>
#include "blockmem.h"
#include "log.h"

void* operator new(size_t size)
{
	void* mem = Alloc(size);
	hlassume(mem != NULL, assume_NoMemory);

	if ( !mem )
	{
		throw std::bad_alloc();
	}

	return mem;
}

void operator delete(void* ptr) noexcept
{
	if ( ptr )
	{
		Free(ptr);
	}
}
