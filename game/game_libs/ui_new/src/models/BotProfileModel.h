#pragma once

#include <RmlUi/Core/Types.h>
#include "framework/BaseMenuObserver.h"

class BotProfileModel : private BaseMenuObserver
{
public:
	struct ProfileData
	{
		Rml::String profileName;
		Rml::String skin;
		Rml::String playerName;
	};

	explicit BotProfileModel(BaseMenu* parentMenu);

protected:
	bool SetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

private:
	std::vector<ProfileData> m_BotProfiles;
};
