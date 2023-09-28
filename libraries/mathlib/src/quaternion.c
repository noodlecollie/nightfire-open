#include "MathLib/quaternion.h"
#include "MathLib/mat3x4.h"
#include "MathLib/angles.h"

// make sure quaternions are within 180 degrees of one another,
// if not, reverse q
void QuaternionAlign(const quat_t p, const quat_t q, quat_t qt)
{
	// decide if one of the quaternions is backwards
	float a = 0.0f;
	float b = 0.0f;
	int i;

	for ( i = 0; i < 4; i++ )
	{
		a += (p[i] - q[i]) * (p[i] - q[i]);
		b += (p[i] + q[i]) * (p[i] + q[i]);
	}

	if ( a > b )
	{
		for ( i = 0; i < 4; i++ )
		{
			qt[i] = -q[i];
		}
	}
	else
	{
		for ( i = 0; i < 4; i++ )
		{
			qt[i] = q[i];
		}
	}
}

void QuaternionSlerp(const quat_t p, const quat_t q, float t, quat_t qt)
{
	quat_t q2;

	// 0.0 returns p, 1.0 return q.
	// decide if one of the quaternions is backwards
	QuaternionAlign(p, q, q2);
	QuaternionSlerpNoAlign(p, q2, t, qt);
}

void QuaternionSlerpNoAlign(const quat_t p, const quat_t q, float t, quat_t qt)
{
	float omega, cosom, sinom, sclp, sclq;
	int i;

	// 0.0 returns p, 1.0 return q.
	cosom = p[0] * q[0] + p[1] * q[1] + p[2] * q[2] + p[3] * q[3];

	if ( (1.0f + cosom) > 0.000001f )
	{
		if ( (1.0f - cosom) > 0.000001f )
		{
			omega = acosf(cosom);
			sinom = sinf(omega);
			sclp = sinf((1.0f - t) * omega) / sinom;
			sclq = sinf(t * omega) / sinom;
		}
		else
		{
			sclp = 1.0f - t;
			sclq = t;
		}

		for ( i = 0; i < 4; i++ )
		{
			qt[i] = sclp * p[i] + sclq * q[i];
		}
	}
	else
	{
		qt[0] = -q[1];
		qt[1] = q[0];
		qt[2] = -q[3];
		qt[3] = q[2];

		sclp = sinf((1.0f - t) * (0.5f * M_PI_F));
		sclq = sinf(t * (0.5f * M_PI_F));

		for ( i = 0; i < 3; i++ )
		{
			qt[i] = sclp * p[i] + sclq * qt[i];
		}
	}
}
