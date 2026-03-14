#pragma once

#include "framework/BaseMenu.h"
#include "templatebindings/MenuFrameDataBinding.h"

class MultiplayerMenu : public BaseMenu
{
public:
	MultiplayerMenu();

protected:
	bool SetUpDataBindingsInternal(Rml::DataModelConstructor& constructor) override;

private:
	MenuFrameDataBinding m_MenuFrameDataBinding;
};
