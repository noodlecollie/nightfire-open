#include "menus/PauseMenu.h"
#include "menus/MainMenu.h"
#include <RmlUi/Core/ElementDocument.h>
#include "udll_int.h"

static constexpr const char* const NAME_SHOW_MODAL = "showModal";
static constexpr const char* const EVENT_DISCONNECT = "disconnect";
static constexpr const char* const EVENT_QUIT_GAME = "quitGame";

const char* const PauseMenu::NAME = "pause_menu";

PauseMenu::PauseMenu() :
	MenuPage(NAME, "resource/rml/pause_menu.rml"),
	m_MenuFrameDataBinding(this),
	m_Modal(this, "quit_modal"),
	m_ShowHideEventListener(this, &PauseMenu::ProcessHideEvents, {Rml::EventId::Hide})
{
	m_Modal.SetButtonClickCallback(
		[this](Rml::Event& event, size_t buttonIndex, const Rml::Variant& userData)
		{
			HandleQuitConfirm(event, buttonIndex, userData);
		}
	);
}

bool PauseMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(NAME_SHOW_MODAL, &m_PageModel.showModal) ||
		 !constructor.BindEventCallback(EVENT_DISCONNECT, &PauseMenu::HandleDisconnect, this) ||
		 !constructor.BindEventCallback(EVENT_QUIT_GAME, &PauseMenu::HandleQuitRequest, this) )
	{
		return false;
	}

	return true;
}

void PauseMenu::ProcessHideEvents(Rml::Event& event)
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

void PauseMenu::HandleDisconnect(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	if ( m_IsQuitting )
	{
		return;
	}

	gEngfuncs.pfnClientCmd(false, "disconnect");
	RequestCutStack(0, MainMenu::NAME);
}

void PauseMenu::HandleQuitRequest(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	if ( m_IsQuitting )
	{
		return;
	}

	m_Modal.SetTitle("Quit");
	m_Modal.SetButtons({"No", "Yes"});
	m_PageModel.showModal = true;
	DirtyVariable(NAME_SHOW_MODAL);
}

void PauseMenu::HandleQuitConfirm(Rml::Event&, size_t buttonIndex, const Rml::Variant&)
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
