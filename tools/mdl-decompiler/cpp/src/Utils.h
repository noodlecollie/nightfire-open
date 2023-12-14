#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

template<typename T, size_t SIZE>
static inline constexpr size_t SizeOfArray(const T (&)[SIZE])
{
	return SIZE;
}

template<size_t SIZE>
static constexpr size_t LiteralStrLen(const char (&)[SIZE])
{
	static_assert(SIZE > 0, "String literal cannot have size of 0");
	return SIZE - 1;
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

static inline bool StartsWith(const std::string& str, const std::string& prefix)
{
	return prefix.length() <= str.length() && str.substr(0, prefix.length()) == prefix;
}

// This was written into the original code as finding the last segment of the path after any slash.
static inline std::string GoodFileName(const std::string input)
{
	const size_t index = input.find_last_of("\\/");
	return index != std::string::npos ? input.substr(index + 1) : input;
}
