#pragma once

#include <cstddef>

class CBotrixLogBuffer
{
public:
	static size_t Size();
	static char* Buffer();

private:
	static constexpr size_t BUFFER_SIZE = 8 * 1024;
	static char m_Buffer[BUFFER_SIZE];
};
