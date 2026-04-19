#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/MenuFrameDataBinding.h"

class MainMenu : public MenuPage
{
public:
	static const char* const NAME;

	MainMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	MenuFrameDataBinding m_MenuFrameDataBinding;
};
