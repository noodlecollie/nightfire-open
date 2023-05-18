#pragma once

#include <memory>

class CGameplaySystemsBase;
class CGameplaySystemsSingleplayer;
class CGameplaySystemsMultiplayer;

namespace GameplaySystems
{
	void Create(bool isMultiplayer);
	void Destroy();
	bool Exists();
	bool IsMultiplayer();

	void NotifyServerActivated();
	void NotifyServerDeactivated();

	CGameplaySystemsBase* GetBase();
	CGameplaySystemsSingleplayer* GetSingleplayer();
	CGameplaySystemsMultiplayer* GetMultiplayer();
}  // namespace GameplaySystems
