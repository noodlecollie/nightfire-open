#include "util/projectileMotion.h"
#include "MathLib/utils.h"

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
