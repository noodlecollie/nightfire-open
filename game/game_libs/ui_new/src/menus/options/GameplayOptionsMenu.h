#pragma once

#include "menus/options/BaseOptionsMenu.h"
#include "framework/CvarDataVar.h"
#include "framework/BaseCvarModel.h"

class GameplayOptionsMenu : public BaseOptionsMenu
{
public:
	GameplayOptionsMenu();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	BaseCvarModel m_CvarModel;
};
