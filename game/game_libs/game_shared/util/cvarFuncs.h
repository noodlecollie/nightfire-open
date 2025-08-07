#pragma once

#include "EnginePublicAPI/cvardef.h"
#include "MathLib/vec2.h"

#ifdef CLIENT_DLL
#include "cl_dll.h"
#else
#include "standard_includes.h"
#endif

static inline cvar_t* GetCvarByName(const char* name)
{
#ifdef CLIENT_DLL
	return gEngfuncs.pfnGetCvarPointer(name);
#else
	return g_engfuncs.pfnCVarGetPointer(name);
#endif
}

Vector2D CvarStringToVector2D(const char* str, bool* ok = nullptr);
void Vector2DToCvarString(const Vector2D& vec, char* buffer, size_t length);
