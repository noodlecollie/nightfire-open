#pragma once

#include "models/CvarModel.h"
#include "framework/BaseMenuObserver.h"

#include <limits>

class BaseMenu;

class CreateMultiplayerGamePageModel : private BaseMenuObserver
{
public:
	CreateMultiplayerGamePageModel(BaseMenu* parentMenu);

	void ValidateAndSubmit(const Rml::String& variableName);

protected:
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	class ValidatorProxy
	{
	public:
		ValidatorProxy(
			CvarModel& cvarModel,
			const Rml::String& cvarName,
			int defaultValue = 0,
			int min = std::numeric_limits<int>::min(),
			int max = std::numeric_limits<int>::max()
		);

		bool Bind(
			Rml::DataModelConstructor& constructor,
			Rml::String bufferedValueName,
			Rml::String enabledValueName = Rml::String()
		);

		void ValidateAndSubmit();

		const Rml::String& VariableName() const;

	private:
		int Clamp(int value) const;

		CvarModel& m_CvarModel;
		CvarDataVar<int>* m_Cvar = nullptr;
		int m_BufferedValue = 0;
		bool m_EnabledValue = false;
		int m_Min = std::numeric_limits<int>::min();
		int m_Max = std::numeric_limits<int>::max();

		Rml::DataModelHandle m_ModelHandle;
		Rml::String m_BufferedValueName;
	};

	static constexpr const char* const GAME_MODE_DEATHMATCH = "dm";

	CvarModel m_CvarModel;
	Rml::String m_GameMode = GAME_MODE_DEATHMATCH;

	ValidatorProxy m_TimeLimit;
	ValidatorProxy m_ScoreLimit;
	ValidatorProxy m_MaxPlayers;
};
