#include "weapondebugeventsource.h"
#include "weapondebugevent_hitscanfire.h"

bool CWeaponDebugEventSource::RegisterCallback(
	const CUtlString& id,
	CWeaponDebugEvent_Base::EventType eventType,
	const EventCallback& callback)
{
	if ( m_CallbackPresent.Find(id) != m_CallbackPresent.InvalidIndex() )
	{
		// Callback with this ID already exists.
		return false;
	}

	m_CallbackPresent.Insert(id, eventType);
	int index = m_CallbacksByType.Find(eventType);

	if ( index == m_CallbacksByType.InvalidIndex() )
	{
		// Create a blank list.
		index = m_CallbacksByType.Insert(eventType, EventCallbackListPtr(new EventCallbackList()));
	}

	EventCallbackList& cbList = *m_CallbacksByType[index];
	cbList.AddToTail(EventCallbackListEntry(id, callback));
	return true;
}

void CWeaponDebugEventSource::UnregisterCallback(const CUtlString& id)
{
	int presentIndex = m_CallbackPresent.Find(id);

	if ( presentIndex == m_CallbackPresent.InvalidIndex() )
	{
		// No callback exists with this ID.
		return;
	}

	EventType eventType = m_CallbackPresent[presentIndex];
	m_CallbackPresent.Remove(id);

	int callbackIndex = m_CallbacksByType.Find(eventType);

	if ( callbackIndex == m_CallbacksByType.InvalidIndex() )
	{
		// Should never happen, but here for safety.
		return;
	}

	EventCallbackList& list = *m_CallbacksByType[callbackIndex];
	int itemToRemove = -1;

	FOR_EACH_VEC(list, index)
	{
		if ( list[index].first == id )
		{
			itemToRemove = index;
			break;
		}
	}

	if ( itemToRemove >= 0 )
	{
		list.Remove(itemToRemove);
	}
}

bool CWeaponDebugEventSource::EventHasSubscribers(CWeaponDebugEvent_Base::EventType eventType) const
{
	return m_CallbacksByType.Find(eventType) != m_CallbacksByType.InvalidIndex();
}

void CWeaponDebugEventSource::FireEvent(const CWeaponDebugEvent_Base* event)
{
	if ( !event )
	{
		return;
	}

	int cbIndex = m_CallbacksByType.Find(event->Type());

	if ( cbIndex == m_CallbacksByType.InvalidIndex() )
	{
		return;
	}

	EventCallbackList& list = *m_CallbacksByType[cbIndex];

	FOR_EACH_VEC(list, index)
	{
		list[index].second(event);
	}
}
