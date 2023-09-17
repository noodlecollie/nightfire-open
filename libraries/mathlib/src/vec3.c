#include "MathLib/vec3.h"
#include "MathLib/euleraxes.h"

float VectorNormalizeLength(const vec3_t v, vec3_t out)
{
	float length, ilength;

	length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	length = sqrtf(length);

	if ( length )
	{
		ilength = 1.0f / length;
		out[0] = v[0] * ilength;
		out[1] = v[1] * ilength;
		out[2] = v[2] * ilength;
	}

	return length;
}

qboolean VectorCompareEpsilon(const vec3_t vec1, const vec3_t vec2, vec_t epsilon)
{
	vec_t ax, ay, az;

	ax = fabsf(vec1[0] - vec2[0]);
	ay = fabsf(vec1[1] - vec2[1]);
	az = fabsf(vec1[2] - vec2[2]);

	if ( (ax <= epsilon) && (ay <= epsilon) && (az <= epsilon) )
		return true;
	return false;
}

void VectorVectors(const vec3_t forward, vec3_t right, vec3_t up)
{
	float d;

	right[0] = forward[2];
	right[1] = -forward[0];
	right[2] = forward[1];

	d = DotProduct(forward, right);
	VectorMA(right, -d, forward, right);
	VectorNormalize(right);
	CrossProduct(right, forward, up);
	VectorNormalize(up);
}

void AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RADF(angles[YAW]), &sy, &cy);
	SinCos(DEG2RADF(angles[PITCH]), &sp, &cp);
	SinCos(DEG2RADF(angles[ROLL]), &sr, &cr);

	if ( forward )
	{
		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;
	}

	if ( right )
	{
		right[0] = (-1.0f * sr * sp * cy + -1.0f * cr * -sy);
		right[1] = (-1.0f * sr * sp * sy + -1.0f * cr * cy);
		right[2] = (-1.0f * sr * cp);
	}

	if ( up )
	{
		up[0] = (cr * sp * cy + -sr * -sy);
		up[1] = (cr * sp * sy + -sr * cy);
		up[2] = (cr * cp);
	}
}

void VectorAngles(const float* forward, float* angles)
{
	float tmp, yaw, pitch;

	if ( !forward || !angles )
	{
		if ( angles )
		{
			VectorClear(angles);
		}

		return;
	}

	if ( forward[1] == 0 && forward[0] == 0 )
	{
		// fast case
		yaw = 0;

		if ( forward[2] > 0 )
		{
			pitch = 90.0f;
		}
		else
		{
			pitch = 270.0f;
		}
	}
	else
	{
		yaw = (atan2f(forward[1], forward[0]) * 180.0f / M_PI_F);

		if ( yaw < 0 )
		{
			yaw += 360;
		}

		tmp = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2f(forward[2], tmp) * 180.0f / M_PI_F);

		if ( pitch < 0 )
		{
			pitch += 360;
		}
	}

	VectorSet(angles, pitch, yaw, 0);
}

void VectorsAngles(const vec3_t forward, const vec3_t right, const vec3_t up, vec3_t angles)
{
	float pitch, cpitch, yaw, roll;

	pitch = -asinf(forward[2]);
	cpitch = cosf(pitch);

	if ( fabsf(cpitch) > EQUAL_EPSILON )  // gimball lock?
	{
		cpitch = 1.0f / cpitch;
		pitch = RAD2DEGF(pitch);
		yaw = RAD2DEGF(atan2f(forward[1] * cpitch, forward[0] * cpitch));
		roll = RAD2DEGF(atan2f(-right[2] * cpitch, up[2] * cpitch));
	}
	else
	{
		pitch = forward[2] > 0 ? -90.0f : 90.0f;
		yaw = RAD2DEGF(atan2f(right[0], -right[1]));
		roll = 180.0f;
	}

	angles[PITCH] = pitch;
	angles[YAW] = yaw;
	angles[ROLL] = roll;
}
