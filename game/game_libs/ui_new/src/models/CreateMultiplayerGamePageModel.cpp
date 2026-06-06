#include "models/CreateMultiplayerGamePageModel.h"
#include <algorithm>

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
static constexpr int TIME_LIMIT_DEFAULT = 10;
static constexpr int SCORE_LIMIT_MIN = 1;
static constexpr int SCORE_LIMIT_MAX = 999;
static constexpr int SCORE_LIMIT_DEFAULT = 10;
static constexpr int MAX_PLAYERS_MIN = 4;
static constexpr int MAX_PLAYERS_MAX = 32;
static constexpr int MAX_PLAYERS_DEFAULT = 16;

CreateMultiplayerGamePageModel::CreateMultiplayerGamePageModel(BaseMenu* parentMenu) :
	BaseMenuObserver(parentMenu),
	m_CvarModel(parentMenu),
	m_TimeLimit(m_CvarModel, "mp_timelimit", TIME_LIMIT_DEFAULT, TIME_LIMIT_MIN, TIME_LIMIT_MAX),
	m_ScoreLimit(m_CvarModel, "mp_fraglimit", SCORE_LIMIT_DEFAULT, SCORE_LIMIT_MIN, SCORE_LIMIT_MAX),
	m_MaxPlayers(m_CvarModel, "maxplayers", MAX_PLAYERS_DEFAULT, MAX_PLAYERS_MIN, MAX_PLAYERS_MAX)
{
	m_CvarModel.AddEntry<bool>(NAME_FRIENDLY_FIRE, "mp_friendlyfire");
	m_CvarModel.AddEntry<Rml::String>(NAME_SERVER_NAME, "hostname");
}

// Maybe this will need to be made dynamic at some point later.
void CreateMultiplayerGamePageModel::ValidateAndSubmit(const Rml::String& variableName)
{
	if ( variableName == m_TimeLimit.VariableName() )
	{
		m_TimeLimit.ValidateAndSubmit();
		return;
	}

	if ( variableName == m_ScoreLimit.VariableName() )
	{
		m_ScoreLimit.ValidateAndSubmit();
		return;
	}

	if ( variableName == m_MaxPlayers.VariableName() )
	{
		m_MaxPlayers.ValidateAndSubmit();
		return;
	}
}

bool CreateMultiplayerGamePageModel::SetUpDataModelBindings(Rml::DataModelConstructor& constructor)
{
	if ( !constructor.Bind(NAME_GAME_MODE, &m_GameMode) )
	{
		return false;
	}

	if ( !m_TimeLimit.Bind(constructor, NAME_TIME_LIMIT, NAME_HAS_TIME_LIMIT) )
	{
		return false;
	}

	if ( !m_ScoreLimit.Bind(constructor, NAME_SCORE_LIMIT, NAME_HAS_SCORE_LIMIT) )
	{
		return false;
	}

	if ( !m_MaxPlayers.Bind(constructor, NAME_MAX_PLAYERS) )
	{
		return false;
	}

	return true;
}

CreateMultiplayerGamePageModel::ValidatorProxy::ValidatorProxy(
	CvarModel& cvarModel,
	const Rml::String& cvarName,
	int defaultValue,
	int min,
	int max
) :
	m_CvarModel(cvarModel),
	m_Min(min),
	m_Max(max)
{
	ASSERT(!cvarName.empty());

	m_Cvar = m_CvarModel.AddEntry<int>("validated_cvar_" + cvarName, cvarName);
	ASSERT(m_Cvar);

	if ( m_Max < m_Min )
	{
		std::swap(m_Min, m_Max);
	}

	m_BufferedValue = Clamp(defaultValue);
}

bool CreateMultiplayerGamePageModel::ValidatorProxy::Bind(
	Rml::DataModelConstructor& constructor,
	Rml::String bufferedValueName,
	Rml::String enabledValueName
)
{
	if ( !m_Cvar )
	{
		return false;
	}

	const bool bufferedValueResult = constructor.BindFunc(
		bufferedValueName,
		[this](Rml::Variant& outVal)
		{
			outVal = Rml::Variant(m_BufferedValue);
		},
		[this](const Rml::Variant& inVal)
		{
			m_BufferedValue = inVal.Get<int>();
		}
	);

	if ( !bufferedValueResult )
	{
		return false;
	}

	if ( !enabledValueName.empty() )
	{
		const bool enabledValueResult = constructor.BindFunc(
			enabledValueName,
			[this](Rml::Variant& outVal)
			{
				outVal = Rml::Variant(m_EnabledValue);
			},
			[this](const Rml::Variant& inVal)
			{
				m_EnabledValue = inVal.Get<bool>();
				ValidateAndSubmit();
			}
		);

		if ( !enabledValueResult )
		{
			return false;
		}
	}
	else
	{
		// Always remain enabled if we're not binding this.
		m_EnabledValue = true;
	}

	const bool changeListenerResult = m_CvarModel.SetChangeListener(
		m_Cvar->Name(),
		[this](const Rml::Variant& newVal)
		{
			const int newValue = Clamp(newVal.Get<int>());

			if ( newValue != m_BufferedValue )
			{
				m_BufferedValue = newValue;

				if ( m_ModelHandle )
				{
					m_ModelHandle.DirtyVariable(m_BufferedValueName);
				}
			}
		}
	);

	if ( !changeListenerResult )
	{
		return false;
	}

	m_ModelHandle = constructor.GetModelHandle();
	m_BufferedValueName = std::move(bufferedValueName);

	return true;
}

void CreateMultiplayerGamePageModel::ValidatorProxy::ValidateAndSubmit()
{
	const int validatedValue = Clamp(m_BufferedValue);
	const bool different = validatedValue != m_BufferedValue;
	m_BufferedValue = validatedValue;

	m_Cvar->SetValue(m_EnabledValue ? m_BufferedValue : 0);

	if ( different && m_ModelHandle )
	{
		m_ModelHandle.DirtyVariable(m_BufferedValueName);
	}
}

const Rml::String& CreateMultiplayerGamePageModel::ValidatorProxy::VariableName() const
{
	return m_BufferedValueName;
}

int CreateMultiplayerGamePageModel::ValidatorProxy::Clamp(int value) const
{
	return std::max(m_Min, std::min(value, m_Max));
}
