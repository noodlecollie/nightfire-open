#pragma once

#include <assert.h>
#include "BuildPlatform/Typedefs.h"
#include "MathLib/mathdefs.h"
#include "MathLib/mathlib.h"
#include "MathLib/vec2.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	VEC3_X = 0,
	VEC3_Y,
	VEC3_Z
} Vec3Axis_e;

typedef vec_t vec3_t[3];

extern const vec3_t vec3_origin;

static inline qboolean VectorIsNAN(const vec3_t v)
{
	return IS_NAN(v[0]) || IS_NAN(v[1]) || IS_NAN(v[2]);
}

static inline float DotProduct(const vec3_t x, const vec3_t y)
{
	return (x[0] * y[0]) + (x[1] * y[1]) + (x[2] * y[2]);
}

static inline int DotProductAbs(const vec3_t x, const vec3_t y)
{
	return abs((int)(x[0] * y[0])) + abs((int)(x[1] * y[1])) + abs((int)(x[2] * y[2]));
}

static inline float DotProductFabs(const vec3_t x, const vec3_t y)
{
	return fabsf(x[0] * y[0]) + fabsf(x[1] * y[1]) + fabsf(x[2] * y[2]);
};

static inline double DotProductPrecise(const vec3_t x, const vec3_t y)
{
	return ((double)(x[0]) * (double)(y[0])) + ((double)(x[1]) * (double)(y[1])) + ((double)(x[2]) * (double)(y[2]));
}

static inline void CrossProduct(const vec3_t a, const vec3_t b, vec3_t c)
{
	c[0] = (a[1] * b[2]) - (a[2] * b[1]);
	c[1] = (a[2] * b[0]) - (a[0] * b[2]);
	c[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

static inline void VectorMultiply(const vec3_t a, const vec3_t b, vec3_t c)
{
	c[0] = a[0] * b[0];
	c[1] = a[1] * b[1];
	c[2] = a[2] * b[2];
}

static inline void VectorAdd(const vec3_t a, const vec3_t b, vec3_t c)
{
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	c[2] = a[2] + b[2];
}

static inline void VectorAddScalar(const vec3_t a, vec_t b, vec3_t c)
{
	c[0] = a[0] + b;
	c[1] = a[1] + b;
	c[2] = a[2] + b;
}

static inline void VectorSubtract(const vec3_t a, const vec3_t b, vec3_t c)
{
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
}

static inline void VectorCopy(const vec3_t source, vec3_t dest)
{
	dest[0] = source[0];
	dest[1] = source[1];
	dest[2] = source[2];
}

static inline void VectorScale(const vec3_t in, float scale, vec3_t out)
{
	out[0] = in[0] * scale;
	out[1] = in[1] * scale;
	out[2] = in[2] * scale;
}

static inline qboolean VectorCompare(const vec3_t v1, const vec3_t v2)
{
	return v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2];
}

static inline void VectorDivide(const vec3_t in, float d, vec3_t out)
{
	assert(d != 0.0f);
	VectorScale(in, (1.0f / d), out);
}

static inline vec_t VectorMax(const vec3_t a)
{
	return Q_max(a[0], Q_max(a[1], a[2]));
}

static inline vec_t VectorMin(const vec3_t a)
{
	return Q_min(a[0], Q_min(a[1], a[2]));
}

static inline float VectorAvg(const vec3_t a)
{
	return (a[0] + a[1] + a[2]) / 3.0f;
}

static inline float VectorLengthSquared(const vec3_t a)
{
	return DotProduct(a, a);
}

static inline float VectorLength(const vec3_t a)
{
	return sqrtf(VectorLengthSquared(a));
}

static inline float VectorDistanceSquared(const vec3_t a, const vec3_t b)
{
	return ((a[0] - b[0]) * (a[0] - b[0])) + ((a[1] - b[1]) * (a[1] - b[1])) + ((a[2] - b[2]) * (a[2] - b[2]));
}

static inline float VectorDistance(const vec3_t a, const vec3_t b)
{
	return sqrtf(VectorDistanceSquared(a, b));
}

static inline void VectorAverage(const vec3_t a, const vec3_t b, vec3_t o)
{
	o[0] = (a[0] + b[0]) * 0.5f;
	o[1] = (a[1] + b[1]) * 0.5f;
	o[2] = (a[2] + b[2]) * 0.5f;
}

static inline void VectorSet(vec3_t v, vec_t x, vec_t y, vec_t z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

static inline void VectorFill(vec3_t v, vec_t val)
{
	VectorSet(v, val, val, val);
}

static inline void VectorClear(vec3_t v)
{
	VectorSet(v, 0.0f, 0.0f, 0.0f);
}

static inline void VectorLerp(const vec3_t v1, float lerp, const vec3_t v2, vec3_t c)
{
	c[0] = v1[0] + (lerp * (v2[0] - v1[0]));
	c[1] = v1[1] + (lerp * (v2[1] - v1[1]));
	c[2] = v1[2] + (lerp * (v2[2] - v1[2]));
}

// NFTODO: Make variants of this and similar functions where those suffixed
// with "Self" apply the operation to the input, and other functions have
// specific inputs and outputs.
static inline float VectorNormalize(vec3_t v)
{
	const float length = VectorLength(v);
	const float iLength = length != 0.0f ? (1.0f / length) : 0.0f;

	v[0] *= iLength;
	v[1] *= iLength;
	v[2] *= iLength;

	return length;
}

static inline void VectorNormalize2(const vec3_t v, vec3_t dest)
{
	float ilength = VectorLength(v);

	if ( ilength )
	{
		ilength = 1.0f / ilength;
	}

	dest[0] = v[0] * ilength;
	dest[1] = v[1] * ilength;
	dest[2] = v[2] * ilength;
}

static inline void VectorNormalizeFast(vec3_t v)
{
	const float ilength = rsqrt(VectorLengthSquared(v));

	v[0] *= ilength;
	v[1] *= ilength;
	v[2] *= ilength;
}

static inline void VectorNegate(const vec3_t x, vec3_t y)
{
	y[0] = -(x[0]);
	y[1] = -(x[1]);
	y[2] = -(x[2]);
}

static inline void VectorM(float scale1, const vec3_t b1, vec3_t c)
{
	c[0] = (scale1 * b1[0]);
	c[1] = (scale1 * b1[1]);
	c[2] = (scale1 * b1[2]);
}

static inline void VectorMA(const vec3_t a, float scale, const vec3_t b, vec3_t c)
{
	c[0] = a[0] + (scale * b[0]);
	c[1] = a[1] + (scale * b[1]);
	c[2] = a[2] + (scale * b[2]);
}

static inline void VectorMAM(float scale1, const vec3_t b1, float scale2, const vec3_t b2, vec3_t c)
{
	c[0] = (scale1 * b1[0]) + (scale2 * b2[0]);
	c[1] = (scale1 * b1[1]) + (scale2 * b2[1]);
	c[2] = (scale1 * b1[2]) + (scale2 * b2[2]);
}

static inline void
VectorMAMAM(float scale1, const vec3_t b1, float scale2, const vec3_t b2, float scale3, const vec3_t b3, vec3_t c)
{
	c[0] = (scale1 * b1[0]) + (scale2 * b2[0]) + (scale3 * b3[0]);
	c[1] = (scale1 * b1[1]) + (scale2 * b2[1]) + (scale3 * b3[1]);
	c[2] = (scale1 * b1[2]) + (scale2 * b2[2]) + (scale3 * b3[2]);
}

static inline qboolean VectorIsNull(const vec3_t v)
{
	return v[0] == 0.0f && v[1] == 0.0f && v[2] == 0.0f;
}

static inline void VectorCompareMax(const vec3_t a, const vec3_t b, vec3_t c)
{
	c[0] = Q_max(a[0], b[0]);
	c[1] = Q_max(a[1], b[1]);
	c[2] = Q_max(a[2], b[2]);
};

static inline void VectorCompareMin(const vec3_t a, const vec3_t b, vec3_t c)
{
	c[0] = Q_min(a[0], b[0]);
	c[1] = Q_min(a[1], b[1]);
	c[2] = Q_min(a[2], b[2]);
};

qboolean VectorIsOnAxis(const vec3_t v);
float VectorNormalizeLength(const vec3_t v, vec3_t out);
qboolean VectorCompareEpsilon(const vec3_t vec1, const vec3_t vec2, vec_t epsilon);
void VectorVectors(const vec3_t forward, vec3_t right, vec3_t up);

#ifdef __cplusplus
}  // extern "C"
#endif

#ifdef __cplusplus
// NFTODO: Rename this to Vector3D
class Vector
{
public:
	static constexpr size_t SIZE_BYTES = sizeof(vec3_t);
	static constexpr size_t SIZE_ELEMENTS = SIZE_BYTES / sizeof(vec_t);

	constexpr Vector() :
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{
		static_assert(sizeof(*this) == SIZE_BYTES, "Expected class to match the size of vec3_t");
	}

	constexpr Vector(vec_t inX, vec_t inY, vec_t inZ) :
		x(inX),
		y(inY),
		z(inZ)
	{
	}

	template<size_t INSIZE>
	constexpr Vector(const vec_t (&in)[INSIZE]) :
		x(in[0]),
		y(in[1]),
		z(in[2])
	{
		static_assert(INSIZE == SIZE_ELEMENTS, "Input array size does not match vector size");
	}

	constexpr Vector(const vec_t* values, size_t size) :
		x((values && size > 0) ? values[size] : 0.0f),
		y((values && size > 1) ? values[size] : 0.0f),
		z((values && size > 2) ? values[size] : 0.0f)
	{
	}

	constexpr Vector(const Vector& other) :
		x(other.x),
		y(other.y),
		z(other.z)
	{
	}

	Vector operator-() const
	{
		return Vector(-x, -y, -z);
	}

	bool operator==(const Vector& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	bool operator!=(const Vector& v) const
	{
		return !(*this == v);
	}

	Vector& operator=(const Vector& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}

	Vector operator+(const Vector& v) const
	{
		Vector out;
		VectorAdd(RawData(), v.RawData(), out.RawData());
		return out;
	}

	Vector operator-(const Vector& v) const
	{
		Vector out;
		VectorSubtract(RawData(), v.RawData(), out.RawData());
		return out;
	}

	Vector operator*(float fl) const
	{
		Vector out;
		VectorScale(RawData(), fl, out.RawData());
		return out;
	}

	Vector operator/(float fl) const
	{
		assert(fl != 0.0f);

		Vector out;
		VectorDivide(RawData(), fl, out.RawData());
		return out;
	}

	Vector& operator+=(const Vector& other)
	{
		*this = *this + other;
		return *this;
	}

	Vector& operator-=(const Vector& other)
	{
		*this = *this - other;
		return *this;
	}

	Vector& operator*=(float fl)
	{
		*this = *this * fl;
		return *this;
	}

	Vector& operator/=(float fl)
	{
		*this = *this / fl;
		return *this;
	}

	template<size_t OUTSIZE>
	void CopyToArray(float (&out)[OUTSIZE]) const
	{
		static_assert(OUTSIZE == SIZE_ELEMENTS, "Output array size does not match vector size");

		out[0] = x;
		out[1] = y;
		out[2] = z;
	}

	void CopyToArrayRaw(float* out, size_t outSize) const
	{
		if ( !out )
		{
			return;
		}

		if ( outSize >= 1 )
		{
			out[0] = x;
		}

		if ( outSize >= 2 )
		{
			out[1] = y;
		}

		if ( outSize >= 3 )
		{
			out[2] = z;
		}
	}

	float LengthSquared() const
	{
		return VectorLengthSquared(RawData());
	}

	float Length() const
	{
		return VectorLength(RawData());
	}

	const vec_t* RawData() const
	{
		return &x;
	}

	vec_t* RawData()
	{
		return &x;
	}

	operator vec_t*()
	{
		return RawData();
	}

	operator const vec_t*() const
	{
		return RawData();
	}

	Vector Normalize() const
	{
		Vector out(*this);
		VectorNormalize(out);
		return out;
	}

	Vector2D Make2D() const
	{
		return Vector2D(x, y);
	}

	float Length2D() const
	{
		return Make2D().Length();
	}

	vec_t& operator[](size_t index)
	{
		static vec_t g_Dummy = 0.0f;

		assert(index < SIZE_ELEMENTS);

		// NFTODO: Due to the note below about member variables,
		// and to MSVC not liking (vec3_t*)(&x)[index],
		// we have to do this switch statement here. The proper
		// fix for this would be to use an array in the
		// class itself, but that would require a lot of refactoring
		// of the code that uses this class, and I've already done
		// weeks worth of tedious vector-related refactoring
		// by this point, so my spirit can't take it any more.
		switch ( index )
		{
			case 0:
			{
				return x;
			}

			case 1:
			{
				return y;
			}

			case 2:
			{
				return z;
			}

			default:
			{
				return g_Dummy;
			}
		}
	}

	const vec_t& operator[](size_t index) const
	{
		static const vec_t g_Dummy = 0.0f;

		assert(index < SIZE_ELEMENTS);

		// NFTODO: Due to the note below about member variables,
		// and to MSVC not liking (vec3_t*)(&x)[index],
		// we have to do this switch statement here. The proper
		// fix for this would be to use an array in the
		// class itself, but that would require a lot of refactoring
		// of the code that uses this class, and I've already done
		// weeks worth of tedious vector-related refactoring
		// by this point, so my spirit can't take it any more.
		switch ( index )
		{
			case 0:
			{
				return x;
			}

			case 1:
			{
				return y;
			}

			case 2:
			{
				return z;
			}

			default:
			{
				return g_Dummy;
			}
		}
	}

	// This is lame and an array would be better, but lots of things will
	// be relying on being able to index by x/y/z. We can't use an array
	// along with some alias references, because these would change the
	// size of the class. The static_assert should give us a level of
	// confidence for now, but evaluate whether it'd be worth the effort
	// to swap these out.
	vec_t x;
	vec_t y;
	vec_t z;
};

static inline Vector operator*(float fl, const Vector& v)
{
	return v * fl;
}

static inline Vector CrossProduct(const Vector& a, const Vector& b)
{
	Vector out;
	CrossProduct(a.RawData(), b.RawData(), out.RawData());
	return out;
}
#endif
