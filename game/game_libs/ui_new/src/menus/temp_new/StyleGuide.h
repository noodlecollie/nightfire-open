#pragma once

#include "framework/MenuPage.h"
#include "framework/DataVar.h"
#include "templatebindings/HintDisplay.h"

class StyleGuide : public MenuPage
{
public:
	StyleGuide();

	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

protected:
	void OnDocumentLoaded() override;

private:
	HintDisplay m_HintDisplay;
	DataVar<bool> m_TextAreaDisabled;
};
