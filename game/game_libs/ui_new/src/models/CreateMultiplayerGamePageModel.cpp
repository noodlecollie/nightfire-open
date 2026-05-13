#include "models/CreateMultiplayerGamePageModel.h"

static constexpr const char* const NAME_FRAG_LIMIT = "fragLimit";
static constexpr const char* const NAME_TIME_LIMIT = "timeLimit";
static constexpr const char* const NAME_FRIENDLY_FIRE = "friendlyFire";
static constexpr const char* const NAME_SERVER_NAME = "serverName";
static constexpr const char* const NAME_GAME_MODE = "gameMode";
static constexpr const char* const NAME_MAX_PLAYERS = "maxPlayers";

CreateMultiplayerGamePageModel::CreateMultiplayerGamePageModel(BaseMenu* parentMenu) :
	m_CvarModel(parentMenu)
{
	m_CvarModel.AddEntry<int>(NAME_FRAG_LIMIT, "mp_fraglimit");
	m_CvarModel.AddEntry<int>(NAME_TIME_LIMIT, "mp_timelimit");
	m_CvarModel.AddEntry<int>(NAME_MAX_PLAYERS, "maxplayers");
	m_CvarModel.AddEntry<bool>(NAME_FRIENDLY_FIRE, "mp_friendlyfire");
	m_CvarModel.AddEntry<Rml::String>(NAME_SERVER_NAME, "hostname");
}

bool CreateMultiplayerGamePageModel::OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !m_CvarModel.SetUpDataBindings(constructor) )
	{
		return false;
	}

	if ( !constructor.Bind(NAME_GAME_MODE, &m_GameMode) )
	{
		return false;
	}

	return true;
}
