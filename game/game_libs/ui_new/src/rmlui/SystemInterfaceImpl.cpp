#include "rmlui/SystemInterfaceImpl.h"
#include <RmlUi/Core/StringUtilities.h>
#include "EnginePublicAPI/cvardef.h"
#include "EngineInternalAPI/cursor_type.h"
#include "udll_int.h"

static VGUI_DefaultCursor StringToCursor(const Rml::String& cursor_name)
{
	if ( cursor_name.empty() || cursor_name == "arrow" )
	{
		return VGUI_DefaultCursor::dc_arrow;
	}

	if ( cursor_name == "move" )
	{
		return VGUI_DefaultCursor::dc_sizeall;
	}

	if ( cursor_name == "pointer" )
	{
		return VGUI_DefaultCursor::dc_hand;
	}

	if ( cursor_name == "resize" )
	{
		return VGUI_DefaultCursor::dc_sizeall;
	}

	if ( cursor_name == "cross" )
	{
		return VGUI_DefaultCursor::dc_crosshair;
	}

	if ( cursor_name == "text" )
	{
		return VGUI_DefaultCursor::dc_ibeam;
	}

	if ( cursor_name == "unavailable" )
	{
		return VGUI_DefaultCursor::dc_no;
	}

	if ( Rml::StringUtilities::StartsWith(cursor_name, "rmlui-scroll") )
	{
		return VGUI_DefaultCursor::dc_arrow;
	}

	return VGUI_DefaultCursor::dc_arrow;
}

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

void SystemInterfaceImpl::SetMouseCursor(const Rml::String& cursor_name)
{
	gEngfuncs.pfnSetCursor(StringToCursor(cursor_name));
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
