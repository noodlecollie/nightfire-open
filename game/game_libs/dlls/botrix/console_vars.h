#pragma once

#include "EnginePublicAPI/cvardef.h"

class CBotrixCvars
{
public:
	static void RegisterAll();

	static cvar_t botrix_log_trace;
	static cvar_t botrix_waypoint_auto_analyze;
};
