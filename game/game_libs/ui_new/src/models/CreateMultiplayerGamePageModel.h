#pragma once

#include "models/CvarModel.h"

class BaseMenu;

class CreateMultiplayerGamePageModel
{
public:
	CreateMultiplayerGamePageModel(BaseMenu* parentMenu);

	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor);

private:
	static constexpr const char* const GAME_MODE_DEATHMATCH = "dm";

	CvarModel m_CvarModel;

	Rml::String m_GameMode = GAME_MODE_DEATHMATCH;
};
