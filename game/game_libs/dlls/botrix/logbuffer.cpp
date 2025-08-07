#include "botrix/logbuffer.h"

char CBotrixLogBuffer::m_Buffer[CBotrixLogBuffer::BUFFER_SIZE];

size_t CBotrixLogBuffer::Size()
{
	return BUFFER_SIZE;
}

char* CBotrixLogBuffer::Buffer()
{
	return m_Buffer;
}
