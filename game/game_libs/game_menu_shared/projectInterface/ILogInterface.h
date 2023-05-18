#pragma once

#include <cstdarg>
#include <cstddef>

class CUtlString;

class ILogInterface
{
public:
	static constexpr size_t MAX_MESSAGE_LENGTH = 8192;

	enum class Level
	{
		Debug,
		Message,
		Warning,
		Error
	};

	virtual ~ILogInterface()
	{
	}

	virtual void Log(Level level, const CUtlString& message) = 0;
	virtual void LogF(Level level, const char* format, ...) = 0;
	virtual void LogV(Level level, const char* format, va_list args) = 0;
};
