#pragma once

#include "genericweapon.h"
#include "EnginePublicAPI/eiface.h"

namespace WeaponAtts
{
	struct WAMeleeAttack;
}

class CGenericMeleeWeapon : public CGenericWeapon
{
public:
	CGenericMeleeWeapon();

	// Required so that derived classes can call through.
	virtual void Precache() override;

protected:
	virtual void PrecacheAttackMode(const WeaponAtts::WABaseAttack& attackMode) override;
	virtual bool InvokeWithAttackMode(WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode) override;

private:
	void AttackStrike();
	void InitTraceVecs(const WeaponAtts::WAMeleeAttack* meleeAttack);
	bool CheckForContact(const WeaponAtts::WAMeleeAttack* meleeAttack, TraceResult& tr);
	void FireEvent(const WeaponAtts::WAMeleeAttack* meleeAttack);

	const WeaponAtts::WAMeleeAttack* m_pCachedAttack;
	int m_iStrikeIndex;
	Vector m_vecAttackTraceStart;
	Vector m_vecAttackTraceEnd;
	Vector m_vecContactPointOnSurface;
};
