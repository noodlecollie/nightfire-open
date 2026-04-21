#include "menus/CreditsMenu.h"

CreditsMenu::CreditsMenu() :
	MenuPage("credits_menu", "resource/rml/credits_menu.rml"),
	m_MenuFrameDataBinding(this)
{
}

bool CreditsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	return MenuPage::OnSetUpDataModelBindings(constructor) && m_MenuFrameDataBinding.SetUpDataBindings(constructor);
}
