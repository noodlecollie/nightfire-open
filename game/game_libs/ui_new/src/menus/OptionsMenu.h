#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/MenuFrameDataBinding.h"
#include "models/KeyBindingModel.h"

class OptionsMenu : public MenuPage
{
public:
	OptionsMenu();

protected:
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	struct KeyBindingEntry
	{
		Rml::String actionName;
		Rml::String binding1;
		Rml::String binding2;
	};

	MenuFrameDataBinding m_MenuFrameDataBinding;
	KeyBindingModel m_KeyBindings;
};
