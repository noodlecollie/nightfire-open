#include "botrix/parameter_vars.h"
#include "EnginePublicAPI/cvardef.h"
#include "standard_includes.h"
#include "game.h"

// Empirically determined heights with sv_gravity = 800:
// - Max height to jump onto something: 45 units
//   - Matches with pm_shared.c:2291
//   - Upward speed equals 268.3281573 u/s
// - Max height to crouch jump onto something: 63 units
//
// Given this info, we use the current gravity to calculate the maximum heights
// based on it. See https://en.wikipedia.org/wiki/Equations_for_a_falling_body
// v = sqrt(2 * gravity * distance)
// distance = v^2 / (2 * gravity)

static float CalcDistanceTravelledFromRest(float currentFallSpeed, float gravity)
{
	return (currentFallSpeed * currentFallSpeed) / (2.0f * gravity);
}

float CBotrixParameterVars::CalcMaxHeightOfJump()
{
	// If we calculate the distance travelled in freefall
	// at the point where we are moving at the jump impulse
	// speed, this is the same distance as that from the
	// ground to the apex of the jump - it's just the
	// reverse action.
	return CalcDistanceTravelledFromRest(DEFAULT_JUMP_IMPULSE, g_psv_gravity->value);
}

float CBotrixParameterVars::CalcMaxHeightOfCrouchJump()
{
	// A crouch jump is the same as a normal jump, the hull just shrinks a bit.
	// Therefore, we can just add a constant of extra height.
	static constexpr float DIFF_BETWEEN_NORMAL_AND_CROUCH_JUMP = 63.0f - 45.0f;
	return CalcMaxHeightOfJump() + DIFF_BETWEEN_NORMAL_AND_CROUCH_JUMP;
}

float CBotrixParameterVars::GetStepSize()
{
	static cvar_t* sv_stepsize = nullptr;

	if ( !sv_stepsize )
	{
		sv_stepsize = CVAR_GET_POINTER("sv_stepsize");

		if ( !sv_stepsize )
		{
			// Default
			return 18.0f;
		}
	}

	return sv_stepsize->value;
}
