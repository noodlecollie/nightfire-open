#include "botrix/botrixgamerulesinterface.h"
#include "botrix/server_plugin.h"

namespace Botrix
{
	CBotrixGameRulesInterface::CBotrixGameRulesInterface()
	{
		CBotrixServerPlugin::Init();
	}

	CBotrixGameRulesInterface::~CBotrixGameRulesInterface()
	{
		CBotrixServerPlugin::Deinit();
	}

	void CBotrixGameRulesInterface::ServerActivate()
	{
		CBotrixServerPlugin::ServerActivate();
	}

	void CBotrixGameRulesInterface::ServerDeactivate()
	{
		CBotrixServerPlugin::ServerDeactivate();
	}

	void CBotrixGameRulesInterface::ClientDisconnect(struct edict_s* entity)
	{
		CBotrixServerPlugin::ClientDisconnect(entity);
	}

	void CBotrixGameRulesInterface::ClientPutInServer(struct edict_s* entity)
	{
		CBotrixServerPlugin::ClientPutInServer(entity);
	}

	bool CBotrixGameRulesInterface::ClientCommand(CBasePlayer* player, const char* command)
	{
		return CBotrixServerPlugin::ClientCommand(player, command);
	}

	void CBotrixGameRulesInterface::ClientTeamChange(struct edict_s* entity, int newTeam)
	{
		CBotrixServerPlugin::ClientTeamChange(entity, newTeam);
	}

	void CBotrixGameRulesInterface::ClientRespawn(struct edict_s* entity)
	{
		CBotrixServerPlugin::ClientRespawn(entity);
	}

	void CBotrixGameRulesInterface::ClientHurt(struct edict_s* victim, struct edict_s* attacker)
	{
		CBotrixServerPlugin::ClientHurt(victim, attacker);
	}

	void CBotrixGameRulesInterface::ClientKilled(struct edict_s* victim, struct edict_s* killer)
	{
		CBotrixServerPlugin::ClientKilled(victim, killer);
	}

	void CBotrixGameRulesInterface::Think()
	{
		CBotrixServerPlugin::Think();
	}

	edict_t* CBotrixGameRulesInterface::GetListenServerClient() const
	{
		return CBotrixServerPlugin::GetListenServerClient();
	}
}  // namespace Botrix
