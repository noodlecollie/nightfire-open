#pragma once

#include "menus/options/BaseOptionsMenu.h"
#include "models/CvarModel.h"

class GameplayOptionsMenu : public BaseOptionsMenu
{
public:
	GameplayOptionsMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	CvarModel m_CvarModel;
};
