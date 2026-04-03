#pragma once

#include <RmlUi/Core/Types.h>

namespace Rml
{
	class Event;
}

Rml::String DescribeElement(Rml::Element* element);
int GetEventKeyId(const Rml::Event& event);
