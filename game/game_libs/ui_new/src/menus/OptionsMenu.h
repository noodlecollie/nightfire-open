#pragma once

#include "framework/BaseMenu.h"
#include "templatebindings/MenuFrameDataBinding.h"

class OptionsMenu : public BaseMenu
{
public:
	OptionsMenu();

protected:
	bool SetUpDataBindingsInternal(Rml::DataModelConstructor& constructor) override;

private:
	MenuFrameDataBinding m_MenuFrameDataBinding;
};
