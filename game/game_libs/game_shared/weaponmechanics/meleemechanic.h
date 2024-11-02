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
		explicit CMeleeMechanic(CGenericWeapon* weapon, const WeaponAtts::WAMeleeAttack* attackMode);

		void Precache() override;
		InvocationResult Invoke() override;
		void Reset() override;

	private:
		const WeaponAtts::WAMeleeAttack* MeleeAttackMode() const
		{
			return GetAttackMode<WeaponAtts::WAMeleeAttack>();
		}

		InvocationResult AttackStrike();
		void InitTraceVecs();
		void FireEvent();

#ifndef CLIENT_DLL
		static void
		FindHullIntersection(const Vector& vecSrc, TraceResult& tr, float* mins, float* maxs, edict_t* pEntity);

		void AttackStrike_Server();
		bool CheckForContact(TraceResult& tr);
#endif

		int m_iStrikeIndex = -1;
		Vector m_vecAttackTraceStart;
		Vector m_vecAttackTraceEnd;
		Vector m_vecContactPointOnSurface;
	};
}  // namespace WeaponMechanics
