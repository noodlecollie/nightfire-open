#include "menus/CreateMultiplayerGameMenu.h"

static constexpr const char* const NAME_GAME_TYPE = "gameType";
static constexpr const char* const NAME_SERVER_NAME = "serverName";

CreateMultiplayerGameMenu::CreateMultiplayerGameMenu() :
	MenuPage("create_multiplayer_game_menu", "resource/rml/create_multiplayer_game_menu.rml"),
	m_MenuFrameDataBinding(this)
{
}

bool CreateMultiplayerGameMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !m_MenuFrameDataBinding.SetUpDataBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(NAME_GAME_TYPE, &m_PageModel.gameType) ||
		 !constructor.Bind(NAME_SERVER_NAME, &m_PageModel.serverName) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}
