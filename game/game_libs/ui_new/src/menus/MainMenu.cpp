#include "menus/MainMenu.h"
#include "rmlui/Utils.h"
#include <RmlUi/Core/ElementDocument.h>
#include "udll_int.h"

static constexpr const char* const NAME_SHOW_CONSOLE_BUTTON = "showConsoleButton";
static constexpr const char* const EVENT_SHOW_DEVELOPER_CONSOLE = "showDeveloperConsole";

const char* const MainMenu::NAME = "main_menu";

MainMenu::MainMenu() :
	MenuPage(NAME, "resource/rml/main_menu.rml"),
	m_MenuFrameDataBinding(this),
	m_KeyEventListener(this, &MainMenu::ProcessKeyEvent, {Rml::EventId::Keydown})
{
}

// TODO: Should handle quit etc here, instead of just having the
// RML page execute it as a console command.
bool MainMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) || !m_MenuFrameDataBinding.SetUpDataBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.BindEventCallback(EVENT_SHOW_DEVELOPER_CONSOLE, &MainMenu::HandleShowDeveloperConsole, this) )
	{
		return false;
	}

	const bool bindSuccess = constructor.BindFunc(
		NAME_SHOW_CONSOLE_BUTTON,
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

bool MainMenu::ShouldPop(const Rml::String&) const
{
	return false;
}

void MainMenu::ProcessKeyEvent(Rml::Event& event)
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

void MainMenu::HandleShowDeveloperConsole(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	RequestSwitchToConsole();
}

void MainMenu::RequestSwitchToConsole()
{
	RequestSwitchFocus("console", MainMenu::NAME);
}
