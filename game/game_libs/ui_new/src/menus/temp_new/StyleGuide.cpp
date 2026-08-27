#include "menus/temp_new/StyleGuide.h"
#include "framework/ElementFinder.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/StringUtilities.h>

StyleGuide::StyleGuide() :
	MenuPage("style_guide", "style_guide.rml"),
	m_HintDisplay(this, "colour_hint_container", "colourHintText"),
	m_TextAreaDisabled {"textAreaDisabled", false}
{
}

bool StyleGuide::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(m_TextAreaDisabled.name, &m_TextAreaDisabled.value) )
	{
		return false;
	}

	return true;
}

void StyleGuide::OnDocumentLoaded()
{
	m_HintDisplay.SetDefaultHintText("None");
	m_HintDisplay.SetHintInnerRml("Colour constant: {{colourHintText}}");

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
