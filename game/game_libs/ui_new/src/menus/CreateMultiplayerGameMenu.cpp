#include "menus/CreateMultiplayerGameMenu.h"

CreateMultiplayerGameMenu::CreateMultiplayerGameMenu() :
	MenuPage("create_multiplayer_game_menu", "resource/rml/create_multiplayer_game_menu.rml"),
	m_MenuFrameDataBinding(this),
	m_PageModel(this)
{
}

bool CreateMultiplayerGameMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !m_MenuFrameDataBinding.SetUpDataBindings(constructor) || !m_PageModel.OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	return true;
}
