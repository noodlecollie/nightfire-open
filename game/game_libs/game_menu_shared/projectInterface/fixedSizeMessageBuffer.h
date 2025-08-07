#pragma once

#include <cstdarg>
#include "miniutl.h"
#include "utlstring.h"

template<size_t SIZE>
class CFixedSizeMessageBuffer
{
public:
	static constexpr size_t BUFFER_SIZE = SIZE;

	CFixedSizeMessageBuffer()
	{
		m_Buffer[0] = '\0';
	}

	inline void Format(const char* format, ...)
	{
		if ( !format )
		{
			m_Buffer[0] = '\0';
			return;
		}

		va_list args;

		va_start(args, format);
		V_vsnprintf(m_Buffer, BUFFER_SIZE, format, args);
		va_end(args);
	}

	inline void VFormat(const char* format, va_list args)
	{
		if ( !format )
		{
			m_Buffer[0] = '\0';
			return;
		}

		V_vsnprintf(m_Buffer, BUFFER_SIZE, format, args);
	}

	inline void Set(const char* string)
	{
		if ( !string )
		{
			m_Buffer[0] = '\0';
			return;
		}

		V_strncpy(m_Buffer, string, BUFFER_SIZE);
	}

	inline void Set(const CUtlString& string)
	{
		V_strncpy(m_Buffer, string.String(), BUFFER_SIZE);
	}

	inline const char* Buffer() const
	{
		return m_Buffer;
	}

private:
	char m_Buffer[BUFFER_SIZE];
};
