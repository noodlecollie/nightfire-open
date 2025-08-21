#pragma once

#include "MathLib/vec3.h"

Vector CalculateProjectilePosition(const Vector& basePos, const Vector& baseVel, float gravity, float time);
