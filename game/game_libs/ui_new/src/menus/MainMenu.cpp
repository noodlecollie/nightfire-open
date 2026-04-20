#include "menus/MainMenu.h"
#include "udll_int.h"

static constexpr const char* const NAME_SHOW_CONSOLE_BUTTON = "showConsoleButton";
static constexpr const char* const EVENT_SHOW_DEVELOPER_CONSOLE = "showDeveloperConsole";

const char* const MainMenu::NAME = "main_menu";

MainMenu::MainMenu() :
	MenuPage(NAME, "resource/rml/main_menu.rml"),
	m_MenuFrameDataBinding(this)
{
}

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
		[this](Rml::Variant& outValue)
		{
			outValue = Rml::Variant(ShouldPop(""));
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
	return gpGlobals->developer != 0;
}

void MainMenu::HandleShowDeveloperConsole(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&)
{
	RequestPop();
}
