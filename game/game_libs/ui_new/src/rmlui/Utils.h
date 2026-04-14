#pragma once

#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/Input.h>

namespace Rml
{
	class Event;
}

Rml::String DescribeElement(const Rml::Element* element);
int GetEventKeyId(const Rml::Event& event);
Rml::Input::KeyIdentifier EngineKeyToRmlKey(int key);
int RmlKeyToEngineKey(Rml::Input::KeyIdentifier key);
unsigned char EngineKeyToRmlKeyModifier(int key);
