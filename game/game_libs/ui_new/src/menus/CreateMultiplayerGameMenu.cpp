#include "menus/CreateMultiplayerGameMenu.h"
#include <RmlUi/Core/ElementDocument.h>

static constexpr const char* const EVENT_CREATE_GAME = "createGame";

CreateMultiplayerGameMenu::CreateMultiplayerGameMenu() :
	MenuPage("create_multiplayer_game_menu", "resource/rml/create_multiplayer_game_menu.rml"),
	m_MenuFrameDataBinding(this),
	m_PageModel(this),
	m_InputFieldEventListener(
		this,
		&CreateMultiplayerGameMenu::ProcessInputFieldEvent,
		"input[type='text'][validate]",
		{Rml::EventId::Blur}
	)
{
}

bool CreateMultiplayerGameMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !constructor.BindEventCallback(EVENT_CREATE_GAME, &CreateMultiplayerGameMenu::HandleCreateGame, this) )
	{
		return false;
	}

	return true;
}

void CreateMultiplayerGameMenu::ProcessInputFieldEvent(Rml::Event& /* event */)
{
	// TODO
}

void CreateMultiplayerGameMenu::HandleCreateGame(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	// TODO
}
