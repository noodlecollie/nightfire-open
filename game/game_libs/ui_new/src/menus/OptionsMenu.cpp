#include "menus/OptionsMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>

OptionsMenu::OptionsMenu() :
	BaseMenu("options_menu", "resource/rml/options_menu.rml")
{
}

bool OptionsMenu::SetUpDataBindingsInternal(Rml::DataModelConstructor& constructor)
{
	if ( !m_MenuFrameDataBinding.SetUpDataBindings(constructor) )
	{
		return false;
	}

	return true;
}
