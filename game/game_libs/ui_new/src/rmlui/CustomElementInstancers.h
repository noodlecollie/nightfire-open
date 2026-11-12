#pragma once

#include <RmlUi/Core/ElementInstancer.h>

class StudioMDLSceneElement;

class CustomElementInstancers
{
public:
	CustomElementInstancers();
	~CustomElementInstancers();

	CustomElementInstancers(const CustomElementInstancers& other) = delete;
	CustomElementInstancers& operator=(const CustomElementInstancers& other) = delete;

private:
	Rml::ElementInstancerGeneric<StudioMDLSceneElement> m_StudioMDLSceneElementInstancer;
};
