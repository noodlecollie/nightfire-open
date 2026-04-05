#include "framework/ElementFinder.h"
#include <RmlUi/Core/Log.h>
#include <RmlUi/Core/Element.h>
#include "rmlui/Utils.h"

bool ElementFinder::Add(Rml::Element* const* root, Rml::String selector, Rml::Element** outElement, bool optional)
{
	if ( !root )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"ElementFinder::Add: Null root pointer provided (selector: %s)",
			selector.c_str()
		);

		return false;
	}

	if ( !outElement )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"ElementFinder::Add: Null element pointer provided (selector: %s)",
			selector.c_str()
		);

		return false;
	}

	if ( selector.empty() )
	{
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "ElementFinder::Add: Empty selector provided");
		return false;
	}

	const auto it = std::find_if(
		m_Defs.begin(),
		m_Defs.end(),
		[outElement](const ElementDef& def)
		{
			return def.element == outElement;
		}
	);

	if ( it != m_Defs.end() )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"ElementFinder::Add: Ignoring duplicate request to find element (selector: %s)",
			selector.c_str()
		);

		return false;
	}

	m_Defs.push_back({outElement, std::move(selector), root, optional});
	return true;
}

bool ElementFinder::AddMulti(Rml::Element* const* root, Rml::String selector, std::vector<Rml::Element*>* outElements)
{
	if ( !root )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"ElementFinder::AddMulti: Null root pointer provided (selector: %s)",
			selector.c_str()
		);

		return false;
	}

	if ( !outElements )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_ERROR,
			"ElementFinder::AddMulti: Null elements pointer provided (selector: %s)",
			selector.c_str()
		);

		return false;
	}

	if ( selector.empty() )
	{
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "ElementFinder::AddMulti: Empty selector provided");
		return false;
	}

	const auto it = std::find_if(
		m_MultiElementDefs.begin(),
		m_MultiElementDefs.end(),
		[outElements](const MultiElementDef& def)
		{
			return def.elementList == outElements;
		}
	);

	if ( it != m_MultiElementDefs.end() )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"ElementFinder::AddMulti: Ignoring duplicate request to find elements (selector: %s)",
			selector.c_str()
		);

		return false;
	}

	m_MultiElementDefs.push_back({outElements, std::move(selector), root});
	return true;
}

bool ElementFinder::FindAll(bool resetAllIfAnyMissed) const
{
	const bool ok = FindSingleElements() && FindMultiElements();

	if ( !ok && resetAllIfAnyMissed )
	{
		for ( const ElementDef& def : m_Defs )
		{
			*(def.element) = nullptr;
		}

		for ( const MultiElementDef& def : m_MultiElementDefs )
		{
			def.elementList->clear();
		}
	}

	return ok;
}

bool ElementFinder::FindSingleElements() const
{
	bool missedAny = false;

	for ( const ElementDef& def : m_Defs )
	{
		Rml::Element* root = *(def.root);

		if ( !root )
		{
			Rml::Log::Message(
				Rml::Log::Type::LT_WARNING,
				"ElementFinder::FindSingleElements: No root available to find element (selector: %s)",
				def.selector.c_str()
			);

			missedAny = true;
			continue;
		}

		Rml::Element* found = root->QuerySelector(def.selector);

		if ( !found && !def.optional )
		{
			Rml::Log::Message(
				Rml::Log::Type::LT_WARNING,
				"ElementFinder::FindSingleElements: Failed to find descendent matching selector %s under root element "
				"%s",
				def.selector.c_str(),
				DescribeElement(root).c_str()
			);

			missedAny = true;
			continue;
		}

		(*def.element) = found;
	}

	return !missedAny;
}

bool ElementFinder::FindMultiElements() const
{
	bool missedAny = false;

	for ( const MultiElementDef& def : m_MultiElementDefs )
	{
		Rml::Element* root = *(def.root);

		if ( !root )
		{
			Rml::Log::Message(
				Rml::Log::Type::LT_WARNING,
				"ElementFinder::FindMultiElements: No root available to find element (selector: %s)",
				def.selector.c_str()
			);

			missedAny = true;
			continue;
		}

		root->QuerySelectorAll(*def.elementList, def.selector);
	}

	return !missedAny;
}
