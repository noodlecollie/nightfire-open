#include "MathLib/mat3x4.h"
#include "MathLib/angles.h"

const matrix3x4 m_matrix3x4_identity = {
	{1, 0, 0, 0},  // PITCH	[forward], org[0]
	{0, 1, 0, 0},  // YAW	[right]  , org[1]
	{0, 0, 1, 0},  // ROLL	[up]     , org[2]
};

void Matrix3x4_Copy(matrix3x4 out, const matrix3x4 in)
{
	memcpy(out, in, sizeof(matrix3x4));
}

void Matrix3x4_VectorTransform(const matrix3x4 in, const float v[3], float out[3])
{
	out[0] = v[0] * in[0][0] + v[1] * in[0][1] + v[2] * in[0][2] + in[0][3];
	out[1] = v[0] * in[1][0] + v[1] * in[1][1] + v[2] * in[1][2] + in[1][3];
	out[2] = v[0] * in[2][0] + v[1] * in[2][1] + v[2] * in[2][2] + in[2][3];
}

void Matrix3x4_VectorITransform(const matrix3x4 in, const float v[3], float out[3])
{
	vec3_t dir;

	dir[0] = v[0] - in[0][3];
	dir[1] = v[1] - in[1][3];
	dir[2] = v[2] - in[2][3];

	out[0] = dir[0] * in[0][0] + dir[1] * in[1][0] + dir[2] * in[2][0];
	out[1] = dir[0] * in[0][1] + dir[1] * in[1][1] + dir[2] * in[2][1];
	out[2] = dir[0] * in[0][2] + dir[1] * in[1][2] + dir[2] * in[2][2];
}

void Matrix3x4_VectorRotate(const matrix3x4 in, const float v[3], float out[3])
{
	out[0] = v[0] * in[0][0] + v[1] * in[0][1] + v[2] * in[0][2];
	out[1] = v[0] * in[1][0] + v[1] * in[1][1] + v[2] * in[1][2];
	out[2] = v[0] * in[2][0] + v[1] * in[2][1] + v[2] * in[2][2];
}

void Matrix3x4_VectorIRotate(const matrix3x4 in, const float v[3], float out[3])
{
	out[0] = v[0] * in[0][0] + v[1] * in[1][0] + v[2] * in[2][0];
	out[1] = v[0] * in[0][1] + v[1] * in[1][1] + v[2] * in[2][1];
	out[2] = v[0] * in[0][2] + v[1] * in[1][2] + v[2] * in[2][2];
}

void Matrix3x4_ConcatTransforms(matrix3x4 out, const matrix3x4 in1, const matrix3x4 in2)
{
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] + in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] + in1[0][2] * in2[2][2];
	out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] + in1[0][2] * in2[2][3] + in1[0][3];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] + in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] + in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] + in1[1][2] * in2[2][2];
	out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] + in1[1][2] * in2[2][3] + in1[1][3];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] + in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] + in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] + in1[2][2] * in2[2][2];
	out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] + in1[2][2] * in2[2][3] + in1[2][3];
}

void Matrix3x4_SetOrigin(matrix3x4 out, float x, float y, float z)
{
	out[0][3] = x;
	out[1][3] = y;
	out[2][3] = z;
}

void Matrix3x4_OriginFromMatrix(const matrix3x4 in, vec3_t out)
{
	out[0] = in[0][3];
	out[1] = in[1][3];
	out[2] = in[2][3];
}

void Matrix3x4_FromOriginQuat(matrix3x4 out, const quat_t quaternion, const vec3_t origin)
{
	Matrix3x4_FromQuat(out, quaternion);

	out[0][3] = origin[0];
	out[1][3] = origin[1];
	out[2][3] = origin[2];
}

void Matrix3x4_FromQuat(matrix3x4 out, const quat_t quaternion)
{
	out[0][0] = 1.0f - 2.0f * quaternion[1] * quaternion[1] - 2.0f * quaternion[2] * quaternion[2];
	out[1][0] = 2.0f * quaternion[0] * quaternion[1] + 2.0f * quaternion[3] * quaternion[2];
	out[2][0] = 2.0f * quaternion[0] * quaternion[2] - 2.0f * quaternion[3] * quaternion[1];

	out[0][1] = 2.0f * quaternion[0] * quaternion[1] - 2.0f * quaternion[3] * quaternion[2];
	out[1][1] = 1.0f - 2.0f * quaternion[0] * quaternion[0] - 2.0f * quaternion[2] * quaternion[2];
	out[2][1] = 2.0f * quaternion[1] * quaternion[2] + 2.0f * quaternion[3] * quaternion[0];

	out[0][2] = 2.0f * quaternion[0] * quaternion[2] + 2.0f * quaternion[3] * quaternion[1];
	out[1][2] = 2.0f * quaternion[1] * quaternion[2] - 2.0f * quaternion[3] * quaternion[0];
	out[2][2] = 1.0f - 2.0f * quaternion[0] * quaternion[0] - 2.0f * quaternion[1] * quaternion[1];
}

void Matrix3x4_CreateFromEntityScale3f(matrix3x4 out, const vec3_t angles, const vec3_t origin, const vec3_t scale)
{
	float angle, sr, sp, sy, cr, cp, cy;

	if ( angles[ROLL] )
	{
		angle = angles[YAW] * (M_PI2_F / 360.0f);
		SinCos(angle, &sy, &cy);
		angle = angles[PITCH] * (M_PI2_F / 360.0f);
		SinCos(angle, &sp, &cp);
		angle = angles[ROLL] * (M_PI2_F / 360.0f);
		SinCos(angle, &sr, &cr);

		out[0][0] = (cp * cy) * scale[0];
		out[0][1] = (sr * sp * cy + cr * -sy) * scale[1];
		out[0][2] = (cr * sp * cy + -sr * -sy) * scale[2];
		out[0][3] = origin[0];
		out[1][0] = (cp * sy) * scale[0];
		out[1][1] = (sr * sp * sy + cr * cy) * scale[1];
		out[1][2] = (cr * sp * sy + -sr * cy) * scale[2];
		out[1][3] = origin[1];
		out[2][0] = (-sp) * scale[0];
		out[2][1] = (sr * cp) * scale[1];
		out[2][2] = (cr * cp) * scale[2];
		out[2][3] = origin[2];
	}
	else if ( angles[PITCH] )
	{
		angle = angles[YAW] * (M_PI2_F / 360.0f);
		SinCos(angle, &sy, &cy);
		angle = angles[PITCH] * (M_PI2_F / 360.0f);
		SinCos(angle, &sp, &cp);

		out[0][0] = (cp * cy) * scale[0];
		out[0][1] = (-sy) * scale[1];
		out[0][2] = (sp * cy) * scale[2];
		out[0][3] = origin[0];
		out[1][0] = (cp * sy) * scale[0];
		out[1][1] = (cy)*scale[1];
		out[1][2] = (sp * sy) * scale[2];
		out[1][3] = origin[1];
		out[2][0] = (-sp) * scale[0];
		out[2][1] = 0.0f;
		out[2][2] = (cp)*scale[2];
		out[2][3] = origin[2];
	}
	else if ( angles[YAW] )
	{
		angle = angles[YAW] * (M_PI2_F / 360.0f);
		SinCos(angle, &sy, &cy);

		out[0][0] = (cy)*scale[0];
		out[0][1] = (-sy) * scale[1];
		out[0][2] = 0.0f;
		out[0][3] = origin[0];
		out[1][0] = (sy)*scale[0];
		out[1][1] = (cy)*scale[1];
		out[1][2] = 0.0f;
		out[1][3] = origin[1];
		out[2][0] = 0.0f;
		out[2][1] = 0.0f;
		out[2][2] = scale[2];
		out[2][3] = origin[2];
	}
	else
	{
		out[0][0] = scale[0];
		out[0][1] = 0.0f;
		out[0][2] = 0.0f;
		out[0][3] = origin[0];
		out[1][0] = 0.0f;
		out[1][1] = scale[1];
		out[1][2] = 0.0f;
		out[1][3] = origin[1];
		out[2][0] = 0.0f;
		out[2][1] = 0.0f;
		out[2][2] = scale[2];
		out[2][3] = origin[2];
	}
}

vec_t Matrix3x4_CalcSign(const matrix3x4 in)
{
	vec3_t out;

	out[0] = (in[0][1] * in[1][2]) - (in[0][2] * in[1][1]);
	out[1] = (in[0][2] * in[1][0]) - (in[0][0] * in[1][2]);
	out[2] = (in[0][0] * in[1][1]) - (in[0][1] * in[1][0]);

	return (out[0] * in[2][0]) + (out[1] * in[2][1]) + (out[2] * in[2][2]);
}

void Matrix3x4_TransformAABB(const matrix3x4 world, const vec3_t mins, const vec3_t maxs, vec3_t absmin, vec3_t absmax)
{
	vec3_t localCenter, localExtents;
	vec3_t worldCenter, worldExtents;

	VectorAverage(mins, maxs, localCenter);
	VectorSubtract(maxs, localCenter, localExtents);

	Matrix3x4_VectorTransform(world, localCenter, worldCenter);
	worldExtents[0] = (float)DotProductAbs(localExtents, world[0]);  // auto-transposed!
	worldExtents[1] = (float)DotProductAbs(localExtents, world[1]);
	worldExtents[2] = (float)DotProductAbs(localExtents, world[2]);

	VectorSubtract(worldCenter, worldExtents, absmin);
	VectorAdd(worldCenter, worldExtents, absmax);
}
