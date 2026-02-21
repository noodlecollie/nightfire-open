#pragma once

#include <RmlUi/Core/EventListener.h>

class EventListenerImpl : public Rml::EventListener
{
public:
	EventListenerImpl() = delete;
	EventListenerImpl(Rml::String strategy, Rml::String args);

	void ProcessEvent(Rml::Event& event) override;

private:
	Rml::String m_Strategy;
	Rml::String m_Args;
};
