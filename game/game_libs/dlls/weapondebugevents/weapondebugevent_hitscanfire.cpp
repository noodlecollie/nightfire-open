#include "weapondebugevent_hitscanfire.h"

CWeaponDebugEvent_HitscanFire::CWeaponDebugEvent_HitscanFire(CGenericWeapon& weapon) :
	CWeaponDebugEvent_Base(weapon),
	m_TraceList()
{
}

CWeaponDebugEvent_Base::EventType CWeaponDebugEvent_HitscanFire::Type() const
{
	return CWeaponDebugEvent_Base::EventType::Event_HitscanFire;
}

void CWeaponDebugEvent_HitscanFire::AddTrace(const Vector& begin, const TraceResult& tr)
{
	m_TraceList.AddToTail({begin, tr});
}

const CWeaponDebugEvent_HitscanFire::Trace* CWeaponDebugEvent_HitscanFire::GetTrace(uint32_t index) const
{
	if ( index >= static_cast<uint32_t>(m_TraceList.Count()) )
	{
		return nullptr;
	}

	return &m_TraceList[index];
}

size_t CWeaponDebugEvent_HitscanFire::TraceCount() const
{
	return m_TraceList.Count();
}
