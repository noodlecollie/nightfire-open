#include "models/CreateMultiplayerGamePageModel.h"

static constexpr const char* const NAME_FRAG_LIMIT = "fragLimit";
static constexpr const char* const NAME_TIME_LIMIT = "timeLimit";
static constexpr const char* const NAME_FRIENDLY_FIRE = "friendlyFire";
static constexpr const char* const NAME_SERVER_NAME = "serverName";
static constexpr const char* const NAME_GAME_MODE = "gameMode";
static constexpr const char* const NAME_MAX_PLAYERS = "maxPlayers";
static constexpr const char* const NAME_HAS_TIME_LIMIT = "hasTimeLimit";
static constexpr const char* const NAME_HAS_SCORE_LIMIT = "hasScoreLimit";

static constexpr int TIME_LIMIT_MIN = 1;
static constexpr int TIME_LIMIT_MAX = 10 * 60;
static constexpr int FRAG_LIMIT_MIN = 1;
static constexpr int FRAG_LIMIT_MAX = 999;

CreateMultiplayerGamePageModel::CreateMultiplayerGamePageModel(BaseMenu* parentMenu) :
	m_CvarModel(parentMenu),
	m_HasTimeLimit {NAME_HAS_TIME_LIMIT, false},
	m_HasScoreLimit {NAME_HAS_SCORE_LIMIT, false}
{
	m_CvarFragLimit = m_CvarModel.AddEntry<int>("cvarFragLimit", "mp_fraglimit");
	m_CvarTimeLimit = m_CvarModel.AddEntry<int>("cvarTimeLimit", "mp_timelimit");
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

	if ( !BindCvarProxy(
			 constructor,
			 NAME_FRAG_LIMIT,
			 m_CvarFragLimit,
			 FRAG_LIMIT_MIN,
			 FRAG_LIMIT_MAX,
			 &m_HasScoreLimit
		 ) )
	{
		return false;
	}

	if ( !BindCvarProxy(
			 constructor,
			 NAME_TIME_LIMIT,
			 m_CvarTimeLimit,
			 TIME_LIMIT_MIN,
			 TIME_LIMIT_MAX,
			 &m_HasTimeLimit
		 ) )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	return true;
}

bool CreateMultiplayerGamePageModel::BindCvarProxy(
	Rml::DataModelConstructor& constructor,
	const char* rmlVarName,
	CvarDataVar<int>* cvar,
	int min,
	int max,
	DataVar<bool>* enabledCheck
)
{
	bool mainBindSuccess = constructor.BindFunc(
		rmlVarName,
		[cvar](Rml::Variant& out)
		{
			out = Rml::Variant(cvar->CachedValue());
		},
		[cvar, min, max, enabledCheck](const Rml::Variant& in)
		{
			if ( enabledCheck && !enabledCheck->value )
			{
				cvar->SetValue(0);
			}

			int value = in.Get<int>(0);

			if ( value < min )
			{
				value = min;
			}
			else if ( value > max )
			{
				value = max;
			}

			cvar->SetValue(value);
		}
	);

	if ( !enabledCheck || !mainBindSuccess )
	{
		return mainBindSuccess;
	}

	return constructor.BindFunc(
		enabledCheck->name,
		[enabledCheck](Rml::Variant& out)
		{
			out = Rml::Variant(enabledCheck->value);
		},
		[this, cvar, enabledCheck, rmlVarName](const Rml::Variant& in)
		{
			enabledCheck->value = in.Get<bool>();

			if ( !enabledCheck->value )
			{
				cvar->SetValue(0);
			}

			if ( m_ModelHandle )
			{
				m_ModelHandle.DirtyVariable(rmlVarName);
			}
		}
	);
}
