#pragma once

#include <RmlUi/Core/EventListener.h>
#include <functional>
#include "UIDebug.h"

class EventListenerObject : public Rml::EventListener
{
public:
	using EventCallback = std::function<void(Rml::Event&)>;

	template<typename Class>
	using ClassMemberCallback = void (Class::*)(Rml::Event&);

	EventListenerObject() = default;

	explicit EventListenerObject(EventCallback func)
	{
		SetCallback(func);
	}

	template<typename Class>
	explicit EventListenerObject(Class* recipient, ClassMemberCallback<Class> memberFunc)
	{
		SetCallback<Class>(recipient, memberFunc);
	}

	void SetCallback(EventCallback func)
	{
		m_Callback = std::move(func);
	}

	template<typename Class>
	void SetCallback(Class* recipient, ClassMemberCallback<Class> memberFunc)
	{
		ASSERT(recipient && memberFunc);

		if ( recipient && memberFunc )
		{
			m_Callback = [recipient, memberFunc](Rml::Event& event)
			{
				(recipient->*memberFunc)(event);
			};
		}
		else
		{
			m_Callback = nullptr;
		}
	}

	void ProcessEvent(Rml::Event& event) override
	{
		if ( m_Callback )
		{
			m_Callback(event);
		}
	}

private:
	EventCallback m_Callback;
};
