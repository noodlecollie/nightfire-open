#include "rmlui/TextInputHandlerImpl.h"
#include "udll_int.h"

void TextInputHandlerImpl::OnActivate(Rml::TextInputContext*)
{
	gTextfuncs.pfnEnableTextInput(true);
}

void TextInputHandlerImpl::OnDeactivate(Rml::TextInputContext*)
{
	gTextfuncs.pfnEnableTextInput(false);
}
