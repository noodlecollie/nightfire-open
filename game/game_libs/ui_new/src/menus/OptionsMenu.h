#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/MenuFrameDataBinding.h"

class OptionsMenu : public MenuPage
{
public:
	OptionsMenu();

protected:
	bool SetUpDefaultDataModelBindings(Rml::DataModelConstructor& constructor) override;
	bool SetUpCustomDataModelBindings(Rml::Context& context) override;

private:
	struct KeyBindingEntry
	{
		Rml::String actionName;
		Rml::String binding;
	};

	MenuFrameDataBinding m_MenuFrameDataBinding;
	std::vector<KeyBindingEntry> m_KeyBindings;
};
