#include "menus/temp_new/NewPauseMenu.h"
#include "menus/temp_new/NewMainMenu.h"
#include <RmlUi/Core/ElementDocument.h>
#include "udll_int.h"

static constexpr const char* const NAME_SHOW_MODAL = "showModal";
static constexpr const char* const EVENT_DISCONNECT = "disconnect";
static constexpr const char* const EVENT_QUIT_GAME = "quitGame";

const char* const NewPauseMenu::NAME = "new_pause_menu";

NewPauseMenu::NewPauseMenu() :
	MenuPage(NAME, "pause_menu.rml"),
	m_Modal(this, "quit_modal"),
	m_ShowHideEventListener(this, &NewPauseMenu::ProcessHideEvents, {Rml::EventId::Hide})
{
	m_Modal.SetButtonClickCallback(
		[this](Rml::Event& event, size_t buttonIndex, const Rml::Variant& userData)
		{
			HandleQuitConfirm(event, buttonIndex, userData);
		}
	);
}

bool NewPauseMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(NAME_SHOW_MODAL, &m_PageModel.showModal) ||
		 !constructor.BindEventCallback(EVENT_DISCONNECT, &NewPauseMenu::HandleDisconnect, this) ||
		 !constructor.BindEventCallback(EVENT_QUIT_GAME, &NewPauseMenu::HandleQuitRequest, this) )
	{
		return false;
	}

	return true;
}

void NewPauseMenu::ProcessHideEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Hide:
		{
			m_PageModel.showModal = false;
			DirtyVariable(NAME_SHOW_MODAL);
			break;
		}

		default:
		{
			break;
		}
	}
}

void NewPauseMenu::HandleDisconnect(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	if ( m_IsQuitting )
	{
		return;
	}

	gEngfuncs.pfnClientCmd(false, "disconnect");
	RequestCutStack(0, NewMainMenu::NAME);
}

void NewPauseMenu::HandleQuitRequest(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	if ( m_IsQuitting )
	{
		return;
	}

	m_PageModel.showModal = true;
	DirtyVariable(NAME_SHOW_MODAL);
}

void NewPauseMenu::HandleQuitConfirm(Rml::Event&, size_t buttonIndex, const Rml::Variant&)
{
	const bool quit = buttonIndex == 1;

	if ( quit )
	{
		gEngfuncs.pfnClientCmd(false, "quit");
		m_IsQuitting = true;
	}
	else
	{
		m_PageModel.showModal = false;
		DirtyVariable(NAME_SHOW_MODAL);
	}
}
