#include "menus/OptionsMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>
#include "rmlui/Utils.h"
#include "UIDebug.h"
#include "udll_int.h"

static constexpr const char* const PROP_ACTIVE_TAB = "activeTab";
static constexpr const char* const PROP_SHOW_MODAL = "showModal";
static constexpr const char* const EVENT_REBIND_KEY = "rebindKey";

OptionsMenu::OptionsMenu() :
	MenuPage("options_menu", "resource/rml/options_menu.rml"),
	m_Modal(this, "options_modal"),
	m_ShowHideEventListener(this, &OptionsMenu::ProcessShowHideEvents),
	m_KeyEventListener(this, &OptionsMenu::ProcessKeyEvents)
{
	m_Modal.SetButtonClickCallback(
		[this](Rml::Event& event, size_t buttonIndex)
		{
			HandleModelButtonClicked(event, buttonIndex);
		}
	);
}

bool OptionsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) || !m_MenuFrameDataBinding.SetUpDataBindings(constructor) ||
		 !m_KeyBindings.SetUpDataBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(PROP_ACTIVE_TAB, &m_PageModel.activeTab) ||
		 !constructor.Bind(PROP_SHOW_MODAL, &m_PageModel.showModal) ||
		 !constructor.BindEventCallback(EVENT_REBIND_KEY, &OptionsMenu::HandleRebindKeyEvent, this) )
	{
		return false;
	}

	// TODO: Swap this out for a "reset to defaults" button.
	m_KeyBindings.Reset();
	m_ModelHandle = constructor.GetModelHandle();

	return true;
}

void OptionsMenu::OnEndDocumentLoaded()
{
	MenuPage::OnEndDocumentLoaded();

	Rml::ElementDocument* document = Document();

	document->AddEventListener(Rml::EventId::Show, &m_ShowHideEventListener);
	document->AddEventListener(Rml::EventId::Hide, &m_ShowHideEventListener);
	document->AddEventListener(Rml::EventId::Keydown, &m_KeyEventListener);
}

void OptionsMenu::OnBeginDocumentUnloaded()
{
	Rml::ElementDocument* document = Document();

	document->RemoveEventListener(Rml::EventId::Show, &m_ShowHideEventListener);
	document->RemoveEventListener(Rml::EventId::Hide, &m_ShowHideEventListener);
	document->RemoveEventListener(Rml::EventId::Keydown, &m_KeyEventListener);

	MenuPage::OnBeginDocumentUnloaded();
}

void OptionsMenu::ProcessShowHideEvents(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Show:
		case Rml::EventId::Hide:
		{
			ResetRebindingRow();
			break;
		}

		default:
		{
			break;
		}
	}
}

void OptionsMenu::ProcessKeyEvents(Rml::Event& event)
{
	ASSERT(event.GetId() == Rml::EventId::Keydown);

	if ( !m_PageModel.showModal )
	{
		return;
	}

	if ( GetEventKeyId(event) == Rml::Input::KI_ESCAPE )
	{
		ResetRebindingRow();
	}
	else
	{
		gEngfuncs.Con_Printf(
			"Key pressed: %d\n",
			RmlKeyToEngineKey(static_cast<Rml::Input::KeyIdentifier>(GetEventKeyId(event)))
		);
	}
}

void OptionsMenu::HandleRebindKeyEvent(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& arguments)
{
	if ( arguments.size() < 2 )
	{
		ASSERT(false);
		return;
	}

	Rml::String consoleCommand;
	int bindIndex = 0;

	if ( !arguments[0].GetInto(consoleCommand) || !arguments[1].GetInto(bindIndex) )
	{
		ASSERT(false);
		return;
	}

	HandleRebindKeyEvent(consoleCommand, bindIndex);
}

void OptionsMenu::HandleRebindKeyEvent(const Rml::String& consoleCommand, int bindIndex)
{
	ResetRebindingRow();
	ASSERT(m_RebindingRow == INVALID_ROW);

	if ( bindIndex != 0 && bindIndex != 1 )
	{
		ASSERT(false);
		return;
	}

	if ( !m_KeyBindings.RowForConsoleCommand(consoleCommand, m_RebindingRow) )
	{
		ASSERT(false);
		return;
	}

	m_KeyBindings.SetIsRebinding(m_RebindingRow, bindIndex == 0, true);
	ShowModal(true);
	SetRequestPopOnEscapeKey(false);
}

void OptionsMenu::HandleModelButtonClicked(Rml::Event&, size_t buttonIndex)
{
	// At the moment, we just assume any click is the cancel button,
	// because that's the only button we've added.
	ASSERT(buttonIndex == 0);

	ResetRebindingRow();
}

void OptionsMenu::ResetRebindingRow()
{
	if ( m_RebindingRow != INVALID_ROW )
	{
		m_KeyBindings.SetIsRebinding(m_RebindingRow, true, false);
		m_KeyBindings.SetIsRebinding(m_RebindingRow, false, false);

		m_RebindingRow = INVALID_ROW;
	}

	ShowModal(false);
	SetRequestPopOnEscapeKey(true);
}

void OptionsMenu::ShowModal(bool show)
{
	if ( m_PageModel.showModal != show )
	{
		m_PageModel.showModal = show;
		m_ModelHandle.DirtyVariable(PROP_SHOW_MODAL);
	}
}
