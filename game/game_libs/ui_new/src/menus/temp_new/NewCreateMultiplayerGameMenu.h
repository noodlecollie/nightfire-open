#pragma once

#include "framework/MenuPage.h"
#include "framework/MenuEventListenerObject.h"
#include "models/CreateMultiplayerGamePageModel.h"
#include "components/TooltipComponent.h"
#include "components/StudioMDLSceneComponent.h"

class NewCreateMultiplayerGameMenu : public MenuPage
{
public:
	NewCreateMultiplayerGameMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	void ProcessInputFieldEvent(Rml::Event& event);
	void HandleShowEvent(Rml::Event& event);
	void HandleSelectMap(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList& args);
	void HandleCreateGame(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);

	TooltipComponent m_TooltipComponent;
	StudioMDLSceneComponent m_SceneComponent;
	CreateMultiplayerGamePageModel m_PageModel;
	MenuEventListenerObject m_InputFieldEventListener;
	MenuEventListenerObject m_ShowListener;
};
