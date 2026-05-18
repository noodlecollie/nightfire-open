#include "menus/options/BaseOptionsMenu.h"

BaseOptionsMenu::BaseOptionsMenu(const char* name, const char* rmlFilePath) :
	MenuPage(name, rmlFilePath),
	m_MenuFrameDataBinding(this),
	m_TabBarDataBinding(this, Name())
{
	m_TabBarDataBinding.SetActiveTabChangeCallback(
		[this](const Rml::String& menu)
		{
			if ( menu != Name() )
			{
				RequestPop(menu);
			}
		}
	);
}
