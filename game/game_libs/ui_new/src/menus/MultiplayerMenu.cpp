#include "menus/MultiplayerMenu.h"

MultiplayerMenu::MultiplayerMenu() :
	MenuPage("multiplayer_menu", "resource/rml/multiplayer_menu.rml"),
	m_MenuFrameDataBinding(this)
{
}

bool MultiplayerMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return MenuPage::OnSetUpDataModelBindings(constructor) && m_MenuFrameDataBinding.SetUpDataBindings(constructor);
}
