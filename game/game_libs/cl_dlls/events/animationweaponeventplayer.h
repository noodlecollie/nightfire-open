#pragma once

#include "baseweaponeventplayer.h"

class AnimationWeaponEventPlayer : public BaseWeaponEventPlayer
{
public:
	virtual ~AnimationWeaponEventPlayer() = default;

protected:
	virtual void EventStart() override;
};
