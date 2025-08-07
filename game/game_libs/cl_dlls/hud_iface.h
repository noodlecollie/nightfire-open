//========= Copyright (c) 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#pragma once
#if !defined(HUD_IFACEH)
#define HUD_IFACEH

#include "exportdef.h"

typedef int (*pfnUserMsgHook)(const char* pszName, int iSize, void* pbuf);
#include "EnginePublicAPI/wrect.h"
#include "EnginePublicAPI/cdll_int.h"
extern cl_enginefunc_t gEngfuncs;
#endif
