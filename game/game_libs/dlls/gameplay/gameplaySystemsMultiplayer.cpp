#include "gameplaySystemsMultiplayer.h"

bool CGameplaySystemsMultiplayer::IsMultiplayer() const
{
	return true;
}

void CGameplaySystemsMultiplayer::OnServerActivated()
{
	CGameplaySystemsBase::OnServerActivated();
}

void CGameplaySystemsMultiplayer::OnServerDeactivated()
{
	CGameplaySystemsBase::OnServerDeactivated();
}
