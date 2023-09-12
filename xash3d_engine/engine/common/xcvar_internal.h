#pragma once

#include <stdint.h>
#include "PlatformDefs/typedefs.h"
#include "XashDefs/xcvar.h"

typedef enum xcvar_internalflag_e
{
	// Save to a seperate config called video.cfg.
	FXCVAR_INTERNAL_RENDERINFO = (1 << 0),

	// Cannot be set by user at all, and can't be
	// requested by CvarGetPointer from game dlls.
	FXCVAR_INTERNAL_READ_ONLY = (1 << 1),

	// Recreate the window if cvar with this flag is changed.
	FXCVAR_INTERNAL_VIDRESTART = (1 << 2),

	// Can be unlinked at any time, and can be replaced by
	// other cvars with the same name.
	FXCVAR_INTERNAL_TEMPORARY = (1 << 3),

	// Part of movevars_t struct that is shared between
	// client and server.
	FXCVAR_INTERNAL_MOVEVARS = (1 << 4),

	// Set each time the cvar is changed.
	FXCVAR_INTERNAL_CHANGED = (1 << 5),
} xcvar_internalflag;

void Xcvar_Init(void);

xcvar_handle_t* Xcvar_GetFirst(void);
xcvar_handle_t* Xcvar_GetNext(xcvar_handle_t* cvar);
xcvar_handle_t* Xcvar_Find(const char* name);

// If the variable does not exist, it is created.
// If the variable does exist, the provided flags are added to the existing flags.
xcvar_handle_t* Xcvar_LookUp(const xcvar_params_t* params);

// This invalidates the handles of any matching cvars!
// Name may be null if no name matching is required.
// Masks may be 0 if no mask matching is required.
// Unlike Xcvar_Unlink(), no checks are performed based on whether
// game DLLs are loaded.
size_t Xcvar_UnlinkDirect(const char* name, uint32_t externalFlagMask, uint32_t internalFlagMask);
void Xcvar_Unlink(uint32_t externalFlagMask, uint32_t internalFlagMask);

float Xcvar_GetFloatByHandle(const xcvar_handle_t* handle);
const char* Xcvar_GetStringByHandle(const xcvar_handle_t* handle);
void Xcvar_SetFloatByHandle(xcvar_handle_t* handle, float value);
void Xcvar_SetStringByHandle(xcvar_handle_t* handle, const char* value);
