#pragma once

#include <stdint.h>

typedef enum xcvar_creationflag_e
{
	// Value is saved to vars.rc.
	FXCVAR_ARCHIVE = (1 << 0),

	// Changes the client's info string.
	FXCVAR_USERINFO = (1 << 1),

	// Notifies connected players when changed.
	FXCVAR_SERVER = (1 << 2),

	// Same as FXCVAR_SERVER, but the data is protected since it's a password etc.
	// Sends 1 if the value has content, otherwise 0.
	FXCVAR_PROTECTED = (1 << 3),

	// Cannot be changed by clients connected to a multiplayer server.
	FXCVAR_SPONLY = (1 << 4),

	// Cannot contain unprintable characters (eg. used for player name etc).
	FXCVAR_PRINTABLEONLY = (1 << 5),

	// If this is a FXCVAR_SERVER, don't log changes to
	// the log file/console if we are creating a log.
	FXCVAR_UNLOGGED = (1 << 6),

	// Strip trailing/leading white space from the value.
	FXCVAR_NOEXTRAWHITEPACE = (1 << 7),

	// only available in privileged mode.
	FXCVAR_PRIVILEGED = (1 << 8),

	// Filtered in unprivileged mode if cl_filterstuffcmd is 1.
	FXCVAR_FILTERABLE = (1 << 9),

	// Write it into <renderer>.cfg (see RefAPI).
	FXCVAR_GLCONFIG = (1 << 10),

	// Cannot be changed if cheats are disabled.
	FXCVAR_CHEAT = (1 << 11),

	// Notify client that this cvar will be applied only after server restart.
	FXCVAR_REQUIRES_RESTART = (1 << 12)
} xcvar_creationflag_t;

typedef struct xcvar_params_s
{
	const char* name;
	const char* description;
	const char* defaultValue;
	uint32_t flags;
} xcvar_params_t;

typedef struct xcvar_handle_s xcvar_handle_t;

#define XCVAR_PARAMS(name, desc, valueString, flags) \
	{ \
		name, desc, valueString, flags \
	}

#define XCVAR_PARAMS_N(name, desc, valueNum, flags) \
	{ \
		name, desc, #valueNum, flags \
	}
