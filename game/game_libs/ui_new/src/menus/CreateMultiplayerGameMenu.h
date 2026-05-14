#pragma once

#include "framework/MenuPage.h"
#include "templatebindings/MenuFrameDataBinding.h"
#include "models/CreateMultiplayerGamePageModel.h"

class CreateMultiplayerGameMenu : public MenuPage
{
public:
	CreateMultiplayerGameMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	void HandleCreateGame(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);

	MenuFrameDataBinding m_MenuFrameDataBinding;
	CreateMultiplayerGamePageModel m_PageModel;
};
