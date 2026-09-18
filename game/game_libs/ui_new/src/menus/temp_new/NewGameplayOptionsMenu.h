#pragma once

#include "components/TooltipComponent.h"
#include "menus/temp_new/NewBaseOptionsMenu.h"
#include "models/CvarModel.h"

class NewGameplayOptionsMenu : public NewBaseOptionsMenu
{
public:
	NewGameplayOptionsMenu();

private:
	CvarModel m_CvarModel;
	TooltipComponent m_TooltipComponent;
};
