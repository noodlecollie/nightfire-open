#include "MathLib/mathdefs.h"
#include "MathLib/angles.h"

static vec_t NormalizeAngleValue(vec_t val)
{
	if ( val < 0.0f )
	{
		return -(fmodf((-val) + 180.0f, 360.0f) - 180.0f);
	}
	else
	{
		// We do a check for <= 180.0f here, otherwise
		// 180 turns into -180. We want this the other
		// way around for the negative case above,
		// but we don't want to flip it here.
		return val <= 180.0f ? val : fmodf(val + 180.0f, 360.0f) - 180.0f;
	}
}

void NormalizeAngles(vec3_t angles)
{
	NormalizeAnglesOut(angles, angles);
}

void NormalizeAnglesOut(const vec3_t in, vec3_t out)
{
	out[PITCH] = NormalizeAngleValue(in[PITCH]);
	out[YAW] = NormalizeAngleValue(in[YAW]);
	out[ROLL] = NormalizeAngleValue(in[ROLL]);
}

// FIXME:  Use Quaternions to avoid discontinuities
void InterpolateAngles(const vec3_t start, const vec3_t end, vec3_t output, float frac)
{
	vec3_t normStart;
	vec3_t normEnd;

	NormalizeAnglesOut(start, normStart);
	NormalizeAnglesOut(end, normEnd);

	for ( int i = 0; i < 3; i++ )
	{
		const float ang1 = normStart[i];
		const float ang2 = normEnd[i];

		float d = ang2 - ang1;

		if ( d > 180 )
		{
			d -= 360;
		}

		else if ( d < -180 )
		{
			d += 360;
		}

		output[i] = ang1 + d * frac;
	}

	NormalizeAngles(output);
}
