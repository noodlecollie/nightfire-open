#include "rmlui/CustomElementInstancers.h"
#include <RmlUi/Core/Factory.h>
#include <RmlUi/Core/XMLParser.h>
#include "elements/StudioMDLSceneElement.h"
#include "elements/StudioMDLSceneElementXMLHandler.h"

CustomElementInstancers::CustomElementInstancers() :
	m_StudioMDLSceneElementInstancer(new Rml::ElementInstancerGeneric<StudioMDLSceneElement>())
{
	Rml::Factory::RegisterElementInstancer(StudioMDLSceneElement::TAG, m_StudioMDLSceneElementInstancer.get());

	Rml::XMLParser::RegisterNodeHandler(
		StudioMDLSceneElement::TAG,
		std::make_shared<StudioMDLSceneElementXMLHandler>()
	);
}

CustomElementInstancers::~CustomElementInstancers()
{
}
