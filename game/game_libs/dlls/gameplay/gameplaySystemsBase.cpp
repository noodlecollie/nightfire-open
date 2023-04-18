#include "gameplaySystemsBase.h"
#include "spawnpointmanager.h"
#include "hitbox_debugging/hitbox_debugData.h"

CGameplaySystemsBase::CGameplaySystemsBase() :
	m_SpawnPointManager(new CSpawnPointManager()),
	m_HitboxDebugData(new CHitboxDebugData())
{
}

void CGameplaySystemsBase::OnServerActivated()
{
	m_SpawnPointManager->Initialise();
}

void CGameplaySystemsBase::OnServerDeactivated()
{
}

CSpawnPointManager& CGameplaySystemsBase::SpawnPointManager()
{
	return *m_SpawnPointManager;
}

CHitboxDebugData& CGameplaySystemsBase::HitboxDebugData()
{
	return *m_HitboxDebugData;
}
