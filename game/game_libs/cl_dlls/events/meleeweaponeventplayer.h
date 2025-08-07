#pragma once

#include "baseweaponeventplayer.h"
#include "weaponatts_meleeattack.h"

class MeleeWeaponEventPlayer : public BaseWeaponEventPlayer
{
public:
	virtual ~MeleeWeaponEventPlayer()
	{
	}

protected:
	virtual void EventStart() override;
};
