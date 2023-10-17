#pragma once

#include <cstddef>

template<typename T, size_t SIZE>
static inline constexpr size_t SizeOfArray(const T (&)[SIZE])
{
	return SIZE;
}

static inline size_t IntToSizeT(int32_t value)
{
	return value > 0 ? static_cast<size_t>(value) : 0;
}

static inline size_t DistToNextMultiple(size_t value, size_t multiple)
{
	size_t remainder = value % multiple;
	return remainder != 0 ? (multiple - remainder) : remainder;
}
