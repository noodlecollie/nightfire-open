#pragma once

#include "menus/temp_new/NewBaseOptionsMenu.h"
#include <RmlUi/Core/DataModelHandle.h>
#include "components/TooltipComponent.h"
#include "models/CvarModel.h"
#include "framework/CvarDataVar.h"

class NewMouseOptionsMenu : public NewBaseOptionsMenu
{
public:
	NewMouseOptionsMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	CvarModel m_CvarModel;
	TooltipComponent m_TooltipComponent;
	CvarDataVar<float>* m_MousePitch = nullptr;
};
