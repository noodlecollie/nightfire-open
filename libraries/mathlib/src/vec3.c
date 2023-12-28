#include "MathLib/vec3.h"
#include "MathLib/angles.h"

const vec3_t vec3_origin = {0, 0, 0};

qboolean VectorIsOnAxis(const vec3_t v)
{
	size_t count = 0;

	for ( size_t i = 0; i < 3; i++ )
	{
		if ( fabsf(v[i]) < 0.0000001f )
		{
			count++;
		}
	}

	// the zero vector will be on axis.
	return count > 1;
}

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
