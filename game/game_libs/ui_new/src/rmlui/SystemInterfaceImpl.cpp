#include "rmlui/SystemInterfaceImpl.h"
#include "udll_int.h"

SystemInterfaceImpl::SystemInterfaceImpl(RmlUiBackend* backend) :
	m_Backend(backend)
{
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
			gEngfuncs.Con_Printf("^1[RmlUi Error]^7 %s\n", message.c_str());
			break;
		}

		case Rml::Log::Type::LT_WARNING:
		{
			gEngfuncs.Con_Printf("^3[RmlUi Warning]^7 %s\n", message.c_str());
			break;
		}

		case Rml::Log::Type::LT_DEBUG:
		{
			gEngfuncs.Con_Printf("[RmlUi Debug] %s\n", message.c_str());
			break;
		}

		// Everything else is info
		default:
		{
			gEngfuncs.Con_Printf("[RmlUi] %s\n", message.c_str());
			break;
		}
	}

	return type != Rml::Log::Type::LT_ASSERT;
}
