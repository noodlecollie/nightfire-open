#pragma once

#include <assert.h>
#include "BuildPlatform/Typedefs.h"
#include "BuildPlatform/Utils.h"
#include "BuildPlatform/StaticAssert.h"
#include "MathLib/mathdefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef vec_t vec2_t[2];

static inline float Vector2DotProduct(const vec2_t x, const vec2_t y)
{
	return (x[0] * y[0]) + (x[1] * y[1]);
}

static inline float Vector2Cross(const vec2_t x, const vec2_t y)
{
	return (x[0] * y[1]) - (x[1] * y[0]);
}

static inline void Vector2Add(const vec2_t a, const vec2_t b, vec2_t c)
{
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
}

static inline void Vector2Subtract(const vec2_t a, const vec2_t b, vec2_t c)
{
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
}

static inline void Vector2Copy(const vec2_t a, vec2_t b)
{
	b[0] = a[0];
	b[1] = a[1];
}

static inline void Vector2Average(const vec2_t a, const vec2_t b, vec2_t c)
{
	c[0] = (a[0] + b[0]) * 0.5f;
	c[1] = (a[1] + b[1]) * 0.5f;
};

static inline void Vector2Set(vec2_t v, vec_t x, vec_t y)
{
	v[0] = x;
	v[1] = y;
}

static inline void Vector2Lerp(const vec2_t v1, float lerp, const vec2_t v2, vec2_t c)
{
	c[0] = v1[0] + (lerp * (v2[0] - v1[0]));
	c[1] = v1[1] + (lerp * (v2[1] - v1[1]));
}

static inline void Vector2Scale(const vec2_t in, float scale, vec2_t out)
{
	out[0] = in[0] * scale;
	out[1] = in[1] * scale;
}

static inline void Vector2Divide(const vec2_t in, float d, vec2_t out)
{
	assert(d != 0.0f);
	Vector2Scale(in, (1.0f / d), out);
}

static inline float Vector2LengthSquared(const vec2_t a)
{
	return Vector2DotProduct(a, a);
}

static inline float Vector2Length(const vec2_t a)
{
	return sqrtf(Vector2LengthSquared(a));
}

static inline float Vector2Normalize(vec2_t v)
{
	const float length = Vector2Length(v);
	const float iLength = length != 0.0f ? (1.0f / length) : 0.0f;

	v[0] *= iLength;
	v[1] *= iLength;

	return length;
}

#ifdef __cplusplus
}  // extern "C"
#endif

#ifdef __cplusplus
class Vector2D
{
public:
	static constexpr size_t SIZE_BYTES = sizeof(vec2_t);
	static constexpr size_t SIZE_ELEMENTS = SIZE_BYTES / sizeof(vec_t);

	Vector2D() :
		x(0.0f),
		y(0.0f)
	{
		static_assert(sizeof(*this) == SIZE_BYTES, "Expected class to match the size of vec2_t");
	}

	Vector2D(vec_t inX, vec_t inY) :
		x(inX),
		y(inY)
	{
	}

	Vector2D operator+(const Vector2D& v) const
	{
		Vector2D out;
		Vector2Add(RawData(), v.RawData(), out.RawData());
		return out;
	}

	Vector2D operator-(const Vector2D& v) const
	{
		Vector2D out;
		Vector2Subtract(RawData(), v.RawData(), out.RawData());
		return out;
	}

	Vector2D operator*(float fl) const
	{
		Vector2D out;
		Vector2Scale(RawData(), fl, out.RawData());
		return out;
	}

	Vector2D operator/(float fl) const
	{
		assert(fl != 0.0f);

		Vector2D out;
		Vector2Divide(RawData(), fl, out.RawData());
		return out;
	}

	Vector2D& operator+=(const Vector2D& other)
	{
		*this = *this + other;
		return *this;
	}

	Vector2D& operator-=(const Vector2D& other)
	{
		*this = *this - other;
		return *this;
	}

	Vector2D& operator*=(float fl)
	{
		*this = *this * fl;
		return *this;
	}

	Vector2D& operator/=(float fl)
	{
		*this = *this / fl;
		return *this;
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

	float Length() const
	{
		return Vector2Length(RawData());
	}

	float LengthSquared() const
	{
		return Vector2LengthSquared(RawData());
	}

	Vector2D Normalize() const
	{
		Vector2D out(*this);
		Vector2Normalize(out);
		return out;
	}

	vec_t& operator[](size_t index)
	{
		static vec_t g_Dummy = 0.0f;

		assert(index < SIZE_ELEMENTS);

		// NFTODO: Due to the note below about member variables,
		// and to MSVC not liking (vec2_t*)(&x)[index],
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
		// and to MSVC not liking (vec2_t*)(&x)[index],
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

			default:
			{
				return g_Dummy;
			}
		}
	}

	// This is lame and an array would be better, but lots of things will
	// be relying on being able to index by x and y. We can't use an array
	// along with some alias references, because these would change the
	// size of the class. The static_assert should give us a level of
	// confidence for now, but evaluate whether it'd be worth the effort
	// to swap these out.
	vec_t x;
	vec_t y;
};

inline float DotProduct(const Vector2D& a, const Vector2D& b)
{
	return (a.x * b.x) + (a.y * b.y);
}
inline Vector2D operator*(float fl, const Vector2D& v)
{
	return v * fl;
}
#endif  // __cplusplus
