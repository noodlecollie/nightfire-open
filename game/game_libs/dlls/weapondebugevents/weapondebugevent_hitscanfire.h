#pragma once

#include "weapondebugevent_base.h"
#include "standard_includes.h"
#include "utlvector.h"

class CWeaponDebugEvent_HitscanFire : public CWeaponDebugEvent_Base
{
public:
	struct Trace
	{
		Vector begin;
		TraceResult traceResult;
	};

	CWeaponDebugEvent_HitscanFire(CGenericWeapon& weapon);
	virtual ~CWeaponDebugEvent_HitscanFire()
	{
	}

	virtual CWeaponDebugEvent_Base::EventType Type() const override;

	void AddTrace(const Vector& begin, const TraceResult& tr);
	const Trace* GetTrace(uint32_t index) const;
	size_t TraceCount() const;

private:
	typedef CUtlVector<Trace> TraceList;

	TraceList m_TraceList;
};
