#pragma once

#include <utility>
#include <cstdint>
#include <cstddef>

namespace Conversion
{
	template<typename T>
	using Pair = std::pair<const char*, T>;

	template<typename T, size_t SIZE>
	static inline const Pair<T>* FindByString(const Pair<T> (&array)[SIZE], const char* str)
	{
		for ( size_t index = 0; index < SIZE; ++index )
		{
			if ( strcmp(str, array[index].first) == 0 )
			{
				return &array[index];
			}
		}

		return nullptr;
	}

	template<typename T, size_t SIZE>
	static inline const Pair<T>* FindByValue(const Pair<T> (&array)[SIZE], int32_t value)
	{
		for ( size_t index = 0; index < SIZE; ++index )
		{
			if ( static_cast<int32_t>(array[index].second) == value )
			{
				return &array[index];
			}
		}

		return nullptr;
	}
}  // namespace Conversion
