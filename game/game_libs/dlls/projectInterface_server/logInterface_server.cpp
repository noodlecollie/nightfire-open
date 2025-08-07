#include <cstdarg>
#include <memory>
#include "logInterface_server.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"

CFixedSizeMessageBuffer<ILogInterface::MAX_MESSAGE_LENGTH> LogInterface_Server::m_MsgBuffer;

void LogInterface_Server::Log(ILogInterface::Level level, const CUtlString& message)
{
	LogMessage(level, message.String());
}

void LogInterface_Server::LogF(ILogInterface::Level level, const char* format, ...)
{
	if ( !format )
	{
		return;
	}

	va_list args;

	va_start(args, format);
	m_MsgBuffer.VFormat(format, args);
	va_end(args);

	LogMessage(level, m_MsgBuffer.Buffer());
}

void LogInterface_Server::LogV(Level level, const char* format, va_list args)
{
	if ( !format )
	{
		return;
	}

	m_MsgBuffer.VFormat(format, args);
	LogMessage(level, m_MsgBuffer.Buffer());
}

void LogInterface_Server::LogMessage(Level level, const char* message)
{
	if ( !message )
	{
		return;
	}

	switch ( level )
	{
		case ILogInterface::Level::Debug:
		{
			ALERT(at_aiconsole, message);
			break;
		}

		case ILogInterface::Level::Warning:
		{
			ALERT(at_warning, message);
			break;
		}

		case ILogInterface::Level::Error:
		{
			ALERT(at_error, message);
			break;
		}

		default:
		{
			ALERT(at_console, message);
			break;
		}
	}
}
