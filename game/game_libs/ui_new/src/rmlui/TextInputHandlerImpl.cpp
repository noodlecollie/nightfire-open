#include "rmlui/TextInputHandlerImpl.h"
#include "udll_int.h"

void TextInputHandlerImpl::OnActivate(Rml::TextInputContext*)
{
	m_IsActive = true;
	gTextfuncs.pfnEnableTextInput(true);
}

void TextInputHandlerImpl::OnDeactivate(Rml::TextInputContext*)
{
	gTextfuncs.pfnEnableTextInput(false);
	m_IsActive = false;
}

bool TextInputHandlerImpl::IsActive() const
{
	return m_IsActive;
}
