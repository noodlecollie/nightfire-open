#pragma once

#include <assert.h>
#include "PlatformDefs/utils.h"
#include "PlatformDefs/static_assert.h"
#include "MathLib/mathtypes.h"

typedef vec_t vec2_t[2];

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

static inline void Vector2Lerp(const vec2_t v1, vec_t lerp, const vec2_t v2, vec2_t c)
{
	c[0] = v1[0] + lerp * (v2[0] - v1[0]);
	c[1] = v1[1] + lerp * (v2[1] - v1[1]);
}

#ifdef __cplusplus
class Vector2D
{
public:
	static constexpr size_t SIZE = sizeof(vec2_t);

	inline Vector2D(void) :
		x(0.0f),
		y(0.0f)
	{
		static_assert(sizeof(*this) == SIZE, "Expected class to match the size of vec2_t");
	}

	inline Vector2D(float inX, float inY) :
		x(inX),
		y(inY)
	{
	}

	inline Vector2D operator+(const Vector2D& v) const
	{
		return Vector2D(x + v.x, y + v.y);
	}

	inline Vector2D operator-(const Vector2D& v) const
	{
		return Vector2D(x - v.x, y - v.y);
	}

	inline Vector2D operator*(float fl) const
	{
		return Vector2D(x * fl, y * fl);
	}

	inline Vector2D operator/(float fl) const
	{
		return Vector2D(x / fl, y / fl);
	}

	operator float*()
	{
		return &x;
	}

	operator const float*() const
	{
		return &x;
	}

	inline float Length(void) const
	{
		return sqrtf((x * x) + (y * y));
	}

	inline Vector2D Normalize(void) const
	{
		float flLen = Length();

		if ( flLen == 0 )
		{
			return Vector2D(0, 0);
		}
		else
		{
			flLen = 1 / flLen;
			return Vector2D(x * flLen, y * flLen);
		}
	}

	inline float& operator[](size_t index)
	{
		static float g_Dummy = 0.0f;

#ifndef NDEBUG
		if ( index >= SIZE )
		{
			assert(false);
		}
#endif

		return index < SIZE ? operator float*()[index] : g_Dummy;
	}

	inline const float& operator[](size_t index) const
	{
		static const float g_Dummy = 0.0f;

#ifndef NDEBUG
		if ( index >= SIZE )
		{
			assert(false);
		}
#endif

		return index < SIZE ? operator const float*()[index] : g_Dummy;
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
#endif  // __cplusplus
