#include <cstdarg>
#include "logInterface_client.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"
#include "cl_dll.h"

#define PREFIX_WARNING "^3Warning:^7 "
#define PREFIX_ERROR "^1Error:^7 "

CFixedSizeMessageBuffer<ILogInterface::MAX_MESSAGE_LENGTH> LogInterface_Client::m_MsgBuffer;

void LogInterface_Client::Log(ILogInterface::Level level, const CUtlString& message)
{
	LogMessage(level, message.String());
}

void LogInterface_Client::LogF(ILogInterface::Level level, const char* format, ...)
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

void LogInterface_Client::LogV(Level level, const char* format, va_list args)
{
	if ( !format )
	{
		return;
	}

	m_MsgBuffer.VFormat(format, args);
	LogMessage(level, m_MsgBuffer.Buffer());
}

void LogInterface_Client::LogMessage(Level level, const char* message)
{
	if ( !message )
	{
		return;
	}

	switch ( level )
	{
		case ILogInterface::Level::Debug:
		{
			gEngfuncs.Con_DPrintf(message);
			break;
		}

		case ILogInterface::Level::Warning:
		{
			gEngfuncs.Con_Printf(PREFIX_WARNING "%s", message);
			break;
		}

		case ILogInterface::Level::Error:
		{
			gEngfuncs.Con_Printf(PREFIX_ERROR "%s", message);
			break;
		}

		default:
		{
			gEngfuncs.Con_Printf(message);
			break;
		}
	}
}
