#pragma once

#include "framework/EventListenerObject.h"
#include "framework/BaseMenuObserver.h"
#include <RmlUi/Core/Event.h>
#include <type_traits>
#include <set>

class MenuEventListenerObject : public BaseMenuObserver
{
public:
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

	void DocumentLoaded(Rml::ElementDocument* document) override;
	void DocumentUnloaded(Rml::ElementDocument* document) override;

private:
	EventListenerObject m_EventListenerObject;
	std::set<Rml::EventId> m_EventsToSubscribeTo;
};
