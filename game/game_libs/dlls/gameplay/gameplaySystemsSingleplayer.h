#pragma once

#include "gameplaySystemsBase.h"

class CGameplaySystemsSingleplayer : public CGameplaySystemsBase
{
public:
	virtual ~CGameplaySystemsSingleplayer()
	{
	}

	virtual bool IsMultiplayer() const override;

	virtual void OnServerActivated() override;
	virtual void OnServerDeactivated() override;

	// Nothing else unique to this class yet.
};
