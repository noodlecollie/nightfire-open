#pragma once

#include <cstdint>
#include "mathlib.h"
#include "const.h"
#include "botprofiles/botprofiletable.h"
#include "utlstring.h"
#include "botfactory.h"

class CBasePlayer;
class CGameRules;

class CBotGameRulesInterface
{
public:
	CBotGameRulesInterface(CGameRules& parent);

	void ServerActivate();
	void ServerDeactivate();
	void ClientDisconnect(edict_t* entity);
	void ClientPutInServer(edict_t* entity);
	bool ClientCommand(CBasePlayer* player, const char* command);
	void Think();

	CBotFactory& BotFactory();
	const CBotFactory& BotFactory() const;

private:
	void SpawnBotsInRegister();

	CGameRules* m_pParent;
	CBotFactory m_BotFactory;
	bool m_bSpawnedRegisterBots;
};
