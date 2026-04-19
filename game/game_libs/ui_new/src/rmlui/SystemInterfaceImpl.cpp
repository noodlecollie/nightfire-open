#include "rmlui/SystemInterfaceImpl.h"
#include "EnginePublicAPI/cvardef.h"
#include "udll_int.h"

SystemInterfaceImpl::SystemInterfaceImpl(RmlUiBackend* backend) :
	m_Backend(backend)
{
}

void SystemInterfaceImpl::RegisterCvars()
{
#ifdef _DEBUG
#define LOGGING_DEFAULT_VALUE "1"
#else
#define LOGGING_DEFAULT_VALUE "0"
#endif

	m_cvarDebugLogs = gEngfuncs.pfnRegisterVariable("ui_debug_logging", LOGGING_DEFAULT_VALUE, 0);

#undef LOGGING_DEFAULT_VALUE
}

double SystemInterfaceImpl::GetElapsedTime()
{
	return gpGlobals->time;
}

void SystemInterfaceImpl::SetMouseCursor(const Rml::String& /* cursor_name */)
{
	// TODO: Engine needs to have this implemented first.
}

void SystemInterfaceImpl::SetClipboardText(const Rml::String& /* text */)
{
	// TODO: Provide a function to set clipboard text.
}

void SystemInterfaceImpl::GetClipboardText(Rml::String& text)
{
	text = gEngfuncs.pfnGetClipboardData();
}

bool SystemInterfaceImpl::LogMessage(Rml::Log::Type type, const Rml::String& message)
{
	switch ( type )
	{
		case Rml::Log::Type::LT_ASSERT:
		case Rml::Log::Type::LT_ERROR:
		{
			gEngfuncs.Con_Printf("^1[UI Error]^7 %s\n", message.c_str());
			break;
		}

		case Rml::Log::Type::LT_WARNING:
		{
			gEngfuncs.Con_Printf("^3[UI Warning]^7 %s\n", message.c_str());
			break;
		}

		case Rml::Log::Type::LT_DEBUG:
		{
			if ( m_cvarDebugLogs && m_cvarDebugLogs->value != 0.0f )
			{
				gEngfuncs.Con_Printf("[UI Debug] %s\n", message.c_str());
			}

			break;
		}

		// Everything else is info
		default:
		{
			gEngfuncs.Con_Printf("[UI] %s\n", message.c_str());
			break;
		}
	}

	return type != Rml::Log::Type::LT_ASSERT;
}
