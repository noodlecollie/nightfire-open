#include "menus/MainMenu.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Event.h>
#include <RmlUi/Core/Element.h>

const char* const MainMenu::NAME = "main_menu";

MainMenu::MainMenu() :
	BaseMenu(NAME, "resource/rml/main_menu.rml")
{
}

void MainMenu::Update(float)
{
}

bool MainMenu::SetUpDataBindingsInternal(Rml::DataModelConstructor& constructor)
{
	if ( !m_MenuFrameDataBinding.SetUpDataBindings(constructor) )
	{
		return false;
	}

	return true;
}
