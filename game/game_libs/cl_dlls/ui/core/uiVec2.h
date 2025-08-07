#pragma once

#include <cstddef>
#include "debug_assert.h"

struct UIVec2
{
	static constexpr size_t NUM_COMPONENTS = 2;

	inline UIVec2(int inX = 0, int inY = 0) :
		x(inX),
		y(inY)
	{
	}

	inline UIVec2(const UIVec2& other)
	{
		*this = other;
	}

	inline UIVec2& operator=(const UIVec2& other) = default;

	inline bool operator==(const UIVec2& other) const
	{
		return x == other.x && y == other.y;
	}

	inline bool operator!=(const UIVec2& other) const
	{
		return !(*this == other);
	}

	inline int& operator[](size_t index)
	{
		if ( index < NUM_COMPONENTS )
		{
			return index == 0 ? x : y;
		}
		else
		{
			static int dummy = 0;

			ASSERT(false);
			return dummy;
		}
	}

	inline const int& operator[](size_t index) const
	{
		if ( index < NUM_COMPONENTS )
		{
			return index == 0 ? x : y;
		}
		else
		{
			static int dummy = 0;

			ASSERT(false);
			return dummy;
		}
	}

	inline UIVec2 operator+(const UIVec2& other) const
	{
		return UIVec2(x + other.x, y + other.y);
	}

	inline UIVec2& operator+=(const UIVec2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	inline UIVec2 operator-(const UIVec2& other) const
	{
		return UIVec2(x - other.x, y - other.y);
	}

	inline UIVec2& operator-=(const UIVec2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline UIVec2 operator*(float factor) const
	{
		return UIVec2(static_cast<int>(x * factor), static_cast<int>(y * factor));
	}

	inline UIVec2& operator*=(float factor)
	{
		x = static_cast<int>(static_cast<float>(x) * factor);
		y = static_cast<int>(static_cast<float>(y) * factor);
		return *this;
	}

	inline UIVec2 operator/(float divisor) const
	{
		ASSERT(divisor != 0);

		return UIVec2(static_cast<int>(x / divisor), static_cast<int>(y / divisor));
	}

	inline UIVec2& operator/=(float divisor)
	{
		ASSERT(divisor != 0);

		x = static_cast<int>(static_cast<float>(x) / divisor);
		y = static_cast<int>(static_cast<float>(y) / divisor);
		return *this;
	}

	int x = 0;
	int y = 0;
};
