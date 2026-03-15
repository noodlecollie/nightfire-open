#include "menus/OptionsMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>

OptionsMenu::OptionsMenu() :
	MenuPage("options_menu", "resource/rml/options_menu.rml")
{
}

bool OptionsMenu::SetUpDefaultDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::SetUpDefaultDataModelBindings(constructor) ||
		 !m_MenuFrameDataBinding.SetUpDataBindings(constructor) )
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
