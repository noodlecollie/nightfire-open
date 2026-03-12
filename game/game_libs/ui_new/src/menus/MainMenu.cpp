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

bool MainMenu::SetUpDataBindings(Rml::Context* context)
{
	Rml::DataModelConstructor constructor = context->CreateDataModel("mainmenumodel");

	if ( !constructor )
	{
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Failed to construct main menu data model");
		return false;
	}

	if ( !m_MenuFrameDataBinding.SetUpDataBindings(context, constructor) )
	{
		return false;
	}

	return true;
}
