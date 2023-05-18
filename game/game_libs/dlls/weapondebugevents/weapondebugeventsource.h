#pragma once

#include <functional>
#include <memory>
#include "standard_includes.h"
#include "utlhashmap.h"
#include "utlstring.h"
#include "utlvector.h"
#include "weapondebugevent_base.h"

class CGenericWeapon;

class CWeaponDebugEventSource
{
public:
	typedef std::function<void(const CWeaponDebugEvent_Base*)> EventCallback;

	bool
	RegisterCallback(const CUtlString& id, CWeaponDebugEvent_Base::EventType eventType, const EventCallback& callback);
	void UnregisterCallback(const CUtlString& id);
	bool EventHasSubscribers(CWeaponDebugEvent_Base::EventType eventType) const;

	void FireEvent(const CWeaponDebugEvent_Base* event);

private:
	typedef CWeaponDebugEvent_Base::EventType EventType;
	typedef std::pair<CUtlString, EventCallback> EventCallbackListEntry;
	typedef CUtlVector<EventCallbackListEntry> EventCallbackList;
	typedef std::shared_ptr<EventCallbackList> EventCallbackListPtr;

	CUtlHashMap<CUtlString, EventType> m_CallbackPresent;
	CUtlHashMap<EventType, EventCallbackListPtr> m_CallbacksByType;
};
