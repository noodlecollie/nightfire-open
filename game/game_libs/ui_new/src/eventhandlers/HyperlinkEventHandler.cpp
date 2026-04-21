#include "eventhandlers/HyperlinkEventHandler.h"
#include <RmlUi/Core/ElementDocument.h>
#include "udll_int.h"

static constexpr const char* const SELECTOR = "a[href]";

HyperlinkEventHandler::HyperlinkEventHandler(BaseMenu* parentMenu) :
	DocumentObserver(parentMenu),
	m_ClickListener(this, &HyperlinkEventHandler::HandleClickEvent)
{
}

void HyperlinkEventHandler::DocumentLoaded(Rml::ElementDocument* document)
{
	Rml::ElementList list;
	document->QuerySelectorAll(list, SELECTOR);

	for ( Rml::Element* element : list )
	{
		element->AddEventListener(Rml::EventId::Click, &m_ClickListener);
	}
}

void HyperlinkEventHandler::DocumentUnloaded(Rml::ElementDocument* document)
{
	Rml::ElementList list;
	document->QuerySelectorAll(list, SELECTOR);

	for ( Rml::Element* element : list )
	{
		element->RemoveEventListener(Rml::EventId::Click, &m_ClickListener);
	}
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
