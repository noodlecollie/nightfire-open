#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/MenuFrameDataBinding.h"

class MultiplayerMenu : public MenuPage
{
public:
	MultiplayerMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	MenuFrameDataBinding m_MenuFrameDataBinding;
};
