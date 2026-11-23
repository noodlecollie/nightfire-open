#pragma once

#include <RmlUi/Core/ElementInstancer.h>
#include <memory>

class StudioMDLSceneElement;

class CustomElementInstancers
{
public:
	CustomElementInstancers();
	~CustomElementInstancers();

	CustomElementInstancers(const CustomElementInstancers& other) = delete;
	CustomElementInstancers& operator=(const CustomElementInstancers& other) = delete;

private:
	std::unique_ptr<Rml::ElementInstancerGeneric<StudioMDLSceneElement>> m_StudioMDLSceneElementInstancer;
};
