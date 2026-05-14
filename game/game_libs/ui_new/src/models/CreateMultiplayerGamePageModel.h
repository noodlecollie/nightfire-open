#pragma once

#include "models/CvarModel.h"
#include "framework/DataVar.h"

class BaseMenu;

class CreateMultiplayerGamePageModel
{
public:
	CreateMultiplayerGamePageModel(BaseMenu* parentMenu);

	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor);

private:
	static constexpr const char* const GAME_MODE_DEATHMATCH = "dm";

	bool BindCvarProxy(
		Rml::DataModelConstructor& constructor,
		const char* rmlVarName,
		CvarDataVar<int>* cvar,
		int min,
		int max,
		DataVar<bool>* enabledCheck = nullptr
	);

	CvarModel m_CvarModel;
	Rml::DataModelHandle m_ModelHandle;
	Rml::String m_GameMode = GAME_MODE_DEATHMATCH;
	DataVar<bool> m_HasTimeLimit;
	DataVar<bool> m_HasScoreLimit;

	CvarDataVar<int>* m_CvarFragLimit = nullptr;
	CvarDataVar<int>* m_CvarTimeLimit = nullptr;
};
