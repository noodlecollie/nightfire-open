#include "MenuEventListenerObject.h"
#include <RmlUi/Core/ElementDocument.h>

void MenuEventListenerObject::DocumentLoaded(Rml::ElementDocument* document)
{
	SelectElements(document);

	for ( Rml::Element* element : m_SelectedElements )
	{
		for ( Rml::EventId eventID : m_EventsToSubscribeTo )
		{
			element->AddEventListener(eventID, &m_EventListenerObject);
		}
	}
}

void MenuEventListenerObject::DocumentUnloaded(Rml::ElementDocument*)
{
	for ( Rml::Element* element : m_SelectedElements )
	{
		for ( Rml::EventId eventID : m_EventsToSubscribeTo )
		{
			element->RemoveEventListener(eventID, &m_EventListenerObject);
		}
	}
}

void MenuEventListenerObject::SelectElements(Rml::ElementDocument* document)
{
	m_SelectedElements.clear();

	if ( m_Selector.empty() )
	{
		m_SelectedElements.push_back(document);
		return;
	}

	document->QuerySelectorAll(m_SelectedElements, m_Selector);
}
