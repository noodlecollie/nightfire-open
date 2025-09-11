#pragma once

#include "MathLib/vec3.h"
#include "hulldefs.h"
#include "pm_shared.h"

class CBotrixParameterVars
{
public:
	static constexpr float PLAYER_WIDTH = -VEC_HULL_MIN.x + VEC_HULL_MAX.x;
	static constexpr float PLAYER_HEIGHT = -VEC_HULL_MIN.z + VEC_HULL_MAX.z;
	static constexpr float PLAYER_HEIGHT_CROUCHED = -VEC_DUCK_HULL_MIN.z + VEC_DUCK_HULL_MAX.z;
	static constexpr float PLAYER_EYE = -VEC_HULL_MIN.z + VEC_VIEW.z;
	static constexpr float PLAYER_EYE_CROUCHED = -VEC_DUCK_HULL_MIN.z + VEC_DUCK_VIEW.z;

	// The convars for these are client-only, so for now we use constants on the server side.
	static constexpr float PLAYER_RUN_SPEED = 250.0f;
	static constexpr float PLAYER_WALK_SPEED = 0.5f * PLAYER_RUN_SPEED;  // Based on cl_movespeedkey
	static constexpr float PLAYER_CROUCH_SPEED = PLAYER_DUCKING_MULTIPLIER * PLAYER_RUN_SPEED;

	// We don't actually support sprint right now, but just set to some sensible default.
	static constexpr float PLAYER_SPRINT_SPEED = 1.5f * PLAYER_RUN_SPEED;

	static float CalcMaxHeightOfJump();
	static float CalcMaxHeightOfCrouchJump();
	static float GetStepSize();

private:
	static constexpr float DEFAULT_JUMP_IMPULSE = 268.3281573f;
};
