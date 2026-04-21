#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/MenuFrameDataBinding.h"

class CreditsMenu : public MenuPage
{
public:
	CreditsMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	MenuFrameDataBinding m_MenuFrameDataBinding;
};
