#pragma once

#include <cstdint>
#include "MathLib/vec2.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"

struct SpreadPatternArgs
{
	WeaponAtts::SpreadPattern pattern = WeaponAtts::SpreadPattern::Gaussian;
	Vector2D baseSpread;
	int randomSeed;
	uint32_t shotNumber = 0;
	uint32_t totalShots = 0;
};

Vector2D CalculateSpread(const SpreadPatternArgs& args);
