#include "gameplay/eventSystem.h"

namespace Events
{
	size_t CEventSystem::RegisterEventCallback(EventType eventType, const Callback& callback)
	{
		ASSERT(callback.operator bool());

		if ( !callback )
		{
			return 0;
		}

		const size_t id = m_NextID++;

		if ( m_NextID == 0 )
		{
			++m_NextID;
		}

		const int eventTypeIndex = static_cast<int>(eventType);
		ASSERT(eventTypeIndex >= 0);

		if ( eventTypeIndex >= m_Registrations.Count() )
		{
			m_Registrations.SetCount(eventTypeIndex + 1);
		}

		Registration reg;
		reg.id = id;
		reg.callback = callback;

		RegistrationVector& regVec = m_Registrations[eventTypeIndex];
		regVec.AddToTail(std::move(reg));

		return id;
	}

	void CEventSystem::UnregisterEventCallback(EventType eventType, size_t id)
	{
		ASSERT(id != 0);

		if ( id == 0 )
		{
			return;
		}

		RegistrationVector* regVec = GetRegistrationsForEventType(eventType);
		ASSERT(regVec);

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

	void CEventSystem::SendEvent(const CEvent& event) const
	{
		ASSERT(event.GetBaseEventData()->IsValid());

		if ( !event.GetBaseEventData()->IsValid() )
		{
			return;
		}

		const RegistrationVector* registrations = GetRegistrationsForEventType(event.GetType());

		if ( !registrations || registrations->IsEmpty() )
		{
			return;
		}

		FOR_EACH_VEC(*registrations, index)
		{
			(*registrations)[index].callback(event);
		}
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
