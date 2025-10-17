#include "gameplay/eventSystem.h"

namespace Events
{
	size_t CEventSystem::RegisterEventCallback(EventType eventType, const Callback& callback)
	{
		ASSERT(callback.operator bool());

		if ( !callback )
		{
			return INVALID_ID;
		}

		Registration reg;
		reg.callback = callback;

		return AddRegistration(eventType, std::move(reg));
	}

	size_t CEventSystem::RegisterEventCallback(EventType eventType, CBaseEntity* subscriber, const Callback& callback)
	{
		ASSERT(subscriber);

		if ( !subscriber )
		{
			return INVALID_ID;
		}

		EHANDLE subscriberHandle;
		subscriberHandle = subscriber;

		Registration reg;
		reg.subscriber = subscriberHandle;
		reg.has_subscriber = true;
		reg.callback = [subscriberHandle, callback](const CEvent& event)
		{
			if ( subscriberHandle )
			{
				callback(event);
			}
		};

		return AddRegistration(eventType, std::move(reg));
	}

	void CEventSystem::UnregisterEventCallback(EventType eventType, size_t id)
	{
		ASSERT(id != INVALID_ID);

		if ( id == INVALID_ID )
		{
			return;
		}

		RegistrationVector* regVec = GetRegistrationsForEventType(eventType);
		ASSERTSZ(regVec, "No event registrations present for the given type");

		if ( !regVec )
		{
			return;
		}

		FOR_EACH_VEC(*regVec, index)
		{
			if ( (*regVec)[index].id == id )
			{
				regVec->Remove(index);
				return;
			}
		}

		ASSERTSZ(false, "No event callback found with given ID.");
	}

	void CEventSystem::UnregisterAllCallbacks()
	{
		m_Registrations.Purge();
	}

	void CEventSystem::SendEvent(const CEvent& event)
	{
		ASSERT(!m_ProcessingEvent);
		ASSERT(event.GetBaseEventData()->IsValid());

		if ( !event.GetBaseEventData()->IsValid() )
		{
			return;
		}

		RegistrationVector* registrations = GetRegistrationsForEventType(event.GetType());

		if ( !registrations || registrations->IsEmpty() )
		{
			return;
		}

		m_ProcessingEvent = true;
		bool needsDeletions = false;

		FOR_EACH_VEC(*registrations, index)
		{
			Registration& reg = (*registrations)[index];

			if ( reg.has_subscriber && !reg.subscriber )
			{
				needsDeletions = true;
				continue;
			}

			(*registrations)[index].callback(event);
		}

		if ( needsDeletions )
		{
			FOR_EACH_VEC_BACK(*registrations, index)
			{
				Registration& reg = (*registrations)[index];

				if ( reg.has_subscriber && !reg.subscriber )
				{
					registrations->Remove(index);
				}
			}
		}

		m_ProcessingEvent = false;
	}

	size_t CEventSystem::AddRegistration(EventType type, Registration&& reg)
	{
		const size_t id = m_NextID++;

		if ( m_NextID == INVALID_ID )
		{
			++m_NextID;
		}

		const int eventTypeIndex = static_cast<int>(type);
		ASSERT(eventTypeIndex >= 0);

		if ( eventTypeIndex >= m_Registrations.Count() )
		{
			m_Registrations.SetCount(eventTypeIndex + 1);
		}

		reg.id = id;

		RegistrationVector& regVec = m_Registrations[eventTypeIndex];
		regVec.AddToTail(std::move(reg));

		return id;
	}

	CEventSystem::RegistrationVector* CEventSystem::GetRegistrationsForEventType(EventType type)
	{
		const int eventTypeIndex = static_cast<int>(type);
		return (eventTypeIndex >= 0 && eventTypeIndex < m_Registrations.Count()) ? &m_Registrations[eventTypeIndex]
																				 : nullptr;
	}

	const CEventSystem::RegistrationVector* CEventSystem::GetRegistrationsForEventType(EventType type) const
	{
		const int eventTypeIndex = static_cast<int>(type);
		return (eventTypeIndex >= 0 && eventTypeIndex < m_Registrations.Count()) ? &m_Registrations[eventTypeIndex]
																				 : nullptr;
	}
}  // namespace Events
