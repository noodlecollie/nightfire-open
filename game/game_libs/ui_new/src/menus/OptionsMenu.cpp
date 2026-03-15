#include "menus/OptionsMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>

OptionsMenu::OptionsMenu() :
	MenuPage("options_menu", "resource/rml/options_menu.rml")
{
}

bool OptionsMenu::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::SetUpDataModelBindings(constructor) || !m_MenuFrameDataBinding.SetUpDataBindings(constructor) ||
		 !m_KeyBindings.SetUpDataBindings(constructor) )
	{
		return false;
	}

	return true;
}
