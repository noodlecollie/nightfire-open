#pragma once

#include "models/CvarModel.h"
#include "framework/DataVar.h"
#include "framework/BaseMenuObserver.h"

class BaseMenu;

class CreateMultiplayerGamePageModel : private BaseMenuObserver
{
public:
	CreateMultiplayerGamePageModel(BaseMenu* parentMenu);

protected:
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	static constexpr const char* const GAME_MODE_DEATHMATCH = "dm";

	CvarModel m_CvarModel;
	Rml::String m_GameMode = GAME_MODE_DEATHMATCH;
	CvarDataVar<int>* m_CvarFragLimit = nullptr;
	CvarDataVar<int>* m_CvarTimeLimit = nullptr;
};
