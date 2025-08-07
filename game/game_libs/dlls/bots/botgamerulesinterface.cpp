#include "botgamerulesinterface.h"

#include "utlvector.h"
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "enginecallback.h"
#include "gamerules.h"
#include "client.h"
#include "nodes.h"
#include "bot.h"
#include "botprofiles/botprofileparser.h"
#include "botprofiles/botprofiletable.h"
#include "bot_misc.h"
#include "botregister.h"
#include "mp_utils.h"

CBotGameRulesInterface::CBotGameRulesInterface(CGameRules& parent) :
	m_pParent(&parent),
	m_BotFactory(),
	m_bSpawnedRegisterBots(false)
{
	ASSERT(m_pParent);

	// This class will be instantiated on world precache, when the game rules are created.
	// Reload the bot profiles at this point.
	m_BotFactory.LoadBotProfiles();
}

void CBotGameRulesInterface::ServerActivate()
{
}

void CBotGameRulesInterface::ServerDeactivate()
{
}

void CBotGameRulesInterface::ClientDisconnect(struct edict_s*)
{
}

void CBotGameRulesInterface::ClientPutInServer(struct edict_s* entity)
{
	if ( !entity || !(entity->v.flags & FL_FAKECLIENT) )
	{
		return;
	}

	// This step converts the entity into a bot
	// by linking up the edict to the CBaseBot class.
	CBaseBot* bot = GetClassPtrFromEdict<CBaseBot>(entity);

	// Should never happen, but to be safe:
	if ( !bot )
	{
		return;
	}

	m_pParent->PlayerThink(bot);
}

bool CBotGameRulesInterface::ClientCommand(CBasePlayer*, const char*)
{
	return false;
}

void CBotGameRulesInterface::ClientTeamChange(struct edict_s*, int)
{
	// Nothing to do right now.
}

void CBotGameRulesInterface::ClientRespawn(struct edict_s*)
{
	// Nothing to do right now.
}

void CBotGameRulesInterface::ClientHurt(struct edict_s*, struct edict_s*)
{
	// Nothing to do right now.
}

void CBotGameRulesInterface::ClientKilled(struct edict_s*, struct edict_s*)
{
	// Nothing to do right now.
}

void CBotGameRulesInterface::Think()
{
	for ( int clientIndex = 1; clientIndex <= gpGlobals->maxClients; ++clientIndex )
	{
		CBasePlayer* player = MPUtils::CBasePlayerFromIndex(clientIndex);

		if ( !player )
		{
			continue;
		}

		if ( player->IsFakeClient() )
		{
			player->BotThink();
		}
		else if ( player->IsNetClient() && player->IsAlive() )
		{
			// We do this here for now, as if a real player is in the game then we know it's safe
			// to spawn bots. If we find a way of determining when it's safe otherwise, move this
			// logic there instead.
			if ( !m_bSpawnedRegisterBots )
			{
				SpawnBotsInRegister();
				m_bSpawnedRegisterBots = true;
			}

			WorldGraph.MarkLocationFavorable(player->pev->origin);
		}
	}

	BotGlobals.HandleAutoAdjDifficulty();
}

CBotFactory& CBotGameRulesInterface::BotFactory()
{
	return m_BotFactory;
}

const CBotFactory& CBotGameRulesInterface::BotFactory() const
{
	return m_BotFactory;
}

void CBotGameRulesInterface::SpawnBotsInRegister()
{
	CBotRegister& reg = CBotRegister::StaticInstance();

	for ( uint32_t index = 0; index < reg.Count(); ++index )
	{
		const CUtlString profileName = reg.ProfileName(index);
		const CUtlString customName = reg.CustomName(index);

		m_BotFactory.TryCreateBot(profileName, customName);
	}
}
