#pragma once

#include "menus/options/BaseOptionsMenu.h"
#include "framework/CvarDataVar.h"

class GameplayOptionsMenu : public BaseOptionsMenu
{
public:
	GameplayOptionsMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	CvarDataVar<bool> m_EnableCrosshair;
};
