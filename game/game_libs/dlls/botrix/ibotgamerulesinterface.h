#pragma once

class CBasePlayer;
struct edict_s;

class IBotGameRulesInterface
{
public:
	virtual ~IBotGameRulesInterface() = default;

	virtual void ServerActivate() = 0;
	virtual void ServerDeactivate() = 0;
	virtual void ClientDisconnect(struct edict_s* entity) = 0;
	virtual void ClientPutInServer(struct edict_s* entity) = 0;
	virtual bool ClientCommand(CBasePlayer* player, const char* command) = 0;
	virtual void ClientTeamChange(struct edict_s* entity, int newTeam) = 0;
	virtual void ClientRespawn(struct edict_s* entity) = 0;
	virtual void ClientHurt(struct edict_s* victim, struct edict_s* attacker) = 0;
	virtual void ClientKilled(struct edict_s* victim, struct edict_s* killer) = 0;
	virtual void Think() = 0;
};
