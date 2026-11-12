#include "rmlui/CustomElementInstancers.h"
#include <RmlUi/Core/Factory.h>
#include "elements/StudioMDLSceneElement.h"

CustomElementInstancers::CustomElementInstancers()
{
	Rml::Factory::RegisterElementInstancer(StudioMDLSceneElement::ELEMENT_NAME, &m_StudioMDLSceneElementInstancer);
}

CustomElementInstancers::~CustomElementInstancers()
{
}
