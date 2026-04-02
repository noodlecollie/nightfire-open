#include "menus/OptionsMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>
#include "UIDebug.h"

static constexpr const char* const PROP_ACTIVE_TAB = "activeTab";
static constexpr const char* const PROP_SHOW_MODAL = "showModal";
static constexpr const char* const EVENT_REBIND_KEY = "rebindKey";

OptionsMenu::OptionsMenu() :
	MenuPage("options_menu", "resource/rml/options_menu.rml"),
	m_Modal(this, "options_modal")
{
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
}

void OptionsMenu::ShowModal(bool show)
{
	if ( m_PageModel.showModal != show )
	{
		m_PageModel.showModal = show;
		m_ModelHandle.DirtyVariable(PROP_SHOW_MODAL);
	}
}
