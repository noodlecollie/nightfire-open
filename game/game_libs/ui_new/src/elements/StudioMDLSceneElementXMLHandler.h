#pragma once

#include <RmlUi/Core/XMLNodeHandler.h>

class StudioMDLSceneElement;

class StudioMDLSceneElementXMLHandler : public Rml::XMLNodeHandler
{
public:
	Rml::Element*
	ElementStart(Rml::XMLParser* parser, const Rml::String& name, const Rml::XMLAttributes& attributes) override;
	bool ElementEnd(Rml::XMLParser* parser, const Rml::String& name) override;
	bool ElementData(Rml::XMLParser* parser, const Rml::String& data, Rml::XMLDataType type) override;

private:
	void HandleAddModel(const Rml::XMLAttributes& attributes);
	void HandleAddCamera(const Rml::XMLAttributes& attributes);

	Rml::String m_CurrentElementTag;
	StudioMDLSceneElement* m_CurrentElement = nullptr;
};
