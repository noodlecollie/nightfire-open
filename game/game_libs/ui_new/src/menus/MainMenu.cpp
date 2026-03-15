#include "menus/MainMenu.h"

const char* const MainMenu::NAME = "main_menu";

MainMenu::MainMenu() :
	MenuPage(NAME, "resource/rml/main_menu.rml")
{
}

bool MainMenu::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return MenuPage::SetUpDataModelBindings(constructor) && m_MenuFrameDataBinding.SetUpDataBindings(constructor);
}
