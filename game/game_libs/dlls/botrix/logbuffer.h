#pragma once

#include <cstddef>

namespace Botrix
{
	class CBotrixLogBuffer
	{
	public:
		static size_t Size();
		static char* Buffer();

	private:
		static constexpr size_t BUFFER_SIZE = 8 * 1024;
		static char m_Buffer[BUFFER_SIZE];
	};
}  // namespace Botrix
