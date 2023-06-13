#include <cstdarg>
#include "logInterface_mainui.h"
#include "enginecallback_menu.h"
#include "utlstring.h"

#define PREFIX_WARNING "^3Warning:^7 "
#define PREFIX_ERROR "^1Error:^7 "

CFixedSizeMessageBuffer<ILogInterface::MAX_MESSAGE_LENGTH> LogInterface_MainUI::m_MsgBuffer;

void LogInterface_MainUI::Log(ILogInterface::Level level, const CUtlString& message)
{
	LogMessage(level, message.String());
}

void LogInterface_MainUI::LogF(ILogInterface::Level level, const char* format, ...)
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

void LogInterface_MainUI::LogV(Level level, const char* format, va_list args)
{
	if ( !format )
	{
		return;
	}

	m_MsgBuffer.VFormat(format, args);
	LogMessage(level, m_MsgBuffer.Buffer());
}

void LogInterface_MainUI::LogMessage(Level level, const char* message)
{
	if ( !message )
	{
		return;
	}

	switch ( level )
	{
		case ILogInterface::Level::Debug:
		{
			Con_DPrintf("%s", message);
			break;
		}

		case ILogInterface::Level::Warning:
		{
			Con_Printf(PREFIX_WARNING "%s", message);
			break;
		}

		case ILogInterface::Level::Error:
		{
			Con_Printf(PREFIX_ERROR "%s", message);
			break;
		}

		default:
		{
			Con_Printf("%s", message);
			break;
		}
	}
}
