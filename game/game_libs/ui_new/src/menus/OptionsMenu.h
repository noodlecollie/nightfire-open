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
	static constexpr const char* const TAB_GAMEPLAY = "gameplay";
	static constexpr const char* const TAB_KEYS = "keys";
	static constexpr const char* const TAB_MOUSE = "mouse";
	static constexpr const char* const TAB_AV = "av";

	struct KeyBindingEntry
	{
		Rml::String actionName;
		Rml::String binding1;
		Rml::String binding2;
	};

	struct PageModel
	{
		Rml::String activeTab = TAB_GAMEPLAY;
	};

	MenuFrameDataBinding m_MenuFrameDataBinding;
	KeyBindingModel m_KeyBindings;
	PageModel m_PageModel;
};
