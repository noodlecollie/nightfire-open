#include "botrix/console_vars.h"
#include "standard_includes.h"

cvar_t CBotrixCvars::botrix_log_trace = CONSTRUCT_CVAR_T("botrix_log_trace", 0, FCVAR_PRIVILEGED);
cvar_t CBotrixCvars::botrix_waypoint_auto_analyze =
	CONSTRUCT_CVAR_T("botrix_waypoint_auto_analyze", 0, FCVAR_PRIVILEGED);

void CBotrixCvars::RegisterAll()
{
	CVAR_REGISTER(&botrix_log_trace);
	CVAR_REGISTER(&botrix_waypoint_auto_analyze);
}
