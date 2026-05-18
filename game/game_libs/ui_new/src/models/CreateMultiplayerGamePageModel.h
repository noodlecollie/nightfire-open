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

	bool BindCvarProxy(
		Rml::DataModelConstructor& constructor,
		DataVar<int>* dataVar,
		CvarDataVar<int>* cvar,
		int min,
		int max,
		DataVar<bool>* enabledCheck = nullptr
	);

	CvarModel m_CvarModel;
	Rml::String m_GameMode = GAME_MODE_DEATHMATCH;
	DataVar<int> m_TimeLimit;
	DataVar<bool> m_HasTimeLimit;
	DataVar<int> m_ScoreLimit;
	DataVar<bool> m_HasScoreLimit;

	CvarDataVar<int>* m_CvarFragLimit = nullptr;
	CvarDataVar<int>* m_CvarTimeLimit = nullptr;
};
