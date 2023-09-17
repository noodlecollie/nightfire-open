#pragma once
#include "MathLib/vec2.h"

class CBasePlayer;

namespace WeaponAtts
{
	struct AccuracyParameters;
}

namespace InaccuracyModifiers
{
	Vector2D GetInterpolatedSpread(const WeaponAtts::AccuracyParameters& params, float inaccuracy);

	bool IsInaccuracyDebuggingEnabled();
	bool GetInaccuracyValuesFromDebugCvars(WeaponAtts::AccuracyParameters& params);
	void SetInaccuracyDebugCvarsFromValues(const WeaponAtts::AccuracyParameters& params);
}  // namespace InaccuracyModifiers
