#pragma once

#include <cstddef>

template<typename T, size_t SIZE>
static inline constexpr size_t SizeOfArray(const T (&)[SIZE])
{
	return SIZE;
}
