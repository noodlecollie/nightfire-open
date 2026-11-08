#include "gameplaySystemsBase.h"
#include "spawnpointmanager.h"
#include "hitbox_debugging/hitbox_debugData.h"
#include "gameplay/inaccuracymodifiers.h"

CGameplaySystemsBase::CGameplaySystemsBase() :
	m_SpawnPointManager(new CSpawnPointManager()),
	m_ObserverPointList(new CObserverPointList()),
	m_HitboxDebugData(new CHitboxDebugData()),
	m_EventSystem(new Events::CEventSystem())
{
}

void CGameplaySystemsBase::OnServerActivated()
{
	InaccuracyModifiers::Init();
	m_SpawnPointManager->Initialise();
	m_ObserverPointList->Initialise();
}

void CGameplaySystemsBase::OnServerDeactivated()
{
	m_EventSystem->UnregisterAllCallbacks();
	InaccuracyModifiers::Deinit();
}

CSpawnPointManager& CGameplaySystemsBase::SpawnPointManager()
{
	return *m_SpawnPointManager;
}

CObserverPointList& CGameplaySystemsBase::ObserverPointList()
{
	return *m_ObserverPointList;
}

CHitboxDebugData& CGameplaySystemsBase::HitboxDebugData()
{
	return *m_HitboxDebugData;
}

Events::CEventSystem& CGameplaySystemsBase::EventSystem()
{
	return *m_EventSystem;
}
