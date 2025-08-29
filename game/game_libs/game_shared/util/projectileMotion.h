#pragma once

#include "MathLib/vec3.h"

Vector CalculateProjectilePosition(const Vector& basePos, const Vector& baseVel, float gravity, float time);

// Launch pitch should be in the range (-90 90).
// Launch speed should be > 0.
// Assumes that up is pitch of -90, and down is pitch of 90.
// Returns 0 if the trajectory never crosses ground level,
// ie. the projectile was below ground.
float CalculateProjectileDistanceAtGroundContact(
	float launchHeight,
	float launchPitch,
	float launchSpeed,
	float gravity);
