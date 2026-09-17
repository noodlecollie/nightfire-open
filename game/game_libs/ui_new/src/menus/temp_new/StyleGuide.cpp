#include "menus/temp_new/StyleGuide.h"
#include "framework/ElementFinder.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/StringUtilities.h>

StyleGuide::StyleGuide() :
	MenuPage("style_guide", "style_guide.rml"),
	m_ColourTooltipComponent(this, "colour_hint_component", "colour_hint_container", "colourHintText"),
	m_FormTooltipComponent(this, "tooltip_component", "tooltip_content", "tooltipText"),
	m_ColoursPageListener(this, &StyleGuide::HandleTabsetEvent, "#section_tabset", {Rml::EventId::Tabchange}),
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
	m_ColourTooltipComponent.SetDefaultTooltipText("None");
	m_ColourTooltipComponent.SetHintInnerRml("Colour constant: {{colourHintText}}");

	{
		ElementFinder finder;

		Rml::Element* tabset = Document()->GetElementById("section_tabset");
		finder.Add(&tabset, "#colours_tab", &m_TabElements.colours);
		finder.Add(&tabset, "#scrollable_tab", &m_TabElements.scrollable);
		finder.Add(&tabset, "#form_tab", &m_TabElements.form);

		finder.FindAll();
		UpdateTabActiveStates();
	}

	{
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
}

void StyleGuide::HandleTabsetEvent(Rml::Event&)
{
	UpdateTabActiveStates();
}

void StyleGuide::UpdateTabActiveStates()
{
	const bool coloursTabActive = m_TabElements.colours && m_TabElements.colours->ArePseudoClassesSet({"selected"});
	const bool formTabActive = m_TabElements.form && m_TabElements.form->ArePseudoClassesSet({"selected"});

	m_ColourTooltipComponent.SetEnabled(coloursTabActive);
	m_FormTooltipComponent.SetEnabled(formTabActive);

	if ( coloursTabActive )
	{
		Rml::Log::Message(Rml::Log::Type::LT_DEBUG, "Colours tab active");
	}

	if ( formTabActive )
	{
		Rml::Log::Message(Rml::Log::Type::LT_DEBUG, "Form tab active");
	}
}
