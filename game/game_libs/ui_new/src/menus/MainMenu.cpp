#include "menus/MainMenu.h"

const char* const MainMenu::NAME = "main_menu";

MainMenu::MainMenu() :
	MenuPage(NAME, "resource/rml/main_menu.rml")
{
}

bool MainMenu::SetUpDefaultDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return MenuPage::SetUpDefaultDataModelBindings(constructor) &&
		m_MenuFrameDataBinding.SetUpDataBindings(constructor);
}
