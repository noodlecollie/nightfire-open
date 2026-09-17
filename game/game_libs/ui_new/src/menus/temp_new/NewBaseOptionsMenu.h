#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/OptionsTabBarDataBinding.h"

class NewBaseOptionsMenu : public MenuPage
{
protected:
	NewBaseOptionsMenu(const char* name, const char* rmlFilePath);

	OptionsTabBarDataBinding m_TabBarDataBinding;
};
