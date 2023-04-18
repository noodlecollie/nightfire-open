#pragma once

#include "cvardef.h"

#ifdef CLIENT_DLL
#include "cl_dll.h"
#else
#include "enginecallback.h"
#endif

static inline cvar_t* GetCvarByName(const char* name)
{
#ifdef CLIENT_DLL
	return gEngfuncs.pfnGetCvarPointer(name);
#else
	return g_engfuncs.pfnCVarGetPointer(name);
#endif
}
