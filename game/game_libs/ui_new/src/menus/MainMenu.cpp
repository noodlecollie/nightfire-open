#include "menus/MainMenu.h"

const char* const MainMenu::NAME = "main_menu";

MainMenu::MainMenu() :
	BaseMenu(NAME, "resource/rml/main_menu.rml")
{
}

void MainMenu::Update(float /* currentTime */)
{
	// TODO
}
