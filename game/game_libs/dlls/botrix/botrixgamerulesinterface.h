#pragma once

#include "bots/ibotgamerulesinterface.h"
#include "standard_includes.h"

class CBotrixGameRulesInterface : public IBotGameRulesInterface
{
public:
	CBotrixGameRulesInterface();
	~CBotrixGameRulesInterface();

	void ServerActivate() override;
	void ServerDeactivate() override;
	void ClientDisconnect(struct edict_s* entity) override;
	void ClientPutInServer(struct edict_s* entity) override;
	bool ClientCommand(CBasePlayer* player, const char* command) override;
	void ClientTeamChange(struct edict_s* entity, int newTeam) override;
	void ClientRespawn(struct edict_s* entity) override;
	void ClientHurt(struct edict_s* victim, struct edict_s* attacker) override;
	void ClientKilled(struct edict_s* victim, struct edict_s* killer) override;
	void Think() override;

	edict_t* GetListenServerClient() const;

	bool TeamPlayEnabled() const;
	bool MapIsRunning() const;
	float CurrentTime() const;
};
