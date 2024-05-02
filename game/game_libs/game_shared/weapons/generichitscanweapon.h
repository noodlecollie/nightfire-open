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
	void PrecacheAttackMode(const WeaponAtts::WABaseAttack& attackMode) override;
	bool InvokeWithAttackMode(
		const CGenericWeapon::WeaponAttackType type,
		const WeaponAtts::WABaseAttack* attackMode) override;
};
