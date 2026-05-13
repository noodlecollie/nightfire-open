#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/MenuFrameDataBinding.h"

class CreateMultiplayerGameMenu : public MenuPage
{
public:
	CreateMultiplayerGameMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	struct PageModel
	{
		Rml::String gameType;
		Rml::String serverName = "Nightfire Open";
	};

	MenuFrameDataBinding m_MenuFrameDataBinding;
	PageModel m_PageModel;
	Rml::DataModelHandle m_ModelHandle;
};
