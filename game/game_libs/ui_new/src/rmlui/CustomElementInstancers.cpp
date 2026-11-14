#include "rmlui/CustomElementInstancers.h"
#include <RmlUi/Core/Factory.h>
#include "elements/StudioMDLSceneElement.h"

CustomElementInstancers::CustomElementInstancers() :
	m_StudioMDLSceneElementInstancer(new Rml::ElementInstancerGeneric<StudioMDLSceneElement>())
{
	Rml::Factory::RegisterElementInstancer(StudioMDLSceneElement::ELEMENT_NAME, m_StudioMDLSceneElementInstancer.get());
}

CustomElementInstancers::~CustomElementInstancers()
{
}
