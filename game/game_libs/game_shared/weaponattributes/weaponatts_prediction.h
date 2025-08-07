#pragma once

#include <memory>
#include <functional>
#include "weaponatts_base.h"

class CBasePlayerWeapon;

namespace WeaponAtts
{
	// In order to use operator new() on an entity, you have to pass in entvars and all
	// sorts of stuff so that the engine can set up the object. You also can't create
	// entities if you're not on the server.
	// However, the client needs (shells of) weapon objects in order to perform
	// client-side prediction, and for some reason allocating weapons in static memory
	// works fine (???), so that's how Half Life did it. It means that you have to do
	// a lot of boilerplate code, though, and I don't have time for that shiz.
	// Instead, we have this slight hack-around: a wrapper class holds a weapon instance
	// that is not allocated using operator new(), and subclasses a common interface.
	// This can then be instanciated dynamically as part of the weapon attributes.

	class WABasePredictionWeaponHelper
	{
	public:
		virtual ~WABasePredictionWeaponHelper()
		{
		}

		virtual CBasePlayerWeapon* GetWeapon() const = 0;
	};

	template<typename T>
	class WAPredictionWeaponHelper : public WABasePredictionWeaponHelper
	{
	public:
		virtual ~WAPredictionWeaponHelper()
		{
		}

		virtual CBasePlayerWeapon* GetWeapon() const override
		{
			return static_cast<CBasePlayerWeapon*>(&m_Weapon);
		}

	private:
		mutable T m_Weapon;
	};

	class WAPrediction : public WABase
	{
	public:
		// Call this when setting up weapon attributes in order to make the weapon predictable.
		// For example: SetUpPrediction<CWeaponP99>().
		template<typename T>
		inline void SetUpPrediction()
		{
			// Called later, when weapon prediction is set up.
			m_FactoryFunc = []()
			{
				return new WAPredictionWeaponHelper<T>();
			};
		}

		inline CBasePlayerWeapon* GetWeapon() const
		{
			if ( !m_FactoryFunc )
			{
				return nullptr;
			}

			if ( !m_WeaponHelper )
			{
				m_WeaponHelper.reset(m_FactoryFunc());
			}

			return m_WeaponHelper ? m_WeaponHelper->GetWeapon() : nullptr;
		}

		virtual void Validate() const override
		{
			// Force std::function to bool.
			ASSERTSZ_Q(!!m_FactoryFunc, "Weapon must have prediction set up.");
		}

	private:
		using FactoryFunc = std::function<WABasePredictionWeaponHelper*(void)>;

		FactoryFunc m_FactoryFunc;
		mutable std::unique_ptr<WABasePredictionWeaponHelper> m_WeaponHelper;
	};
}  // namespace WeaponAtts
