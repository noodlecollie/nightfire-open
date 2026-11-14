#include "menus/temp_new/NewBaseOptionsMenu.h"

NewBaseOptionsMenu::NewBaseOptionsMenu(const char* name, const char* rmlFilePath) :
	MenuPage(name, rmlFilePath),
	m_TabBarDataBinding(this)
{
	m_TabBarDataBinding.SetGoToNewTabCallback(
		[this](const Rml::String& menu)
		{
			RequestPop(menu);
		}
	);
}
