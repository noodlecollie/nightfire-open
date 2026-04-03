#pragma once

#include <RmlUi/Core/EventListener.h>
#include <functional>
#include "UIDebug.h"

class EventListenerObject : public Rml::EventListener
{
public:
	using EventListenerFunc = std::function<void(Rml::Event&)>;

	template<typename Class>
	using ClassListenerFunc = void (Class::*)(Rml::Event&);

	explicit EventListenerObject(EventListenerFunc func) :
		m_Func(std::move(func))
	{
	}

	template<typename Class>
	explicit EventListenerObject(Class* recipient, ClassListenerFunc<Class> memberFunc)
	{
		ASSERT(recipient && memberFunc);

		if ( recipient && memberFunc )
		{
			m_Func = [recipient, memberFunc](Rml::Event& event)
			{
				(recipient->*memberFunc)(event);
			};
		}
	}

	void ProcessEvent(Rml::Event& event) override
	{
		if ( m_Func )
		{
			m_Func(event);
		}
	}

private:
	EventListenerFunc m_Func;
};
