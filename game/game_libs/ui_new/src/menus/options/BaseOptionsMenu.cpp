#include "menus/options/BaseOptionsMenu.h"

BaseOptionsMenu::BaseOptionsMenu(const char* name, const char* rmlFilePath) :
	MenuPage(name, rmlFilePath),
	m_MenuFrameDataBinding(this),
	m_TabBarDataBinding(this)
{
	m_TabBarDataBinding.SetGoToNewTabCallback(
		[this](const Rml::String& menu)
		{
			if ( menu != Name() )
			{
				RequestPop(menu);
			}
		}
	);
}
