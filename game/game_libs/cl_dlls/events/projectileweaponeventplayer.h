#pragma once

#include "baseweaponeventplayer.h"
#include "weaponatts_projectileattack.h"

class ProjectileWeaponEventPlayer : public BaseWeaponEventPlayer
{
public:
	virtual ~ProjectileWeaponEventPlayer()
	{
	}

protected:
	virtual bool Initialise() override;
	virtual void EventStart() override;

private:
	const WeaponAtts::WAProjectileAttack* m_pProjectileAttack = nullptr;
	int m_iShellModelIndex = -1;
};
