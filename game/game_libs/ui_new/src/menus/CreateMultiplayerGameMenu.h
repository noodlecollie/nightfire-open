#pragma once

#include "framework/MenuPage.h"
#include "framework/MenuEventListenerObject.h"
#include "templatebindings/MenuFrameDataBinding.h"
#include "models/CreateMultiplayerGamePageModel.h"

class CreateMultiplayerGameMenu : public MenuPage
{
public:
	CreateMultiplayerGameMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	void ProcessInputFieldEvent(Rml::Event& event);
	void HandleShowEvent(Rml::Event& event);
	void HandleCreateGame(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);

	MenuFrameDataBinding m_MenuFrameDataBinding;
	CreateMultiplayerGamePageModel m_PageModel;
	MenuEventListenerObject m_InputFieldEventListener;
	MenuEventListenerObject m_ShowListener;
};
