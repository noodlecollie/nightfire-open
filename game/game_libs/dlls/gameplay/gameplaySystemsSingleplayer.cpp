#include "gameplaySystemsSingleplayer.h"

bool CGameplaySystemsSingleplayer::IsMultiplayer() const
{
	return false;
}

void CGameplaySystemsSingleplayer::OnServerActivated()
{
	CGameplaySystemsBase::OnServerActivated();
}

void CGameplaySystemsSingleplayer::OnServerDeactivated()
{
	CGameplaySystemsBase::OnServerDeactivated();
}
