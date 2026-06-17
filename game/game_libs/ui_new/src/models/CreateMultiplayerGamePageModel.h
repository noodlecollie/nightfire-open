#pragma once

#include "models/CvarModel.h"
#include "framework/BaseMenuObserver.h"
#include "game/Utils.h"

#include <limits>

class BaseMenu;

class CreateMultiplayerGamePageModel : private BaseMenuObserver
{
public:
	CreateMultiplayerGamePageModel(BaseMenu* parentMenu);

	void ValidateAndSubmit(const Rml::String& variableName);
	void SubmitAll();

	void RefreshMapList();

	Rml::String SelectedMap() const;
	void SetSelectedMap(Rml::String selectedMap);

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

	struct MapEntry
	{
		Rml::String fileName;
		Rml::String description;

		MapEntry() = default;

		// To keep this internal and independent from the utils function,
		// we just make a copy of the utils struct data.
		explicit MapEntry(const MapListing& listing) :
			fileName(listing.fileName),
			description(listing.description)
		{
		}
	};

	static constexpr const char* const GAME_MODE_DEATHMATCH = "dm";

	CvarModel m_CvarModel;
	Rml::String m_GameMode = GAME_MODE_DEATHMATCH;

	ValidatorProxy m_TimeLimit;
	ValidatorProxy m_ScoreLimit;
	ValidatorProxy m_MaxPlayers;

	std::vector<MapEntry> m_Maps;
	Rml::String m_SelectedMap;
};
