#include "MenuEventListenerObject.h"
#include <RmlUi/Core/ElementDocument.h>

void MenuEventListenerObject::DocumentLoaded(Rml::ElementDocument* document)
{
	for ( Rml::EventId eventID : m_EventsToSubscribeTo )
	{
		document->AddEventListener(eventID, &m_EventListenerObject);
	}
}

void MenuEventListenerObject::DocumentUnloaded(Rml::ElementDocument* document)
{
	for ( Rml::EventId eventID : m_EventsToSubscribeTo )
	{
		document->RemoveEventListener(eventID, &m_EventListenerObject);
	}
}
