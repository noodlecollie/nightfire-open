#pragma once

#include <limits>

class CGenericWeapon;

namespace WeaponAtts
{
	struct WABaseAttack;
	struct WASoundSet;
}

namespace WeaponMechanics
{
	static constexpr float INVOCATION_COMPLETED = std::numeric_limits<float>::min();

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

		Result result = REJECTED_UNKNOWN_REASON;
		float nextInvocationTime = 0.0f;

		bool WasRejected() const
		{
			return result > INCOMPLETE;
		}

		static InvocationResult Rejected(Result reason = REJECTED_UNKNOWN_REASON)
		{
			return { reason > REJECTED_UNKNOWN_REASON ? reason : REJECTED_UNKNOWN_REASON };
		}

		static InvocationResult Incomplete(float nextTime)
		{
			return { INCOMPLETE, nextTime };
		}

		static InvocationResult Complete()
		{
			return { COMPLETE };
		}
	};

	class CBaseMechanic
	{
	public:

		CBaseMechanic(CGenericWeapon* weapon, const WeaponAtts::WABaseAttack* attackMode);
		virtual ~CBaseMechanic() = default;

		virtual void Precache();
		virtual InvocationResult Invoke();
		virtual void Reset();

	protected:
		// Assumes that the subclass knows what the original attack mode
		// type actually was.
		template<typename T>
		const T* GetAttackMode() const
		{
			return static_cast<const T*>(m_BaseAttackMode);
		}

		static int DefaultEventFlags();

		int GetEventIndex() const;
		CGenericWeapon* GetWeapon() const;
		CBasePlayer* GetPlayer() const;

		void PrecacheSoundSet(const WeaponAtts::WASoundSet& sounds);

		bool HasAmmo(int minCount, bool useClip) const;
		bool IsUnderwaterAndCannotFire() const;

		void PlaySound(const WeaponAtts::WASoundSet& sound, int channel, float volModifier = 1.0f);

	private:
		CGenericWeapon* m_Weapon = nullptr;
		const WeaponAtts::WABaseAttack* m_BaseAttackMode = nullptr;
		int m_EventIndex = -1;
	};
}
