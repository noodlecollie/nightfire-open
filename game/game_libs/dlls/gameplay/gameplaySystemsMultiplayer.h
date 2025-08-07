#pragma once

#include "gameplaySystemsBase.h"

class CGameplaySystemsMultiplayer : public CGameplaySystemsBase
{
public:
	virtual ~CGameplaySystemsMultiplayer()
	{
	}

	virtual bool IsMultiplayer() const override;

	virtual void OnServerActivated() override;
	virtual void OnServerDeactivated() override;

	// Nothing else unique to this class yet.
};
