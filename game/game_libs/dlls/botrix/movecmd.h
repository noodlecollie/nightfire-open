#pragma once

#include <cstdint>
#include "MathLib/vec3.h"

namespace Botrix
{
	struct BotrixMoveCmd
	{
		// Player instantaneous view angles.
		Vector viewangles;
		// Intended velocities
		//	forward velocity.
		float forwardmove = 0.0f;
		//  sideways velocity.
		float sidemove = 0.0f;
		//  upward velocity.
		float upmove = 0.0f;
		// Attack button states
		int buttons = 0;
		// Impulse command issued.
		uint8_t impulse = 0;
	};
}  // namespace Botrix
