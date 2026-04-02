#include "rmlui/Utils.h"
#include <RmlUi/Core/Element.h>

Rml::String DescribeElement(Rml::Element* element)
{
	if ( !element )
	{
		return "<null>";
	}

	Rml::String out = element->GetTagName();
	Rml::String id = element->GetId();

	return (!id.empty()) ? out + "#" + id : out;
}
