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
	virtual ~CGenericHitscanWeapon();
	virtual void WeaponIdle() override;

protected:
	virtual void PrecacheAttackMode(const WeaponAtts::WABaseAttack& attackMode) override;
	virtual bool InvokeWithAttackMode(const CGenericWeapon::WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode) override;

private:
	Vector FireBulletsPlayer(const WeaponAtts::WAHitscanAttack& hitscanAttack,
							 const Vector& vecSrc,
							 const Vector& vecDirShooting);

// SERVER
#ifndef CLIENT_DLL
	void Debug_HitscanBulletFired(const Vector& start, const TraceResult& tr);
	void Debug_FinaliseHitscanEvent();
	void Debug_DeleteHitscanEvent();

	// Can't use a unique_ptr for this because of the forward declaration.
	CWeaponDebugEvent_HitscanFire* m_pHitscanFireEvent = nullptr;
#endif

// CLIENT
#ifdef CLIENT_DLL
	Vector FireBulletsPlayer_Client(const WeaponAtts::WAHitscanAttack& hitscanAttack);
#endif
};
