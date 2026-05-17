#pragma once

#include "framework/EventListenerObject.h"
#include "framework/BaseMenuObserver.h"
#include <RmlUi/Core/Event.h>
#include <type_traits>
#include <set>

class MenuEventListenerObject : public BaseMenuObserver
{
public:
	// Listen for the specified events on the page document itself.
	template<typename T>
	MenuEventListenerObject(
		typename std::enable_if<std::is_base_of<BaseMenu, T>::value, T*>::type menu,
		EventListenerObject::ClassMemberCallback<T> memberFunc,
		std::set<Rml::EventId>&& events
	) :
		BaseMenuObserver(menu),
		m_EventListenerObject(menu, memberFunc),
		m_EventsToSubscribeTo(std::move(events))
	{
	}

	template<typename T>
	MenuEventListenerObject(
		BaseMenu* menu,
		T* recipient,
		EventListenerObject::ClassMemberCallback<T> memberFunc,
		std::set<Rml::EventId>&& events
	) :
		BaseMenuObserver(menu),
		m_EventListenerObject(recipient, memberFunc),
		m_EventsToSubscribeTo(std::move(events))
	{
	}

	// Listen for the specified events on the elements that match the selector.
	template<typename T>
	MenuEventListenerObject(
		typename std::enable_if<std::is_base_of<BaseMenu, T>::value, T*>::type menu,
		EventListenerObject::ClassMemberCallback<T> memberFunc,
		Rml::String selector,
		std::set<Rml::EventId>&& events
	) :
		BaseMenuObserver(menu),
		m_EventListenerObject(menu, memberFunc),
		m_Selector(std::move(selector)),
		m_EventsToSubscribeTo(std::move(events))
	{
		// No selector implies attaching events to the page's document!
		ASSERT(!m_Selector.empty());
	}

	template<typename T>
	MenuEventListenerObject(
		BaseMenu* menu,
		T* recipient,
		EventListenerObject::ClassMemberCallback<T> memberFunc,
		Rml::String selector,
		std::set<Rml::EventId>&& events
	) :
		BaseMenuObserver(menu),
		m_EventListenerObject(recipient, memberFunc),
		m_Selector(std::move(selector)),
		m_EventsToSubscribeTo(std::move(events))
	{
		// No selector implies attaching events to the page's document!
		ASSERT(!m_Selector.empty());
	}

	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;

private:
	void SelectElements(Rml::ElementDocument* document);

	EventListenerObject m_EventListenerObject;
	Rml::String m_Selector;
	std::set<Rml::EventId> m_EventsToSubscribeTo;
	Rml::ElementList m_SelectedElements;
};
