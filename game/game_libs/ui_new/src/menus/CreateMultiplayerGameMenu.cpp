#include "menus/CreateMultiplayerGameMenu.h"
#include <RmlUi/Core/ElementDocument.h>
#include "game/CreateGame.h"
#include "menus/ServerConnectionScreen.h"

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
	),
	m_ShowListener(this, &CreateMultiplayerGameMenu::HandleShowEvent, {Rml::EventId::Show})
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

void CreateMultiplayerGameMenu::ProcessInputFieldEvent(Rml::Event& event)
{
	Rml::Element* element = event.GetTargetElement();

	if ( !element )
	{
		ASSERT(false);
		return;
	}

	Rml::Variant* dataValueAttr = element->GetAttribute("data-value");

	if ( !dataValueAttr )
	{
		ASSERT(false);
		return;
	}

	m_PageModel.ValidateAndSubmit(dataValueAttr->Get<Rml::String>());
}

void CreateMultiplayerGameMenu::HandleShowEvent(Rml::Event&)
{
	m_PageModel.RefreshMapList();
}

void CreateMultiplayerGameMenu::HandleCreateGame(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	m_PageModel.SubmitAll();

	// TODO: Hook up map name
	CreateMultiplayerGame("dm_island");
	RequestCutStack(0, ServerConnectionScreen::NAME);
}
