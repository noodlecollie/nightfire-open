#pragma once

#include <cstddef>
#include <tuple>

namespace CoreUtil
{
	template<std::size_t N, typename T>
	static constexpr std::size_t ArraySize(T (&)[N])
	{
		return N;
	}
}  // namespace CoreUtil
