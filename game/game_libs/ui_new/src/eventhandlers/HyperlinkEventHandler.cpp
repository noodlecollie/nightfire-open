#include "eventhandlers/HyperlinkEventHandler.h"
#include <RmlUi/Core/ElementDocument.h>
#include "udll_int.h"

HyperlinkEventHandler::HyperlinkEventHandler(BaseMenu* parentMenu) :
	m_ClickListener(parentMenu, this, &HyperlinkEventHandler::HandleClickEvent, "a[href]", {Rml::EventId::Click})
{
}

void HyperlinkEventHandler::HandleClickEvent(Rml::Event& event)
{
	if ( event.GetId() != Rml::EventId::Click )
	{
		ASSERT(false);
		return;
	}

	Rml::Element* element = event.GetTargetElement();

	if ( !element )
	{
		return;
	}

	const Rml::Variant* hrefAttr = element->GetAttribute("href");

	if ( !hrefAttr )
	{
		return;
	}

	Rml::String href = hrefAttr->Get<Rml::String>();

	if ( href.empty() )
	{
		return;
	}

	gTextfuncs.pfnOpenUrlInBrowser(href.c_str());
}
