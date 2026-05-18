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
static constexpr int SCOR_LIMIT_MIN = 1;
static constexpr int SCORE_LIMIT_MAX = 999;

CreateMultiplayerGamePageModel::CreateMultiplayerGamePageModel(BaseMenu* parentMenu) :
	BaseMenuObserver(parentMenu),
	m_CvarModel(parentMenu),
	m_TimeLimit {NAME_TIME_LIMIT, 10},
	m_HasTimeLimit {NAME_HAS_TIME_LIMIT, false},
	m_ScoreLimit {NAME_SCORE_LIMIT, 30},
	m_HasScoreLimit {NAME_HAS_SCORE_LIMIT, false}
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

	if ( !BindCvarProxy(
			 constructor,
			 &m_ScoreLimit,
			 m_CvarFragLimit,
			 SCOR_LIMIT_MIN,
			 SCORE_LIMIT_MAX,
			 &m_HasScoreLimit
		 ) )
	{
		return false;
	}

	if ( !BindCvarProxy(constructor, &m_TimeLimit, m_CvarTimeLimit, TIME_LIMIT_MIN, TIME_LIMIT_MAX, &m_HasTimeLimit) )
	{
		return false;
	}

	return true;
}

bool CreateMultiplayerGamePageModel::BindCvarProxy(
	Rml::DataModelConstructor& constructor,
	DataVar<int>* dataVar,
	CvarDataVar<int>* cvar,
	int min,
	int max,
	DataVar<bool>* enabledCheck
)
{
	bool mainBindSuccess = constructor.BindFunc(
		dataVar->name,
		[dataVar, enabledCheck](Rml::Variant& out)
		{
			out = Rml::Variant((enabledCheck && !enabledCheck->value) ? 0 : dataVar->value);
		},
		[dataVar, cvar, min, max, enabledCheck](const Rml::Variant& in)
		{
			int value = in.Get<int>(0);

			if ( value < min )
			{
				value = min;
			}
			else if ( value > max )
			{
				value = max;
			}

			// Always store the value we were provided.
			dataVar->value = value;

			// The value we actually write to the cvar depends on the enabled check.
			if ( enabledCheck && !enabledCheck->value )
			{
				cvar->SetValue(0);
			}
			else
			{
				cvar->SetValue(value);
			}
		}
	);

	if ( !mainBindSuccess || !enabledCheck )
	{
		return mainBindSuccess;
	}

	return constructor.BindFunc(
		enabledCheck->name,
		[enabledCheck](Rml::Variant& out)
		{
			out = Rml::Variant(enabledCheck->value);
		},
		[this, cvar, enabledCheck, dataVar](const Rml::Variant& in)
		{
			bool newVal = in.Get<bool>();

			if ( newVal == enabledCheck->value )
			{
				return;
			}

			enabledCheck->value = newVal;
			cvar->SetValue(enabledCheck->value ? dataVar->value : 0);
			DirtyVariable(dataVar->name);
		}
	);
}
