#include <memory>
#include "gameplaySystems.h"
#include "gameplaySystemsBase.h"
#include "gameplaySystemsSingleplayer.h"
#include "gameplaySystemsMultiplayer.h"

namespace GameplaySystems
{
	static std::unique_ptr<CGameplaySystemsBase> StaticInstance;

	void Create(bool isMultiplayer)
	{
		if ( isMultiplayer )
		{
			StaticInstance.reset(new CGameplaySystemsMultiplayer());
		}
		else
		{
			StaticInstance.reset(new CGameplaySystemsSingleplayer());
		}
	}

	void Destroy()
	{
		StaticInstance.reset();
	}

	bool Exists()
	{
		return StaticInstance != nullptr;
	}

	bool IsMultiplayer()
	{
		return StaticInstance ? StaticInstance->IsMultiplayer() : false;
	}

	void NotifyServerActivated()
	{
		if ( StaticInstance )
		{
			StaticInstance->OnServerActivated();
		}
	}

	void NotifyServerDeactivated()
	{
		if ( StaticInstance )
		{
			StaticInstance->OnServerDeactivated();
		}
	}

	CGameplaySystemsBase* GetBase()
	{
		return StaticInstance.get();
	}

	CGameplaySystemsSingleplayer* GetSingleplayer()
	{
		return dynamic_cast<CGameplaySystemsSingleplayer*>(StaticInstance.get());
	}

	CGameplaySystemsMultiplayer* GetMultiplayer()
	{
		return dynamic_cast<CGameplaySystemsMultiplayer*>(StaticInstance.get());
	}
}  // namespace GameplaySystems
