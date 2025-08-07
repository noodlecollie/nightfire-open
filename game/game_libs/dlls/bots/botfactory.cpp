#include <algorithm>
#include <random>
#include "botfactory.h"
#include "standard_includes.h"
#include "bot.h"
#include "util.h"
#include "EnginePublicAPI/eiface.h"
#include "cbase.h"
#include "client.h"
#include "botprofiles/botprofileparser.h"
#include "mp_utils.h"

void CBotFactory::LoadBotProfiles()
{
	const char* fileName = CVAR_GET_STRING("bot_profile_file");

	if ( !fileName || !(*fileName) )
	{
		// No bot profiles.
		return;
	}

	CUtlString filePath;
	filePath.Format("scripts/%s", fileName);

	CBotProfileParser parser(m_ProfileTable);

	if ( !parser.Parse(filePath) )
	{
		ALERT(at_error, "Could not load bot profiles from '%s'\n", filePath.String());
	}
}

void CBotFactory::CreateBots(uint32_t num)
{
	CUtlVector<CUtlString> randomProfileNameList;
	RandomProfileNameList(randomProfileNameList, num);

	if ( randomProfileNameList.Count() > 0 )
	{
		FOR_EACH_VEC(randomProfileNameList, index)
		{
			CreateBot(m_ProfileTable.GetProfile(randomProfileNameList[index]));
		}
	}
	else
	{
		// No profiles loaded - use default bots.
		for ( uint32_t index = 0; index < num; ++index )
		{
			CreateBot();
		}
	}
}

bool CBotFactory::TryCreateBot(const CUtlString& profileName, const CUtlString& playerName)
{
	if ( !m_ProfileTable.ProfileExists(profileName) )
	{
		ALERT(at_error, "Bot profile '%s' does not exist.\n", profileName.String());
		return false;
	}

	return CreateBot(m_ProfileTable.GetProfile(profileName), playerName);
}

bool CBotFactory::CreateBot(const CBotProfileTable::ProfileData* profile, const CUtlString& playerName)
{
	CUtlString name("Bot");

	if ( playerName.IsValid() )
	{
		name = playerName;
	}
	else if ( profile )
	{
		name = profile->playerName;
	}

	name = MPUtils::SanitisePlayerNetName(name);
	edict_t* bot = g_engfuncs.pfnCreateFakeClient(name.String());

	if ( !bot )
	{
		ALERT(at_error, "No free player slots to create new bot.\n");
		return false;
	}

	// Run the bot through the standard connection functions.
	// It'll eventually get passed back through this class.
	ClientPutInServer(bot);

	// Now that the bot is registered with us, set up profile-specific things.
	SetBotAttributesViaProfile(dynamic_cast<CBaseBot*>(CBaseEntity::Instance(&bot->v)), profile);
	ALERT(at_console, "Added bot '%s'\n", STRING(bot->v.netname));
	return true;
}

void CBotFactory::SetBotAttributesViaProfile(CBaseBot* bot, const CBotProfileTable::ProfileData* profile)
{
	if ( !bot || !profile )
	{
		return;
	}

	SetBotSkin(bot, profile->skin);
}

void CBotFactory::SetBotSkin(CBaseBot* bot, const CUtlString& skin)
{
	if ( !bot )
	{
		return;
	}

	// Annoyingly this takes a non-const char* (can we change this one day?),
	// but the function just uses it as const internally so it's OK to cast.
	g_engfuncs.pfnSetClientKeyValue(
		bot->entindex(),
		g_engfuncs.pfnGetInfoKeyBuffer(bot->edict()),
		"model",
		(char*)CUtlString(skin).String());
}

void CBotFactory::RandomProfileNameList(CUtlVector<CUtlString>& list, size_t count)
{
	list.Purge();

	if ( m_ProfileTable.Count() < 1 )
	{
		return;
	}

	list.EnsureCapacity(static_cast<int>(count));
	auto rng = std::default_random_engine {(unsigned int)RANDOM_LONG(0, 1000)};

	while ( static_cast<size_t>(list.Count()) < count )
	{
		// This has to be a std::vector in order to randomise it.
		std::vector<const char*> intermediateList;
		intermediateList.reserve(m_ProfileTable.Count());

		for ( CBotProfileTable::ConstIterator it = m_ProfileTable.CBegin(); it != m_ProfileTable.CEnd(); ++it )
		{
			intermediateList.push_back(it.Key().String());
		}

		std::shuffle(std::begin(intermediateList), std::end(intermediateList), rng);

		for ( const char* name : intermediateList )
		{
			list.AddToTail(CUtlString(name));

			if ( static_cast<size_t>(list.Count()) >= count )
			{
				break;
			}
		}
	}
}
