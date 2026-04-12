#include "menus/options/BaseOptionsMenu.h"

BaseOptionsMenu::BaseOptionsMenu(const char* name, const char* rmlFilePath) :
	MenuPage(name, rmlFilePath),
	m_TabBarDataBinding(Name())
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

bool BaseOptionsMenu::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !MenuPage::OnSetUpDataModelBindings(constructor) )
	{
		return false;
	}

	return m_MenuFrameDataBinding.SetUpDataBindings(constructor) && m_TabBarDataBinding.SetUpDataBindings(constructor);
}
