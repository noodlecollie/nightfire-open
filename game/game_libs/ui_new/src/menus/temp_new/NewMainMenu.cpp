#include "menus/temp_new/NewMainMenu.h"
#include "rmlui/Utils.h"
#include <RmlUi/Core/ElementDocument.h>
#include "udll_int.h"

static constexpr const char* const NAME_DEVELOPER_MODE = "developerMode";
static constexpr const char* const EVENT_SHOW_DEVELOPER_CONSOLE = "showDeveloperConsole";

const char* const NewMainMenu::NAME = "new_main_menu";

NewMainMenu::NewMainMenu() :
	MenuPage(NAME, "main_menu.rml"),
	m_KeyEventListener(this, &NewMainMenu::ProcessKeyEvent, {Rml::EventId::Keydown})
{
}

// TODO: Should handle quit etc here, instead of just having the
// RML page execute it as a console command.
bool NewMainMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.BindEventCallback(EVENT_SHOW_DEVELOPER_CONSOLE, &NewMainMenu::HandleShowDeveloperConsole, this) )
	{
		return false;
	}

	const bool bindSuccess = constructor.BindFunc(
		NAME_DEVELOPER_MODE,
		[](Rml::Variant& outValue)
		{
			outValue = Rml::Variant(gpGlobals->developer != 0);
		}
	);

	if ( !bindSuccess )
	{
		return false;
	}

	return true;
}

bool NewMainMenu::ShouldPop(const Rml::String&) const
{
	return false;
}

void NewMainMenu::ProcessKeyEvent(Rml::Event& event)
{
	switch ( event.GetId() )
	{
		case Rml::EventId::Keydown:
		{
			const int keyId = GetEventKeyId(event);

			if ( keyId == Rml::Input::KI_ESCAPE )
			{
				event.StopPropagation();
				RequestSwitchToConsole();
			}

			break;
		}

		default:
		{
			break;
		}
	}
}

void NewMainMenu::HandleShowDeveloperConsole(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	RequestSwitchToConsole();
}

void NewMainMenu::RequestSwitchToConsole()
{
	RequestSwitchFocus("console", NewMainMenu::NAME);
}
