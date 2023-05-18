#pragma once

#include <memory>
#include "spawnpointmanager.h"
#include "hitbox_debugging/hitbox_debugData.h"

class CGameplaySystemsBase
{
public:
	CGameplaySystemsBase();
	virtual ~CGameplaySystemsBase()
	{
	}

	virtual bool IsMultiplayer() const = 0;

	virtual void OnServerActivated();
	virtual void OnServerDeactivated();

	CSpawnPointManager& SpawnPointManager();
	CHitboxDebugData& HitboxDebugData();

private:
	std::unique_ptr<CSpawnPointManager> m_SpawnPointManager;
	std::unique_ptr<CHitboxDebugData> m_HitboxDebugData;
};
