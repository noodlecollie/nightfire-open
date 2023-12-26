#include <float.h>
#include "MathLib/utils.h"
#include "MathLib/mathdefs.h"
#include "MathLib/vec4.h"

#define HULL_PRECISION 4
#define NUM_HULL_ROUNDS SIZE_OF_ARRAY(hull_table)

static uint16_t hull_table[] = {
	2, 4, 6, 8, 12, 16, 18, 24, 28, 32, 36, 40, 48, 54, 56, 60, 64, 72, 80, 112, 120, 128, 140, 176,
};

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

void AngleVectorsTranspose(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up)
{
	float angle;

	angle = DEG2RADF(angles[YAW]);
	float sy = sinf(angle);
	float cy = cosf(angle);

	angle = DEG2RADF(angles[PITCH]);
	float sp = sinf(angle);
	float cp = cosf(angle);

	angle = DEG2RADF(angles[ROLL]);
	float sr = sinf(angle);
	float cr = cosf(angle);

	if ( forward )
	{
		forward[0] = cp * cy;
		forward[1] = (sr * sp * cy + cr * -sy);
		forward[2] = (cr * sp * cy + -sr * -sy);
	}

	if ( right )
	{
		right[0] = cp * sy;
		right[1] = (sr * sp * sy + cr * cy);
		right[2] = (cr * sp * sy + -sr * cy);
	}

	if ( up )
	{
		up[0] = -sp;
		up[1] = sr * cp;
		up[2] = cr * cp;
	}
}

void VectorAngles(const vec3_t forward, vec3_t angles)
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

	if ( fabsf(cpitch) > MATH_FLOAT_EQUAL_EPSILON )  // gimball lock?
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

float AngleBetweenVectors(const vec3_t v1, const vec3_t v2)
{
	const float l1 = VectorLength(v1);
	const float l2 = VectorLength(v2);

	if ( l1 == 0.0f || l2 == 0.0f )
	{
		return 0.0f;
	}

	float angle = acosf(DotProduct(v1, v2) / (l1 * l2));

	return RAD2DEGF(angle);
}

float VectorToYaw(const vec3_t src)
{
	float yaw;

	if ( !src )
	{
		return 0.0f;
	}

	if ( src[1] == 0.0f && src[0] == 0.0f )
	{
		yaw = 0.0f;
	}
	else
	{
		yaw = (float)((int)(atan2f(src[1], src[0]) * 180.0f / M_PI_F));

		if ( yaw < 0 )
		{
			yaw += 360.0f;
		}
	}

	return yaw;
}

void GenerateBasisVectors(const vec3_t forward, vec3_t right, vec3_t up)
{
	if ( forward[0] == 0 && forward[1] == 0 )
	{
		right[0] = 1;
		right[1] = 0;
		right[2] = 0;

		up[0] = -forward[2];
		up[1] = 0;
		up[2] = 0;

		return;
	}

	const vec3_t pointingUpwards = {0.0f, 0.0f, 1.0f};

	CrossProduct(forward, pointingUpwards, right);
	VectorNormalize(right);
	CrossProduct(right, forward, up);
	VectorNormalize(up);
}

void Matrix3x4_AnglesFromMatrix(const matrix3x4 in, vec3_t out)
{
	float xyDist = sqrtf(in[0][0] * in[0][0] + in[1][0] * in[1][0]);

	if ( xyDist > 0.001f )
	{
		// enough here to get angles?
		out[0] = RAD2DEGF(atan2f(-in[2][0], xyDist));
		out[1] = RAD2DEGF(atan2f(in[1][0], in[0][0]));
		out[2] = RAD2DEGF(atan2f(in[2][1], in[2][2]));
	}
	else
	{
		// forward is mostly Z, gimbal lock
		out[0] = RAD2DEGF(atan2f(-in[2][0], xyDist));
		out[1] = RAD2DEGF(atan2f(-in[0][1], in[1][1]));
		out[2] = 0.0f;
	}
}

void Matrix3x4_AnglesToMatrix(const vec3_t angles, matrix3x4 matrix)
{
	float angle;

	angle = DEG2RADF(angles[YAW]);
	float sy = sinf(angle);
	float cy = cosf(angle);

	angle = DEG2RADF(angles[PITCH]);
	float sp = sinf(angle);
	float cp = cosf(angle);

	angle = DEG2RADF(angles[ROLL]);
	float sr = sinf(angle);
	float cr = cosf(angle);

	// matrix = ( YAW * PITCH ) * ROLL
	matrix[0][0] = cp * cy;
	matrix[1][0] = cp * sy;
	matrix[2][0] = -sp;
	matrix[0][1] = sr * sp * cy + cr * -sy;
	matrix[1][1] = sr * sp * sy + cr * cy;
	matrix[2][1] = sr * cp;
	matrix[0][2] = (cr * sp * cy + -sr * -sy);
	matrix[1][2] = (cr * sp * sy + -sr * cy);
	matrix[2][2] = cr * cp;
	matrix[0][3] = 0.0;
	matrix[1][3] = 0.0;
	matrix[2][3] = 0.0;
}

void Matrix3x4_AnglesToIMatrix(const vec3_t angles, matrix3x4 matrix)
{
	float angle;

	angle = DEG2RADF(angles[YAW]);
	float sy = sinf(angle);
	float cy = cosf(angle);

	angle = DEG2RADF(angles[PITCH]);
	float sp = sinf(angle);
	float cp = cosf(angle);

	angle = DEG2RADF(angles[ROLL]);
	float sr = sinf(angle);
	float cr = cosf(angle);

	// matrix = ( YAW * PITCH ) * ROLL
	matrix[0][0] = cp * cy;
	matrix[0][1] = cp * sy;
	matrix[0][2] = -sp;
	matrix[1][0] = sr * sp * cy + cr * -sy;
	matrix[1][1] = sr * sp * sy + cr * cy;
	matrix[1][2] = sr * cp;
	matrix[2][0] = (cr * sp * cy + -sr * -sy);
	matrix[2][1] = (cr * sp * sy + -sr * cy);
	matrix[2][2] = cr * cp;
	matrix[0][3] = 0.0;
	matrix[1][3] = 0.0;
	matrix[2][3] = 0.0;
}

void AngleQuaternion(const vec3_t angles, quat_t q, qboolean studio)
{
	float sr, sp, sy, cr, cp, cy;

	if ( studio )
	{
		SinCos(angles[ROLL] * 0.5f, &sy, &cy);
		SinCos(angles[YAW] * 0.5f, &sp, &cp);
		SinCos(angles[PITCH] * 0.5f, &sr, &cr);
	}
	else
	{
		SinCos(DEG2RADF(angles[YAW]) * 0.5f, &sy, &cy);
		SinCos(DEG2RADF(angles[PITCH]) * 0.5f, &sp, &cp);
		SinCos(DEG2RADF(angles[ROLL]) * 0.5f, &sr, &cr);
	}

	q[0] = sr * cp * cy - cr * sp * sy;  // X
	q[1] = cr * sp * cy + sr * cp * sy;  // Y
	q[2] = cr * cp * sy - sr * sp * cy;  // Z
	q[3] = cr * cp * cy + sr * sp * sy;  // W
}

void QuaternionAngle(const quat_t q, vec3_t angles)
{
	matrix3x4 mat;
	Matrix3x4_FromOriginQuat(mat, q, vec3_origin);
	Matrix3x4_AnglesFromMatrix(mat, angles);
}

float V_CalcFov(float* fov_x, float width, float height)
{
	float x, half_fov_y;

	if ( *fov_x < 1.0f || *fov_x > 179.0f )
	{
		*fov_x = 90.0f;  // default value
	}

	x = width / tanf(DEG2RADF(*fov_x) * 0.5f);
	half_fov_y = atanf(height / x);

	return RAD2DEGF(half_fov_y) * 2;
}

void V_AdjustFov(float* fov_x, float* fov_y, float width, float height, qboolean lock_x)
{
	float x, y;

	if ( width * 3 == 4 * height || width * 4 == height * 5 )
	{
		// 4:3 or 5:4 ratio
		return;
	}

	if ( lock_x )
	{
		*fov_y = 2 * atanf((width * 3) / (height * 4) * tanf(*fov_y * M_PI_F / 360.0f * 0.5f)) * 360 / M_PI_F;
		return;
	}

	y = V_CalcFov(fov_x, 640, 480);
	x = *fov_x;

	*fov_x = V_CalcFov(&y, height, width);
	if ( *fov_x < x )
		*fov_x = x;
	else
		*fov_y = y;
}

void RoundUpHullSize(vec3_t size)
{
	size_t i;
	size_t j;

	for ( i = 0; i < 3; i++ )
	{
		qboolean negative = false;
		float result, value;

		value = size[i];
		if ( value < 0.0f )
			negative = true;
		value = Q_ceil(fabs(value));
		result = Q_ceil(size[i]);

		// lookup hull table to find nearest supposed value
		for ( j = 0; j < NUM_HULL_ROUNDS; j++ )
		{
			if ( value > hull_table[j] )
				continue;  // ceil only

			if ( negative )
			{
				result = (value - hull_table[j]);
				if ( result <= HULL_PRECISION )
				{
					result = (float)(-hull_table[j]);
					break;
				}
			}
			else
			{
				result = (value - hull_table[j]);
				if ( result <= HULL_PRECISION )
				{
					result = hull_table[j];
					break;
				}
			}
		}

		size[i] = result;
	}
}

void ClearBounds(vec3_t mins, vec3_t maxs)
{
	mins[0] = mins[1] = mins[2] = FLT_MAX;
	maxs[0] = maxs[1] = maxs[2] = FLT_MIN;
}

qboolean BoundsAreCleared(vec3_t mins, vec3_t maxs)
{
	if ( mins[0] <= maxs[0] || mins[1] <= maxs[1] || mins[2] <= maxs[2] )
	{
		return false;
	}

	return true;
}

void AddPointToBounds(const vec3_t v, vec3_t mins, vec3_t maxs)
{
	for ( int i = 0; i < 3; i++ )
	{
		float val = v[i];

		if ( val < mins[i] )
		{
			mins[i] = val;
		}

		if ( val > maxs[i] )
		{
			maxs[i] = val;
		}
	}
}

qboolean BoundsIntersect(const vec3_t mins1, const vec3_t maxs1, const vec3_t mins2, const vec3_t maxs2)
{
	if ( mins1[0] > maxs2[0] || mins1[1] > maxs2[1] || mins1[2] > maxs2[2] )
	{
		return false;
	}

	if ( maxs1[0] < mins2[0] || maxs1[1] < mins2[1] || maxs1[2] < mins2[2] )
	{
		return false;
	}

	return true;
}

qboolean BoundsAndSphereIntersect(const vec3_t mins, const vec3_t maxs, const vec3_t origin, float radius)
{
	if ( mins[0] > origin[0] + radius || mins[1] > origin[1] + radius || mins[2] > origin[2] + radius )
	{
		return false;
	}

	if ( maxs[0] < origin[0] - radius || maxs[1] < origin[1] - radius || maxs[2] < origin[2] - radius )
	{
		return false;
	}

	return true;
}

qboolean
SphereIntersect(const vec3_t vSphereCenter, float fSphereRadiusSquared, const vec3_t vLinePt, const vec3_t vLineDir)
{
	vec3_t p;

	// translate sphere to origin.
	VectorSubtract(vLinePt, vSphereCenter, p);

	const float a = DotProduct(vLineDir, vLineDir);
	const float b = 2.0f * DotProduct(p, vLineDir);
	const float c = DotProduct(p, p) - fSphereRadiusSquared;
	const float insideSqr = (b * b) - (4.0f * a * c);

	// NFTODO: This is super arbitrary, make it consistent with other calculations?
	return insideSqr > 0.000001f;
}

float RadiusFromBounds(const vec3_t mins, const vec3_t maxs)
{
	vec3_t corner;

	for ( size_t i = 0; i < 3; i++ )
	{
		corner[i] = fabsf(mins[i]) > fabsf(maxs[i]) ? fabsf(mins[i]) : fabsf(maxs[i]);
	}

	return VectorLength(corner);
}

void ExpandBounds(vec3_t mins, vec3_t maxs, float offset)
{
	mins[0] -= offset;
	mins[1] -= offset;
	mins[2] -= offset;

	maxs[0] += offset;
	maxs[1] += offset;
	maxs[2] += offset;
}

float ApproachVal(float target, float value, float speed)
{
	float delta = target - value;

	if ( delta > speed )
	{
		value += speed;
	}
	else if ( delta < -speed )
	{
		value -= speed;
	}
	else
	{
		value = target;
	}

	return value;
}

void R_StudioSlerpBones(int numbones, vec4_t q1[], float pos1[][3], const vec4_t q2[], const float pos2[][3], float s)
{
	int i;

	s = bound(0.0f, s, 1.0f);

	for ( i = 0; i < numbones; i++ )
	{
		QuaternionSlerp(q1[i], q2[i], s, q1[i]);
		VectorLerp(pos1[i], s, pos2[i], pos1[i]);
	}
}
