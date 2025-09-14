#include "botrix/botgamerulesinterface.h"
#include "botrix/server_plugin.h"

namespace Botrix
{
	CBotGameRulesInterface::CBotGameRulesInterface()
	{
		CBotrixServerPlugin::Init();
	}

	CBotGameRulesInterface::~CBotGameRulesInterface()
	{
		CBotrixServerPlugin::Deinit();
	}

	void CBotGameRulesInterface::ServerActivate()
	{
		CBotrixServerPlugin::ServerActivate();
	}

	void CBotGameRulesInterface::ServerDeactivate()
	{
		CBotrixServerPlugin::ServerDeactivate();
	}

	void CBotGameRulesInterface::ClientDisconnect(struct edict_s* entity)
	{
		CBotrixServerPlugin::ClientDisconnect(entity);
	}

	void CBotGameRulesInterface::ClientPutInServer(struct edict_s* entity)
	{
		CBotrixServerPlugin::ClientPutInServer(entity);
	}

	bool CBotGameRulesInterface::ClientCommand(CBasePlayer* player, const char* command)
	{
		return CBotrixServerPlugin::ClientCommand(player, command);
	}

	void CBotGameRulesInterface::ClientTeamChange(struct edict_s* entity, int newTeam)
	{
		CBotrixServerPlugin::ClientTeamChange(entity, newTeam);
	}

	void CBotGameRulesInterface::ClientRespawn(struct edict_s* entity)
	{
		CBotrixServerPlugin::ClientRespawn(entity);
	}

	void CBotGameRulesInterface::ClientHurt(struct edict_s* victim, struct edict_s* attacker)
	{
		CBotrixServerPlugin::ClientHurt(victim, attacker);
	}

	void CBotGameRulesInterface::ClientKilled(struct edict_s* victim, struct edict_s* killer)
	{
		CBotrixServerPlugin::ClientKilled(victim, killer);
	}

	void CBotGameRulesInterface::Think()
	{
		CBotrixServerPlugin::Think();
	}

	edict_t* CBotGameRulesInterface::GetListenServerClient() const
	{
		return CBotrixServerPlugin::GetListenServerClient();
	}
}  // namespace Botrix
