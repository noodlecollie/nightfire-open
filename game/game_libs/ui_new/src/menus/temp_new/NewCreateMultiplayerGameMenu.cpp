#include "menus/temp_new/NewCreateMultiplayerGameMenu.h"
#include <RmlUi/Core/ElementDocument.h>
#include "game/CreateGame.h"

static constexpr const char* const EVENT_CREATE_GAME = "createGame";
static constexpr const char* const EVENT_SELECT_MAP = "selectMap";

NewCreateMultiplayerGameMenu::NewCreateMultiplayerGameMenu() :
	MenuPage("new_create_multiplayer_game_menu", "create_multiplayer_game_menu.rml"),
	m_TooltipComponent(this, "tooltip_component", "tooltip_container", "tooltipText"),
	m_PageModel(this),
	m_InputFieldEventListener(
		this,
		&NewCreateMultiplayerGameMenu::ProcessInputFieldEvent,
		"input[type='text'][validate]",
		{Rml::EventId::Blur}
	),
	m_ShowListener(this, &NewCreateMultiplayerGameMenu::HandleShowEvent, {Rml::EventId::Show})
{
}

bool NewCreateMultiplayerGameMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.BindEventCallback(EVENT_CREATE_GAME, &NewCreateMultiplayerGameMenu::HandleCreateGame, this) ||
		 !constructor.BindEventCallback(EVENT_SELECT_MAP, &NewCreateMultiplayerGameMenu::HandleSelectMap, this) )
	{
		return false;
	}

	return true;
}

void NewCreateMultiplayerGameMenu::ProcessInputFieldEvent(Rml::Event& event)
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

void NewCreateMultiplayerGameMenu::HandleShowEvent(Rml::Event&)
{
	m_PageModel.RefreshMapList();
}

void NewCreateMultiplayerGameMenu::HandleSelectMap(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args)
{
	if ( args.size() < 1 )
	{
		return;
	}

	m_PageModel.SetSelectedMap(args[0].Get<Rml::String>());
}

void NewCreateMultiplayerGameMenu::HandleCreateGame(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	Rml::String mapName = m_PageModel.SelectedMap();

	if ( mapName.empty() )
	{
		ASSERT(false);
		return;
	}

	m_PageModel.SubmitAll();

	CreateMultiplayerGame(mapName);
}
