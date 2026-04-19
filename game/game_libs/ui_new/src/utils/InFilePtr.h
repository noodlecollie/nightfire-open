#pragma once

#include <memory>
#include <limits>
#include <string>
#include "udll_int.h"

class InFileBytesPtr
{
public:
	explicit InFileBytesPtr(const char* path) :
		m_FilePath(path ? path : "")
	{
		if ( !m_FilePath.empty() )
		{
			int length = 0;
			byte* data = gEngfuncs.COM_LoadFile(m_FilePath.c_str(), &length);

			if ( data )
			{
				m_Ptr.reset(data);
				m_Length = static_cast<size_t>(std::max<int>(length, 0));
			}
		}
	}

	const std::string& Path() const
	{
		return m_FilePath;
	}

	const byte* Data() const
	{
		return m_Ptr.get();
	}

	size_t Length() const
	{
		return m_Length;
	}

	bool IsValid() const
	{
		return m_Ptr.get() != nullptr;
	}

	operator bool() const
	{
		return IsValid();
	}

private:
	struct Deleter
	{
		void operator()(void* ptr)
		{
			if ( ptr )
			{
				gEngfuncs.COM_FreeFile(ptr);
			}
		}
	};

	std::string m_FilePath;
	std::unique_ptr<byte, Deleter> m_Ptr;
	size_t m_Length = 0;
};

class InFileCharsPtr : public InFileBytesPtr
{
public:
	explicit InFileCharsPtr(const char* path, int parseFlags = 0) :
		InFileBytesPtr(path),
		m_Cursor(reinterpret_cast<const char*>(Data())),
		m_ParseFlags(parseFlags)
	{
	}

	bool ParseToken(char* buffer, size_t bufferSize, int* tokenLength = nullptr, const int* overrideFlags = nullptr)
	{
		if ( !m_Cursor )
		{
			return false;
		}

		bufferSize = std::min<size_t>(bufferSize, static_cast<size_t>(std::numeric_limits<int>::max()));

		// TODO: Fix up the param in this function definition so that it's const.
		m_Cursor = gTextfuncs.pfnParseFile(
			const_cast<char*>(m_Cursor),
			buffer,
			static_cast<int>(bufferSize),
			overrideFlags ? *overrideFlags : m_ParseFlags,
			tokenLength
		);

		return m_Cursor != nullptr;
	}

	template<size_t BufferSize>
	bool ParseToken(char (&buffer)[BufferSize], int* tokenLength = nullptr, const int* overrideFlags = nullptr)
	{
		return ParseToken(buffer, BufferSize, tokenLength, overrideFlags);
	}

private:
	const char* m_Cursor = nullptr;
	int m_ParseFlags = 0;
};
