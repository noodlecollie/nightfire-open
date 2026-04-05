#pragma once

#include <vector>
#include <RmlUi/Core/Types.h>

namespace Rml
{
	class Element;
}

class ElementFinder
{
public:
	// Root is a ptr-to-ptr so that it can be an outElement used in a previous call.
	bool Add(Rml::Element* const* root, Rml::String selector, Rml::Element** outElement, bool optional = false);
	bool AddMulti(Rml::Element* const* root, Rml::String selector, Rml::ElementList* outElements);

	bool FindAll(bool resetAllIfAnyMissed = true) const;

private:
	struct ElementDef
	{
		Rml::Element** element = nullptr;
		Rml::String selector;
		Rml::Element* const* root = nullptr;
		bool optional = false;
	};

	struct MultiElementDef
	{
		Rml::ElementList* elementList = nullptr;
		Rml::String selector;
		Rml::Element* const* root = nullptr;
	};

	bool FindSingleElements() const;
	bool FindMultiElements() const;

	std::vector<ElementDef> m_Defs;
	std::vector<MultiElementDef> m_MultiElementDefs;
};
