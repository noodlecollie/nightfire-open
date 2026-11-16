#include "rmlui/CustomElementInstancers.h"
#include <RmlUi/Core/Factory.h>
#include "elements/StudioMDLSceneElement.h"
#include "elements/StudioMDLSceneComponentElement.h"

CustomElementInstancers::CustomElementInstancers() :
	m_StudioMDLSceneElementInstancer(new Rml::ElementInstancerGeneric<StudioMDLSceneElement>()),
	m_StudioMDLSceneComponentElementInstancer(new Rml::ElementInstancerGeneric<StudioMDLSceneComponentElement>())
{
	Rml::Factory::RegisterElementInstancer(StudioMDLSceneElement::ELEMENT_NAME, m_StudioMDLSceneElementInstancer.get());

	Rml::Factory::RegisterElementInstancer(
		StudioMDLSceneComponentElement::TAG_CAMERA,
		m_StudioMDLSceneComponentElementInstancer.get()
	);

	Rml::Factory::RegisterElementInstancer(
		StudioMDLSceneComponentElement::TAG_MODEL,
		m_StudioMDLSceneComponentElementInstancer.get()
	);
}

CustomElementInstancers::~CustomElementInstancers()
{
}
