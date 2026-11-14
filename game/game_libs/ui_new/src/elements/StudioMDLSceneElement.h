#pragma once

#include <RmlUi/Core/Element.h>

class StudioMDLSceneElement : public Rml::Element
{
public:
	static constexpr const char* const ELEMENT_NAME = "studiomdlscene";

	StudioMDLSceneElement(const Rml::String& tag);
};
