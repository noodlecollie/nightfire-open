#include "menus/CreateMultiplayerGameMenu.h"
#include <RmlUi/Core/ElementDocument.h>

static constexpr const char* const EVENT_CREATE_GAME = "createGame";

CreateMultiplayerGameMenu::CreateMultiplayerGameMenu() :
	MenuPage("create_multiplayer_game_menu", "resource/rml/create_multiplayer_game_menu.rml"),
	m_MenuFrameDataBinding(this),
	m_PageModel(this),
	m_InputFieldEventListener(this, &CreateMultiplayerGameMenu::ProcessInputFieldEvent)
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

void CreateMultiplayerGameMenu::OnEndDocumentLoaded()
{
	MenuPage::OnEndDocumentLoaded();

	Rml::ElementList elements;
	Document()->QuerySelectorAll(elements, "input[type='text'][validated-value]");

	for ( Rml::Element* element : elements )
	{
		element->AddEventListener(Rml::EventId::Change, &m_InputFieldEventListener);
		element->AddEventListener(Rml::EventId::Submit, &m_InputFieldEventListener);
		element->AddEventListener(Rml::EventId::Textinput, &m_InputFieldEventListener);
	}
}

void CreateMultiplayerGameMenu::OnBeginDocumentUnloaded()
{
	Rml::ElementList elements;
	Document()->QuerySelectorAll(elements, "input[type='text']");

	for ( Rml::Element* element : elements )
	{
		element->RemoveEventListener(Rml::EventId::Change, &m_InputFieldEventListener);
		element->RemoveEventListener(Rml::EventId::Submit, &m_InputFieldEventListener);
		element->RemoveEventListener(Rml::EventId::Textinput, &m_InputFieldEventListener);
	}

	MenuPage::OnBeginDocumentUnloaded();
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
