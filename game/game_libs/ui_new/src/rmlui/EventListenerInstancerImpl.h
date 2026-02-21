#pragma once

#include <RmlUi/Core/EventListenerInstancer.h>

class EventListenerInstancerImpl : public Rml::EventListenerInstancer
{
public:
	Rml::EventListener* InstanceEventListener(const Rml::String& value, Rml::Element* element) override;
};
