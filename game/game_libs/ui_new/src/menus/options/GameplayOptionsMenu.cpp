#include "menus/options/GameplayOptionsMenu.h"

GameplayOptionsMenu::GameplayOptionsMenu() :
	BaseOptionsMenu("gameplay_options_menu", "resource/rml/gameplay_options_menu.rml"),
	m_CvarModel(this)
{
	m_CvarModel.AddEntry<bool>("crosshair", "crosshair");
	m_CvarModel.AddEntry<bool>("autoaim", "sv_aim");
}

bool GameplayOptionsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return BaseOptionsMenu::OnSetUpDataModelBindings(constructor) && m_CvarModel.SetUpDataBindings(constructor);
}
