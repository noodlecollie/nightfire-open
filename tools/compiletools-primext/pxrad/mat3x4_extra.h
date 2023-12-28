#pragma once

#include "MathLib/mat3x4.h"
#include "MathLib/vec3.h"
#include "CompileTools/bspfile.h"

static inline bool Matrix3x4_Invert_Full(matrix3x4 out, const matrix3x4 in)
{
	vec_t texplanes[2][4];
	vec_t faceplane[4];
	vec_t texaxis[2][3];
	vec_t normalaxis[3];
	vec_t det, sqrlen1, sqrlen2, sqrlen3;
	vec_t dot_epsilon = NORMAL_EPSILON * NORMAL_EPSILON;
	vec_t texorg[3];

	for ( int i = 0; i < 4; i++ )
	{
		texplanes[0][i] = in[i][0];
		texplanes[1][i] = in[i][1];
		faceplane[i] = in[i][2];
	}

	sqrlen1 = DotProduct(texplanes[0], texplanes[0]);
	sqrlen2 = DotProduct(texplanes[1], texplanes[1]);
	sqrlen3 = DotProduct(faceplane, faceplane);

	if ( sqrlen1 <= dot_epsilon || sqrlen2 <= dot_epsilon || sqrlen3 <= dot_epsilon )
	{
		// s gradient, t gradient or face normal is too close to 0
		return false;
	}

	CrossProduct(texplanes[0], texplanes[1], normalaxis);
	det = DotProduct(normalaxis, faceplane);

	if ( det * det <= sqrlen1 * sqrlen2 * sqrlen3 * dot_epsilon )
	{
		// s gradient, t gradient and face normal are coplanar
		return false;
	}

	VectorScale(normalaxis, 1.0f / det, normalaxis);
	CrossProduct(texplanes[1], faceplane, texaxis[0]);
	VectorScale(texaxis[0], 1.0f / det, texaxis[0]);

	CrossProduct(faceplane, texplanes[0], texaxis[1]);
	VectorScale(texaxis[1], 1.0f / det, texaxis[1]);

	VectorScale(normalaxis, -faceplane[3], texorg);
	VectorMA(texorg, -texplanes[0][3], texaxis[0], texorg);
	VectorMA(texorg, -texplanes[1][3], texaxis[1], texorg);

	VectorCopy(texaxis[0], out[0]);
	VectorCopy(texaxis[1], out[1]);
	VectorCopy(normalaxis, out[2]);
	VectorCopy(texorg, out[3]);

	return true;
}
