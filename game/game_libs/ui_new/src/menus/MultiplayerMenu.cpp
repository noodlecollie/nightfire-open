#include "menus/MultiplayerMenu.h"

MultiplayerMenu::MultiplayerMenu() :
	MenuPage("multiplayer_menu", "resource/rml/multiplayer_menu.rml")
{
}

bool MultiplayerMenu::SetUpDefaultDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return MenuPage::SetUpDefaultDataModelBindings(constructor) &&
		m_MenuFrameDataBinding.SetUpDataBindings(constructor);
}
