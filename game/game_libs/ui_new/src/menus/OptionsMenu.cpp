#include "menus/OptionsMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>

OptionsMenu::OptionsMenu() :
	MenuPage("options_menu", "resource/rml/options_menu.rml")
{
	// REMOVE ME
	m_KeyBindings.push_back(KeyBindingEntry {"A", "B"});
}

bool OptionsMenu::SetUpDefaultDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return MenuPage::SetUpDefaultDataModelBindings(constructor) &&
		m_MenuFrameDataBinding.SetUpDataBindings(constructor);
}

bool OptionsMenu::SetUpCustomDataModelBindings(Rml::Context& context)
{
	Rml::DataModelConstructor constructor = context.CreateDataModel("options_keybindings_model");

	if ( !constructor )
	{
		return false;
	}

	Rml::StructHandle<KeyBindingEntry> kbType = constructor.RegisterStruct<KeyBindingEntry>();

	if ( !kbType || !kbType.RegisterMember("actionName", &KeyBindingEntry::actionName) ||
		 !kbType.RegisterMember("binding", &KeyBindingEntry::binding) )
	{
		return false;
	}

	if ( !constructor.RegisterArray<std::vector<KeyBindingEntry>>() ||
		 !constructor.Bind("keybindings", &m_KeyBindings) )
	{
		return false;
	}

	return true;
}
