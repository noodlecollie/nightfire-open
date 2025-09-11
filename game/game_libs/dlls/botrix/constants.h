#pragma once

#include "MathLib/vec3.h"
#include "hulldefs.h"

class CBotrixConstants
{
public:
	static constexpr float PLAYER_WIDTH = -VEC_HULL_MIN.x + VEC_HULL_MAX.x;
	static constexpr float PLAYER_HEIGHT = -VEC_HULL_MIN.z + VEC_HULL_MAX.z;
	static constexpr float PLAYER_HEIGHT_CROUCHED = -VEC_DUCK_HULL_MIN.z + VEC_DUCK_HULL_MAX.z;
	static constexpr float PLAYER_EYE = -VEC_HULL_MIN.z + VEC_VIEW.z;
	static constexpr float PLAYER_EYE_CROUCHED = -VEC_DUCK_HULL_MIN.z + VEC_DUCK_VIEW.z;
};
