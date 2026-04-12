#include "menus/options/GameplayOptionsMenu.h"

GameplayOptionsMenu::GameplayOptionsMenu() :
	BaseOptionsMenu("gameplay_options_menu", "resource/rml/gameplay_options_menu.rml"),
	m_EnableCrosshair("crosshair", "crosshair", true)
{
}

bool GameplayOptionsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !BaseOptionsMenu::OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	return m_EnableCrosshair.Bind(constructor);
}
