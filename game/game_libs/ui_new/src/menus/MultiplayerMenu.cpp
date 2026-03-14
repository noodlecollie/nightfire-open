#include "menus/MultiplayerMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>

MultiplayerMenu::MultiplayerMenu() :
	BaseMenu("multiplayer_menu", "resource/rml/multiplayer_menu.rml")
{
}

bool MultiplayerMenu::SetUpDataBindingsInternal(Rml::DataModelConstructor& constructor)
{
	if ( !m_MenuFrameDataBinding.SetUpDataBindings(constructor) )
	{
		return false;
	}

	return true;
}
