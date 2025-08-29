#include "util/projectileMotion.h"
#include "MathLib/utils.h"
#include "standard_includes.h"

Vector CalculateProjectilePosition(const Vector& basePos, const Vector& baseVel, float gravity, float time)
{
	// At any time t, the projectile's horizontal and vertical displacement
	// from the original position are:
	//   horizDisp = launchSpeed * t * cos(theta)
	//   vertDisp = launchSpeed * t * sin(theta) - (0.5 * gravity * t^2)
	// with theta being the launch angle as measured from the horizontal plane.

	if ( VectorIsNull(baseVel) )
	{
		// Not moving.
		return basePos;
	}

	const float angleFromHPlane = 90.0f - AngleBetweenVectors(baseVel, Vector(0.0f, 0.0f, 1.0f));
	const float speed = baseVel.Length();
	const float hDisp = speed * time * cosf(DEG2RADF(angleFromHPlane));
	const float vDisp = (speed * time * sinf(DEG2RADF(angleFromHPlane))) - (0.5f * gravity * time * time);

	const Vector hDir = Vector(baseVel.x, baseVel.y, 0.0f).Normalize();

	return basePos + (hDisp * hDir) + (vDisp * Vector(0.0f, 0.0f, 1.0f));
}

float CalculateProjectileDistanceAtGroundContact(
	float launchHeight,
	float launchPitch,
	float launchSpeed,
	float gravity)
{
	// Negate launch pitch so our calculations work.
	launchPitch *= -1.0f;

	if ( launchPitch <= -90.0f || launchPitch >= 90.0f )
	{
		return 0.0f;
	}

	if ( launchSpeed <= 0.0f )
	{
		return 0.0f;
	}

	Vector launchVel;
	AngleVectors(Vector(launchPitch, 0, 0), launchVel, nullptr, nullptr);

	// At any time t, the projectile's horizontal and vertical displacement are:
	//   horizDisp = launchSpeed * t * cos(launchPitch)
	//   vertDisp = launchSpeed * t * sin(launchPitch) - (0.5 * gravity * t^2)
	// What we want to determine is what the horizontal displacement is at the
	// point when the vertical displacement is -launchHeight.
	// To do this, we first solve for t in the vertical displacement equation,
	// and then feed this into the horizontal one.

	// (t * launchSpeed * sin(launchPitch)) - (t^2 * 0.5 * gravity) = -launchHeight
	// Converted to a quadratic equation in terms of t, this becomes:
	// (-0.5 * gravity) * t^2 + (launchSpeed * sin(launchPitch)) * t + launchHeight = 0
	// Can use quadratic formula: (-b +- sqrt(b^2 - 4ac)) / 2a
	// Here, a = (-0.5 * gravity), b = (launchSpeed * sin(launchPitch)), c = launchHeight

	const float a = -0.5f * gravity;
	const float b = launchSpeed * sinf(DEG2RADF(launchPitch));
	const float c = launchHeight;
	const float discriminant = (b * b) - (4.0f * a * c);

	if ( discriminant < 0.0f )
	{
		// Never crosses ground level.
		return 0.0f;
	}

	float tAtGroundLevel = 0.0f;

	if ( discriminant == 0.0f )
	{
		tAtGroundLevel = (-1.0f * b) / (2.0f * a);
	}
	else
	{
		// The parabola crosses the ground level at two points.
		// Work out which one we want (the smallest positive value of t).
		const float sqrtDiscriminant = sqrtf(discriminant);
		const float firstCandidate = ((-1.0f * b) + sqrtDiscriminant) / (2.0f * a);
		const float secondCandidate = ((-1.0f * b) - sqrtDiscriminant) / (2.0f * a);

		if ( firstCandidate <= 0.0f && secondCandidate <= 0.0f )
		{
			// Both behind the launch point, so neither is valid.
			return 0.0f;
		}

		if ( firstCandidate > 0.0f && secondCandidate > 0.0f )
		{
			tAtGroundLevel = firstCandidate < secondCandidate ? firstCandidate : secondCandidate;
		}
		else
		{
			tAtGroundLevel = firstCandidate > 0.0f ? firstCandidate : secondCandidate;
		}
	}

	// Now we can work out the horizontal displacement.
	return launchSpeed * tAtGroundLevel * cosf(DEG2RADF(launchPitch));
}
