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

	void Precache() override;
	void Holster(int skiplocal = 0) override;

protected:
	virtual void PrecacheAttackMode(const WeaponAtts::WABaseAttack& attack) override;
	virtual bool InvokeWithAttackMode(WeaponAtts::AttackMode mode, const WeaponAtts::WABaseAttack* attack) override;

private:
	void AttackStrike();
	void InitTraceVecs(const WeaponAtts::WAMeleeAttack* meleeAttack);
	void FireEvent(const WeaponAtts::WAMeleeAttack* meleeAttack);

#ifndef CLIENT_DLL
	void AttackStrike_Server();
	bool CheckForContact(const WeaponAtts::WAMeleeAttack* meleeAttack, TraceResult& tr);
#endif

	const WeaponAtts::WAMeleeAttack* m_pCachedAttack;
	int m_iStrikeIndex;
	Vector m_vecAttackTraceStart;
	Vector m_vecAttackTraceEnd;
	Vector m_vecContactPointOnSurface;
};
