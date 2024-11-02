#pragma once

#include <limits>

namespace WeaponAtts
{
	struct WABaseAttack;
	struct WASoundSet;
}

namespace WeaponMechanics
{
	static constexpr float THINK_CANCELLED = std::numeric_limits<float>::min();

	class CBaseMechanic
	{
	public:

		explicit CBaseMechanic(const WeaponAtts::WABaseAttack& attackMode);
		virtual ~CBaseMechanic() = default;

		virtual void Precache();

		// Returns the time point for the next think, or THINK_CANCELLED
		// if no new think should take place.
		virtual float Think();

		int EventIndex() const;

	protected:
		// Assumes that the subclass knows what the original attack mode
		// type actually was.
		template<typename T>
		const T* GetAttackMode() const
		{
			return static_cast<const T*>(m_BaseAttackMode);
		}

		void PrecacheSoundSet(const WeaponAtts::WASoundSet& sounds);

	private:
		const WeaponAtts::WABaseAttack* m_BaseAttackMode = nullptr;
		int m_EventIndex = -1;
	};
}
