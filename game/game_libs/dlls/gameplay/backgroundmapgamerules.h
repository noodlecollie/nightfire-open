#pragma once

#include "standard_includes.h"
#include "gamerules.h"

class CBackgroundMapGameRules : public CHalfLifeRules
{
public:
	void PlayerSpawn(CBasePlayer* pPlayer) override;
};
