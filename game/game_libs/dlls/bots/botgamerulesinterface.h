#pragma once

#include <cstdint>
#include "EnginePublicAPI/const.h"
#include "botprofiles/botprofiletable.h"
#include "utlstring.h"
#include "botfactory.h"
#include "EnginePublicAPI/edict.h"
#include "bots/ibotgamerulesinterface.h"

class CGameRules;

class CBotGameRulesInterface : public IBotGameRulesInterface
{
public:
	CBotGameRulesInterface(CGameRules& parent);
	~CBotGameRulesInterface() = default;

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

	CBotFactory& BotFactory();
	const CBotFactory& BotFactory() const;

private:
	void SpawnBotsInRegister();

	CGameRules* m_pParent = nullptr;
	CBotFactory m_BotFactory;
	bool m_bSpawnedRegisterBots = false;
};
