#pragma once

#include <limits>
#include "weaponattributes/weaponatts_ammobasedattack.h"

class CGenericWeapon;

namespace WeaponAtts
{
	struct WABaseAttack;
	struct WASoundSet;
	struct AccuracyParameters;
}  // namespace WeaponAtts

namespace WeaponMechanics
{
	class CBaseMechanic;

	struct InvocationResult
	{
		enum Result
		{
			// The invocation completed successfully.
			COMPLETE = -1,

			// The invocation was started, but needs to be continued
			// later, eg. on a regular think interval. If this result
			// is returned, nextInvocationTime represents the time
			// point for the next time this mechanic should be invoked.
			INCOMPLETE = 0,

			// The invocation could not be actioned, eg. because
			// the weapon was not allowed to fire yet.
			REJECTED_UNKNOWN_REASON = 1,
			REJECTED_CANNOT_ATTACK_UNDERWATER,
			REJECTED_NO_AMMO,
		};

		CBaseMechanic* mechanic;
		Result result;
		float nextInvocationTime;

		InvocationResult(
			CBaseMechanic* inMechanic = nullptr,
			Result inResult = REJECTED_UNKNOWN_REASON,
			float inTime = 0.0f) :
			mechanic(inMechanic),
			result(inResult),
			nextInvocationTime(inTime)
		{
		}

		bool WasRejected() const
		{
			return result > INCOMPLETE;
		}

		bool WasIncomplete() const
		{
			return mechanic && result == INCOMPLETE;
		}

		bool WasComplete() const
		{
			return result == COMPLETE;
		}

		static InvocationResult Rejected(CBaseMechanic& mechanic, Result reason = REJECTED_UNKNOWN_REASON)
		{
			return InvocationResult(
				&mechanic,
				reason > REJECTED_UNKNOWN_REASON ? reason : REJECTED_UNKNOWN_REASON);
		}

		static InvocationResult Incomplete(CBaseMechanic& mechanic, float nextTimeDelta)
		{
			return InvocationResult(&mechanic, INCOMPLETE, UTIL_WeaponTimeBase() + nextTimeDelta);
		}

		static InvocationResult Complete(CBaseMechanic& mechanic)
		{
			return InvocationResult(&mechanic, COMPLETE);
		}
	};

	class CBaseMechanic
	{
	public:
		CBaseMechanic(CGenericWeapon* weapon, const WeaponAtts::WABaseAttack* attackMode);
		virtual ~CBaseMechanic() = default;

		virtual void Precache();
		virtual InvocationResult Invoke(uint32_t step);
		virtual void Reset();
		virtual void WeaponIdle();

		int GetEventIndex() const;
		const WeaponAtts::WABaseAttack* GetAttackMode() const;

		// Convenience, since we need to check for this a lot.
		// Performs a dynamic cast, so may return null.
		const WeaponAtts::WAAmmoBasedAttack* GetAmmoBasedAttackMode() const;

	protected:
		// Assumes that the subclass knows what the original attack mode
		// type actually was.
		template<typename T>
		const T* GetAttackMode() const
		{
			return static_cast<const T*>(m_BaseAttackMode);
		}

		static int DefaultEventFlags();

		CGenericWeapon* GetWeapon() const;
		CBasePlayer* GetPlayer() const;

		// Returns the current attack mode that this mechanic is acting
		// as in the weapon, or WeaponAtts::AttackMode::None if it is
		// neither primary nor secondary.
		WeaponAtts::AttackMode GetAttackModeInWeapon() const;

		void PrecacheSoundSet(const WeaponAtts::WASoundSet& sounds);

		bool HasAmmo(int minCount, bool useClip) const;
		int AmmoLeft(WeaponAtts::WAAmmoBasedAttack::AmmoPool pool) const;
		int AmmoLeft(const WeaponAtts::WABaseAttack* attackMode) const;
		bool IsUnderwaterAndCannotFire() const;

		void PlaySound(const WeaponAtts::WASoundSet& sound, int channel, float volModifier = 1.0f);
		bool DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool pool, int decrement);
		bool DecrementAmmo(const WeaponAtts::WABaseAttack* attackMode);
		void SetNextIdleTime(float secsInFuture, bool allowIfEarlier = false);

		void DelayFiring(
			float secs,
			bool allowIfEarlier = false,
			WeaponAtts::AttackMode attackMode = WeaponAtts::AttackMode::None);

	private:
		CGenericWeapon* m_Weapon = nullptr;
		const WeaponAtts::WABaseAttack* m_BaseAttackMode = nullptr;
		int m_EventIndex = -1;
	};
}  // namespace WeaponMechanics
