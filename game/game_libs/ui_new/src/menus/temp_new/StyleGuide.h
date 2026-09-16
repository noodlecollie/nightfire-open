#pragma once

#include "framework/MenuPage.h"
#include "framework/DataVar.h"
#include "components/TooltipComponent.h"

class StyleGuide : public MenuPage
{
public:
	StyleGuide();

	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

protected:
	void OnDocumentLoaded() override;

private:
	TooltipComponent m_ColourTooltipComponent;
	DataVar<bool> m_TextAreaDisabled;
};
