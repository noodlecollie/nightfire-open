#pragma once

#include <memory>
#include "genericweapon.h"
#include "weaponatts_hitscanattack.h"

#ifndef CLIENT_DLL
class CWeaponDebugEvent_HitscanFire;
#endif

class CGenericHitscanWeapon : public CGenericWeapon
{
public:
	virtual void WeaponIdle() override;

protected:
	void PrecacheAttackMode(const WeaponAtts::WABaseAttack& attack) override;
	bool InvokeWithAttackMode(WeaponAtts::AttackMode mode, const WeaponAtts::WABaseAttack* attack) override;
};
