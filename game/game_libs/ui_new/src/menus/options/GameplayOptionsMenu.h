#pragma once

#include "menus/options/BaseOptionsMenu.h"
#include "models/CvarModel.h"

class GameplayOptionsMenu : public BaseOptionsMenu
{
public:
	GameplayOptionsMenu();

private:
	CvarModel m_CvarModel;
};
