#include "rmlui/CustomElementInstancers.h"
#include <RmlUi/Core/Factory.h>
#include "elements/StudioMDLSceneElement.h"
#include "elements/StudioMDLSceneItemElement.h"

CustomElementInstancers::CustomElementInstancers() :
	m_StudioMDLSceneElementInstancer(new Rml::ElementInstancerGeneric<StudioMDLSceneElement>()),
	m_StudioMDLSceneItemElementInstancer(new Rml::ElementInstancerGeneric<StudioMDLSceneItemElement>())
{
	Rml::Factory::RegisterElementInstancer(StudioMDLSceneElement::ELEMENT_NAME, m_StudioMDLSceneElementInstancer.get());

	Rml::Factory::RegisterElementInstancer(
		StudioMDLSceneItemElement::TAG_CAMERA,
		m_StudioMDLSceneItemElementInstancer.get()
	);

	Rml::Factory::RegisterElementInstancer(
		StudioMDLSceneItemElement::TAG_MODEL,
		m_StudioMDLSceneItemElementInstancer.get()
	);
}

CustomElementInstancers::~CustomElementInstancers()
{
}
