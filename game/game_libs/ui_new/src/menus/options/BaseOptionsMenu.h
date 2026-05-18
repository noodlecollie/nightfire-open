#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/MenuFrameDataBinding.h"
#include "templatebindings/OptionsTabBarDataBinding.h"

class BaseOptionsMenu : public MenuPage
{
protected:
	BaseOptionsMenu(const char* name, const char* rmlFilePath);

	MenuFrameDataBinding m_MenuFrameDataBinding;
	OptionsTabBarDataBinding m_TabBarDataBinding;
};
