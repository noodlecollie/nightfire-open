#include <stddef.h>
#include <ctype.h>
#include "CRTLib/bitdefs.h"
#include "common/common.h"
#include "common/base_cmd.h"
#include "common/xcvar_internal.h"

// NFTODO: Replace GAME_EXPORT macro with something better defined

struct xcvar_handle_s
{
	char* name;
	char* description;
	char* defaultValue;
	uint32_t externalFlags;
	uint32_t internalFlags;
	char* valueString;
	float valueFloat;

	xcvar_handle_t* next;
};

typedef struct cvarcollection_s
{
	xcvar_handle_t* head;
} cvarcollection_t;

static cvarcollection_t* g_CvarCollection = NULL;

static qboolean NameIsValid(const char* name)
{
	return name && *name && !isspace(*name);
}

static qboolean CheckNameIsValid(const char* name, const char* source)
{
	if ( !NameIsValid(name) )
	{
		Con_Printf(S_ERROR "%s: Provided convar name was %s.\n", source, name ? "empty" : "null");
		return false;
	}

	return true;
}

static xcvar_handle_t* AllocVariable(const xcvar_params_t* params)
{
	ASSERT(params);

	xcvar_handle_t* newVar = (xcvar_handle_t*)Mem_Calloc(host.mempool, sizeof(xcvar_handle_t));

	newVar->name = copystring(params->name);
	newVar->description = copystring(params->description ? params->description : "");
	newVar->defaultValue = copystring(params->defaultValue ? params->defaultValue : "");
	newVar->externalFlags = params->flags;

	newVar->valueString = copystring(newVar->defaultValue);
	newVar->valueFloat = Q_atof(newVar->valueString);

	return newVar;
}

static void FreeVariable(xcvar_handle_t* cvar)
{
	if ( !cvar )
	{
		return;
	}

	if ( cvar->name )
	{
		Mem_Free(cvar->name);
	}

	if ( cvar->description )
	{
		Mem_Free(cvar->description);
	}

	if ( cvar->defaultValue )
	{
		Mem_Free(cvar->defaultValue);
	}

	if ( cvar->valueString )
	{
		Mem_Free(cvar->valueString);
	}

	Mem_Free(cvar);
}

static qboolean UpdateInfo(xcvar_handle_t* var, const char* value, qboolean notify)
{
	ASSERT(var);

	if ( FBitSet(var->externalFlags, FXCVAR_USERINFO) )
	{
		if ( Host_IsDedicated() )
		{
			// g-cont. this is a very strange behavior...
			Info_SetValueForKey(SV_Serverinfo(), var->name, value, MAX_SERVERINFO_STRING),
				SV_BroadcastCommand("fullserverinfo \"%s\"\n", SV_Serverinfo());
		}
#if !XASH_DEDICATED()
		else
		{
			if ( !Info_SetValueForKey(CL_Userinfo(), var->name, value, MAX_INFO_STRING) )
			{
				// Failed to change value
				return false;
			}

			// Time to update server copy of userinfo
			CL_ServerCommand(true, "setinfo \"%s\" \"%s\"\n", var->name, value);
			CL_LegacyUpdateInfo();
		}
#endif
	}

	if ( FBitSet(var->externalFlags, FXCVAR_SERVER) && notify && !FBitSet(var->externalFlags, FXCVAR_UNLOGGED) )
	{
		const char* logValue = FBitSet(var->externalFlags, FXCVAR_PROTECTED) ? "***PROTECTED***" : value;

		Log_Printf("Server cvar \"%s\" = \"%s\"\n", var->name, logValue);
		SV_BroadcastPrintf(NULL, "\"%s\" changed to \"%s\"\n", var->name, logValue);
	}

	return true;
}

static void HandleChange(xcvar_handle_t* var)
{
	Assert(var);

	// tell about changes
	SetBits(var->internalFlags, FXCVAR_INTERNAL_CHANGED);

	// tell the engine parts with global state
	if ( FBitSet(var->externalFlags, FXCVAR_USERINFO) )
	{
		host.userinfo_changed = true;
	}

	if ( FBitSet(var->internalFlags, FXCVAR_INTERNAL_MOVEVARS) )
	{
		host.movevars_changed = true;
	}

	if ( FBitSet(var->internalFlags, FXCVAR_INTERNAL_VIDRESTART) )
	{
		host.renderinfo_changed = true;
	}

	// TODO: Is there a nicer way to do this?
	if ( Q_strcmp(var->name, "sv_cheats") == 0 )
	{
		host.allow_cheats = Q_atoi(var->valueString) != 0;
	}
}

static xcvar_handle_t* RegisterNewVariable(const xcvar_params_t* params)
{
	if ( !g_CvarCollection || !params )
	{
		return NULL;
	}

	if ( !CheckNameIsValid(params->name, "RegisterNewVariable") )
	{
		return NULL;
	}

	// Check for overlap with a command
	if ( Cmd_Exists(params->name) )
	{
		Con_DPrintf(S_ERROR "Can't register variable '%s', it is already defined as a command\n", params->name);
		return NULL;
	}

	{
		// Check to see if it has allready been defined
		xcvar_handle_t* duplicate = Xcvar_Find(params->name);

		if ( duplicate )
		{
			if ( !FBitSet(duplicate->internalFlags, FXCVAR_INTERNAL_TEMPORARY) )
			{
				Con_DPrintf(S_ERROR "Can't register variable '%s', it is already defined\n", params->name);
				return NULL;
			}

			// Unlink the existing temporary variable, to be replaced with the new one.
			Xcvar_UnlinkDirect(params->name, 0, FXCVAR_INTERNAL_TEMPORARY);
		}
	}

	// We are now fully committed to creating this cvar.
	xcvar_handle_t* var = AllocVariable(params);

	// Find the supposed position in chain (alphanumerical order)
	xcvar_handle_t* cur = NULL;
	xcvar_handle_t* find = g_CvarCollection->head;

	while ( find && Q_strcmp(find->name, var->name) < 0 )
	{
		cur = find;
		find = find->next;
	}

	// Now link variable
	if ( cur )
	{
		cur->next = var;
	}
	else
	{
		g_CvarCollection->head = var;
	}

	var->next = find;

	// Fill in cls.userinfo, svs.serverinfo
	UpdateInfo(var, var->valueString, false);

	// Tell engine about changes
	HandleChange(var);

	// Add to map
	BaseCmd_Insert(HM_CVAR, var, var->name);

	return var;
}

void Xcvar_Init(void)
{
	if ( g_CvarCollection )
	{
		return;
	}

	g_CvarCollection = Mem_Calloc(host.mempool, sizeof(*g_CvarCollection));
}

xcvar_handle_t* Xcvar_GetFirst(void)
{
	if ( !g_CvarCollection )
	{
		return NULL;
	}

	return g_CvarCollection->head;
}

xcvar_handle_t* Xcvar_GetNext(xcvar_handle_t* cvar)
{
	if ( !cvar )
	{
		return NULL;
	}

	return cvar->next;
}

xcvar_handle_t* Xcvar_Find(const char* name)
{
	if ( !CheckNameIsValid(name, "Xcvar_Find") )
	{
		return NULL;
	}

	return (xcvar_handle_t*)BaseCmd_Find(HM_CVAR, name);
}

xcvar_handle_t* GAME_EXPORT Xcvar_LookUp(const xcvar_params_t* params)
{
	if ( !g_CvarCollection || !params )
	{
		return NULL;
	}

	xcvar_handle_t* existing = Xcvar_Find(params->name);

	if ( existing )
	{
		existing->externalFlags |= params->flags;
		return existing;
	}

	return RegisterNewVariable(params);
}

size_t Xcvar_UnlinkDirect(const char* name, uint32_t externalFlagMask, uint32_t internalFlagMask)
{
	if ( !g_CvarCollection )
	{
		return 0;
	}

	size_t count = 0;
	xcvar_handle_t** prev = &g_CvarCollection->head;
	xcvar_handle_t* var = NULL;

	for ( ;; )
	{
		var = *prev;

		if ( !var )
		{
			// Reached the end.
			break;
		}

		// Filter by name
		if ( name && Q_strcmp(var->name, name) != 0 )
		{
			prev = &var->next;
			continue;
		}

		// Filter by external flags
		if ( externalFlagMask && !FBitSet(var->externalFlags, externalFlagMask) )
		{
			prev = &var->next;
			continue;
		}

		// Filter by internal flags
		if ( internalFlagMask && !FBitSet(var->internalFlags, internalFlagMask) )
		{
			prev = &var->next;
			continue;
		}

		// This cvar matches, so remove it.

		BaseCmd_Remove(HM_CVAR, var->name);

		// Unlink variable from list
		*prev = var->next;

		// Destroy the unlinked var.
		FreeVariable(var);

		++count;
	}

	return count;
}

void Xcvar_Unlink(uint32_t externalFlagMask, uint32_t internalFlagMask)
{
	if ( Cvar_VariableInteger("host_gameloaded") && FBitSet(externalFlagMask, FCVAR_EXTDLL) )
	{
		Con_Printf(S_ERROR "Refusing to unlink EXTDLL cvars while the game server is loaded!");
		return;
	}

	if ( Cvar_VariableInteger("host_clientloaded") && FBitSet(externalFlagMask, FCVAR_CLIENTDLL) )
	{
		Con_Printf(S_ERROR "Refusing to unlink CLIENTDLL cvars while the game client is loaded!");
		return;
	}

	if ( Cvar_VariableInteger("host_gameuiloaded") && FBitSet(externalFlagMask, FCVAR_GAMEUIDLL) )
	{
		Con_Printf(S_ERROR "Refusing to unlink GAMEUIDLL cvars while the game UI is loaded!");
		return;
	}

	size_t numUnlinked = Xcvar_UnlinkDirect(NULL, externalFlagMask, internalFlagMask);
	Con_Reportf("Unlinked %zu cvars.\n", numUnlinked);
}

float GAME_EXPORT Xcvar_GetFloatByHandle(const xcvar_handle_t* handle)
{
	return handle ? handle->valueFloat : 0.0f;
}

const char* GAME_EXPORT Xcvar_GetStringByHandle(const xcvar_handle_t* handle)
{
	return handle ? handle->valueString : "";
}

void GAME_EXPORT Xcvar_SetFloatByHandle(xcvar_handle_t* handle, float value)
{
	if ( !handle )
	{
		return;
	}

	// TODO
	(void)value;
	Con_Printf(S_ERROR "Implement Xcvar_SetFloatByHandle\n");
}

void GAME_EXPORT Xcvar_SetStringByHandle(xcvar_handle_t* handle, const char* value)
{
	if ( !handle )
	{
		return;
	}

	// TODO
	(void)value;
	Con_Printf(S_ERROR "Implement Xcvar_SetStringByHandle\n");
}
