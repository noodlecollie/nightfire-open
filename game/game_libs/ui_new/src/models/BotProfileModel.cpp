#include "models/BotProfileModel.h"
#include "framework/DataModelConstructorHelper.h"

static constexpr const char* const SUBNAME_PROFILE_NAME = "profileName";
static constexpr const char* const SUBNAME_SKIN = "skin";
static constexpr const char* const SUBNAME_PLAYER_NAME = "playerName";
static constexpr const char* const NAME_BOT_PROFILES = "botProfiles";

BotProfileModel::BotProfileModel(BaseMenu* parentMenu) :
	BaseMenuObserver(parentMenu)
{
}

bool BotProfileModel::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	DataModelConstructorHelper helper(constructor);

	bool profileDataRegistered = helper.RegisterStructIdempotent<ProfileData>(
		[](Rml::StructHandle<ProfileData>& handle)
		{
			return handle.RegisterMember(SUBNAME_PROFILE_NAME, &ProfileData::profileName) &&
				handle.RegisterMember(SUBNAME_SKIN, &ProfileData::skin) &&
				handle.RegisterMember(SUBNAME_PLAYER_NAME, &ProfileData::playerName);
		}
	);

	if ( !profileDataRegistered )
	{
		return false;
	}

	if ( !helper.RegisterArrayIdempotent<std::vector<ProfileData>>() )
	{
		return false;
	}

	if ( !constructor.Bind(NAME_BOT_PROFILES, &m_BotProfiles) )
	{
		return false;
	}

	return true;
}
