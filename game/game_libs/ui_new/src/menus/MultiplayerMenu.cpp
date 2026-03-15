#include "menus/MultiplayerMenu.h"

MultiplayerMenu::MultiplayerMenu() :
	MenuPage("multiplayer_menu", "resource/rml/multiplayer_menu.rml")
{
}

bool MultiplayerMenu::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return MenuPage::SetUpDataModelBindings(constructor) && m_MenuFrameDataBinding.SetUpDataBindings(constructor);
}
