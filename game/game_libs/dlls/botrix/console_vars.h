#pragma once

#include "EnginePublicAPI/cvardef.h"

namespace Botrix
{
	class CBotrixCvars
	{
	public:
		static void RegisterAll();

		static cvar_t botrix_log_trace;
		static cvar_t botrix_waypoint_auto_analyze;
		static cvar_t botrix_reachable_viz_time;
		static cvar_t botrix_draw_player_hull;
		static cvar_t bot_fill_to_percent;
	};
}  // namespace Botrix
