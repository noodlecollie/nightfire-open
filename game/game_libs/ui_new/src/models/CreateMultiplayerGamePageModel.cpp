#include "models/CreateMultiplayerGamePageModel.h"

static constexpr const char* const NAME_SCORE_LIMIT = "scoreLimit";
static constexpr const char* const NAME_TIME_LIMIT = "timeLimit";
static constexpr const char* const NAME_FRIENDLY_FIRE = "friendlyFire";
static constexpr const char* const NAME_SERVER_NAME = "serverName";
static constexpr const char* const NAME_GAME_MODE = "gameMode";
static constexpr const char* const NAME_MAX_PLAYERS = "maxPlayers";
static constexpr const char* const NAME_HAS_TIME_LIMIT = "hasTimeLimit";
static constexpr const char* const NAME_HAS_SCORE_LIMIT = "hasScoreLimit";

static constexpr int TIME_LIMIT_MIN = 1;
static constexpr int TIME_LIMIT_MAX = 10 * 60;
static constexpr int SCORE_LIMIT_MIN = 1;
static constexpr int SCORE_LIMIT_MAX = 999;

CreateMultiplayerGamePageModel::CreateMultiplayerGamePageModel(BaseMenu* parentMenu) :
	BaseMenuObserver(parentMenu),
	m_CvarModel(parentMenu)
{
	m_CvarFragLimit = m_CvarModel.AddEntry<int>("cvarFragLimit", "mp_fraglimit");
	m_CvarTimeLimit = m_CvarModel.AddEntry<int>("cvarTimeLimit", "mp_timelimit");
	m_CvarModel.AddEntry<int>(NAME_MAX_PLAYERS, "maxplayers");
	m_CvarModel.AddEntry<bool>(NAME_FRIENDLY_FIRE, "mp_friendlyfire");
	m_CvarModel.AddEntry<Rml::String>(NAME_SERVER_NAME, "hostname");
}

bool CreateMultiplayerGamePageModel::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !constructor.Bind(NAME_GAME_MODE, &m_GameMode) )
	{
		return false;
	}

	return true;
}
