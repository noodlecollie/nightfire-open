#pragma once

#include "components/TooltipComponent.h"
#include "menus/temp_new/NewBaseOptionsMenu.h"
#include "models/CvarModel.h"

class NewGameplayOptionsMenu : public NewBaseOptionsMenu
{
public:
	NewGameplayOptionsMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	void HandleTestVibration(Rml::DataModelHandle, Rml::Event&, const Rml::VariantList&);

	CvarModel m_CvarModel;
	TooltipComponent m_TooltipComponent;
	CvarDataVar<bool>* m_VibrationEnabled = nullptr;
};
