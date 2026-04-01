#include "menus/OptionsMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>
#include "udll_int.h"
#include "UIDebug.h"

OptionsMenu::OptionsMenu() :
	MenuPage("options_menu", "resource/rml/options_menu.rml")
{
}

bool OptionsMenu::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::SetUpDataModelBindings(constructor) || !m_MenuFrameDataBinding.SetUpDataBindings(constructor) ||
		 !m_KeyBindings.SetUpDataBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind("activeTab", &m_PageModel.activeTab) )
	{
		return false;
	}

	constructor.BindEventCallback("rebindKey", &OptionsMenu::HandleRebindKeyEvent, this);

	// TODO: Swap this out for a "reset to defaults" button.
	m_KeyBindings.Reset();

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
}

void OptionsMenu::ResetRebindingRow()
{
	if ( m_RebindingRow != INVALID_ROW )
	{
		m_KeyBindings.SetIsRebinding(m_RebindingRow, true, false);
		m_KeyBindings.SetIsRebinding(m_RebindingRow, false, false);

		m_RebindingRow = INVALID_ROW;
	}
}
