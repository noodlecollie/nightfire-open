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
