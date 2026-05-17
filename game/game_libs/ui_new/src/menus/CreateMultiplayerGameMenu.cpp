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
		"input[type='text'][validated-value]",
		{Rml::EventId::Change, Rml::EventId::Submit, Rml::EventId::Textinput}
	)
{
}

bool CreateMultiplayerGameMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !m_MenuFrameDataBinding.SetUpDataBindings(constructor) || !m_PageModel.OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.BindEventCallback(EVENT_CREATE_GAME, &CreateMultiplayerGameMenu::HandleCreateGame, this) )
	{
		return false;
	}

	return true;
}

void CreateMultiplayerGameMenu::ProcessInputFieldEvent(Rml::Event& event)
{
	// TODO: It seems we get a textinput event before a new character is added, and
	// a change event after any change (addition or removal)
	Rml::Log::Message(Rml::Log::Type::LT_WARNING, "Received event: %s", event.GetType().c_str());
}

void CreateMultiplayerGameMenu::HandleCreateGame(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	// TODO
}
