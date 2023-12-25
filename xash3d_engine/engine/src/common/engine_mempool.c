#include "common/engine_mempool.h"
#include "common/system.h"
#include "common/common.h"

void Mem_MessageFunc(MemPool_ErrorLevel errorLevel, const char* msg)
{
	if ( errorLevel == MEMPOOL_ERROR )
	{
		Sys_Error("%s", msg);
	}
	else
	{
		Con_Printf("%s", msg);
	}
}
