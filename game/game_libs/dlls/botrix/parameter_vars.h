#pragma once

#include "MathLib/vec3.h"
#include "hulldefs.h"
#include "pm_shared.h"

namespace Botrix
{
	class CBotrixParameterVars
	{
	public:
		static constexpr float PLAYER_WIDTH = -VEC_HULL_MIN.x + VEC_HULL_MAX.x;
		static constexpr float PLAYER_HEIGHT = -VEC_HULL_MIN.z + VEC_HULL_MAX.z;
		static constexpr float PLAYER_HEIGHT_CROUCHED = -VEC_DUCK_HULL_MIN.z + VEC_DUCK_HULL_MAX.z;

		// These are measured from ground level.
		static constexpr float PLAYER_EYE = -VEC_HULL_MIN.z + VEC_VIEW.z;
		static constexpr float PLAYER_EYE_CROUCHED = -VEC_DUCK_HULL_MIN.z + VEC_DUCK_VIEW.z;

		static constexpr float PLAYER_EYE_FROM_ORIGIN = VEC_VIEW.z;
		static constexpr float PLAYER_EYE_CROUCHED_FROM_ORIGIN = VEC_DUCK_VIEW.z;

		// The convars for these are client-only, so for now we use constants on the server side.
		static constexpr float PLAYER_RUN_SPEED = 250.0f;
		static constexpr float PLAYER_WALK_SPEED = 0.5f * PLAYER_RUN_SPEED;  // Based on cl_movespeedkey
		static constexpr float PLAYER_CROUCH_SPEED = PLAYER_DUCKING_MULTIPLIER * PLAYER_RUN_SPEED;

		// We don't actually support sprint right now, so set this to match running.
		static constexpr float PLAYER_SPRINT_SPEED = PLAYER_RUN_SPEED;

		// Taken from pm_shared.c:575: sin^-1(0.7)
		static constexpr float MAX_CLIMBABLE_SLOPE_PITCH = 44.427f;

		static constexpr float PLAYER_MAX_HEALTH = 100.0f;
		static constexpr float PLAYER_MAX_ARMOUR = 100.0f;

		static constexpr int DIST_SQR_POINT_TOUCH_XY = static_cast<int>((PLAYER_WIDTH / 4.0f) * (PLAYER_WIDTH / 4.0f));
		static constexpr int DIST_SQR_POINT_TOUCH_LADDER = 5 * 5;

		static float CalcMaxHeightOfJump();
		static float CalcMaxHeightOfCrouchJump();
		static float CalcMaxFallDistanceWithoutDamage();
		static int CalcSquaredPointTouchDistanceForZAxis();
		static float GetStepSize();
		static int GetPlayerRadiusInt();

	private:
		static constexpr float DEFAULT_JUMP_IMPULSE = 268.3281573f;
	};
}  // namespace Botrix
