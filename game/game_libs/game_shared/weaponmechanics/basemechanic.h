#pragma once

namespace WeaponAtts
{
	struct WABaseAttack;
	struct WASoundSet;
}

namespace WeaponMechanics
{
	class CBaseMechanic
	{
	public:
		explicit CBaseMechanic(const WeaponAtts::WABaseAttack* attackMode);
		virtual ~CBaseMechanic() = default;

		virtual void Precache();

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
