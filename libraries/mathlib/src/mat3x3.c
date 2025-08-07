#include "MathLib/mat3x3.h"
#include "MathLib/angles.h"

const matrix3x3 m_matrix3x3_identity = {
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1},
};

void Matrix3x3_Copy(matrix3x3 out, const matrix3x3 in)
{
	memcpy(out, in, sizeof(matrix3x3));
}
