#include "menus/StyleGuide.h"
#include "framework/ElementFinder.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/StringUtilities.h>

StyleGuide::StyleGuide() :
	MenuPage("style_guide", "resource/rml/style_guide.rml"),
	m_MenuFrameDataBinding(this)
{
}

void StyleGuide::OnDocumentLoaded()
{
	m_MenuFrameDataBinding.SetDefaultTooltipText("None");
	m_MenuFrameDataBinding.SetTooltipInnerRml("Colour constant: {{footerTooltip}}");
	m_MenuFrameDataBinding.SetTitle("Style Guide");

	ElementFinder finder;

	Rml::Element* table = Document()->GetElementById("colours_table");
	Rml::ElementList swatches;
	finder.AddMulti(&table, ".palette-swatch", &swatches);
	finder.FindAll();

	for ( Rml::Element* swatch : swatches )
	{
		const Rml::Property* bgProp = swatch->GetProperty(Rml::PropertyId::BackgroundColor);

		if ( !bgProp )
		{
			continue;
		}

		swatch->SetInnerRML(Rml::StringUtilities::EncodeRml(bgProp->ToString()));
	}
}
