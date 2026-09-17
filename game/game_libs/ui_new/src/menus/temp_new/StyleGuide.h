#pragma once

#include "framework/MenuPage.h"
#include "framework/DataVar.h"
#include "framework/MenuEventListenerObject.h"
#include "components/TooltipComponent.h"

class StyleGuide : public MenuPage
{
public:
	StyleGuide();

	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

protected:
	void OnDocumentLoaded() override;

private:
	struct Tabs
	{
		Rml::Element* colours = nullptr;
		Rml::Element* scrollable = nullptr;
		Rml::Element* form = nullptr;
	};

	void HandleTabsetEvent(Rml::Event& event);
	void UpdateTabActiveStates();

	TooltipComponent m_ColourTooltipComponent;
	TooltipComponent m_FormTooltipComponent;
	MenuEventListenerObject m_ColoursPageListener;
	DataVar<bool> m_TextAreaDisabled;
	Tabs m_TabElements;
};
