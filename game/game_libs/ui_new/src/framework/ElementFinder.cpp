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

bool ElementFinder::FindAll(bool resetAllIfAnyMissed) const
{
	bool missedAny = false;

	for ( const ElementDef& def : m_Defs )
	{
		Rml::Element* root = *(def.root);

		if ( !root )
		{
			Rml::Log::Message(
				Rml::Log::Type::LT_WARNING,
				"ElementFinder::FindAll: No root available to find element (selector: %s)",
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
				"ElementFinder::FindAll: Failed to find descendent matching selector %s under root element %s",
				def.selector.c_str(),
				DescribeElement(root).c_str()
			);

			missedAny = true;
			continue;
		}

		(*def.element) = found;
	}

	if ( missedAny && resetAllIfAnyMissed )
	{
		for ( const ElementDef& def : m_Defs )
		{
			*(def.element) = nullptr;
		}
	}

	return !missedAny;
}
