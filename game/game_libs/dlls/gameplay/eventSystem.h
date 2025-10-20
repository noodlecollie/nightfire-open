#pragma once

#include "standard_includes.h"
#include "utlvector.h"
#include <type_traits>
#include <memory>
#include <functional>

namespace Events
{
	enum class EventType
	{
		PlayerPickedUpHealth,
		PlayerPickedUpArmour,
		PlayerPickedUpWeapon,
		PlayerPickedUpAmmo
	};

	struct BaseEventData
	{
		virtual ~BaseEventData() = default;
		virtual bool IsValid() const = 0;
	};

	struct BasePickupEventData : public BaseEventData
	{
		edict_t* player = nullptr;
		edict_t* item = nullptr;

		bool IsValid() const override
		{
			return player && item;
		}
	};

	struct EventData_PlayerPickedUpHealth : public BasePickupEventData
	{
		static constexpr EventType EVENT_TYPE = EventType::PlayerPickedUpHealth;
		float healthAdded = 0.0f;
	};

	struct EventData_PlayerPickedUpArmour : public BasePickupEventData
	{
		static constexpr EventType EVENT_TYPE = EventType::PlayerPickedUpArmour;
		float armourAdded = 0.0f;
	};

	struct EventData_PlayerPickedUpWeapon : public BasePickupEventData
	{
		static constexpr EventType EVENT_TYPE = EventType::PlayerPickedUpWeapon;
	};

	struct EventData_PlayerPickedUpAmmo : public BasePickupEventData
	{
		static constexpr EventType EVENT_TYPE = EventType::PlayerPickedUpAmmo;
		int ammoAdded = 0;
	};

	class CEvent
	{
	public:
		template<typename T>
		explicit CEvent(T&& impl) :
			m_EventType(T::EVENT_TYPE),
			m_Data(new T(std::move(impl)))
		{
			static_assert(std::is_base_of<BaseEventData, T>::value, "Data type must inherit from BaseEventData");
		}

		EventType GetType() const
		{
			return m_EventType;
		}

		const BaseEventData* GetBaseEventData() const
		{
			return m_Data.get();
		}

		template<typename T>
		typename std::enable_if<std::is_base_of<BaseEventData, T>::value, const T*>::type GetData() const
		{
			if ( T::EVENT_TYPE != GetType() )
			{
				ASSERT(false);
				return nullptr;
			}

			return static_cast<const T*>(m_Data.get());
		}

	private:
		struct Impl;

		EventType m_EventType;
		std::unique_ptr<BaseEventData> m_Data;
	};

	class CEventSystem
	{
	public:
		using Callback = std::function<void(const CEvent&)>;

		static constexpr size_t INVALID_ID = 0;

		~CEventSystem();

		size_t RegisterEventCallback(EventType eventType, const Callback& callback);
		size_t RegisterEventCallback(EventType eventType, CBaseEntity* subscriber, const Callback& callback);
		void UnregisterEventCallback(EventType eventType, size_t id);
		void UnregisterAllCallbacks();

		void SendEvent(const CEvent& event);

		template<typename T>
		typename std::enable_if<std::is_base_of<BaseEventData, T>::value, void>::type SendEvent(T&& data)
		{
			SendEvent(CEvent(std::move(data)));
		}

	private:
		struct Registration
		{
			size_t id = 0;
			Callback callback;
			EHANDLE subscriber;
			bool has_subscriber = false;
		};

		// MSVC crashes occur in std::function if this vector stores
		// objects instead of pointers. These vectors probably do funky
		// stuff with memory that they shouldn't...
		using RegistrationVector = CUtlVector<Registration*>;

		size_t AddRegistration(EventType type, Registration* reg);
		RegistrationVector* GetRegistrationsForEventType(EventType type);
		const RegistrationVector* GetRegistrationsForEventType(EventType type) const;

		CUtlVector<RegistrationVector> m_Registrations;
		size_t m_NextID = 1;
		bool m_ProcessingEvent = false;
	};
}  // namespace Events
