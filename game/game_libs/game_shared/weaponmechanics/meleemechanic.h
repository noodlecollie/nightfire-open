#pragma once

#include "weaponmechanics/basemechanic.h"

namespace WeaponAtts
{
	struct WAMeleeAttack;
};

namespace WeaponMechanics
{
	class CMeleeMechanic : public CBaseMechanic
	{
	public:
		CMeleeMechanic(CGenericWeapon* weapon, const WeaponAtts::WAMeleeAttack* attackMode);
		CMeleeMechanic(CGenericWeapon* weapon, uint32_t attackIndex);

		void Precache() override;
		InvocationResult Invoke(uint32_t step) override;

	private:
		const WeaponAtts::WAMeleeAttack* MeleeAttackMode() const;
		void InitTraceVecs();
		void FireEvent();
		float GetDelayFromPreviousStrike(uint32_t index) const;

#ifndef CLIENT_DLL
		static void
		FindHullIntersection(const Vector& vecSrc, TraceResult& tr, float* mins, float* maxs, edict_t* pEntity);

		void AttackStrike_Server();
		bool CheckForContact(TraceResult& tr);
#endif

		Vector m_vecAttackTraceStart;
		Vector m_vecAttackTraceEnd;
		Vector m_vecContactPointOnSurface;
	};
}  // namespace WeaponMechanics
