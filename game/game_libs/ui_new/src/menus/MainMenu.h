#pragma once

#include "framework/BaseMenu.h"
#include "templatebindings/MenuFrameDataBinding.h"

class MainMenu : public BaseMenu
{
public:
	static const char* const NAME;

	MainMenu();

protected:
	bool SetUpDataBindingsInternal(Rml::DataModelConstructor& constructor) override;

private:
	MenuFrameDataBinding m_MenuFrameDataBinding;
};
