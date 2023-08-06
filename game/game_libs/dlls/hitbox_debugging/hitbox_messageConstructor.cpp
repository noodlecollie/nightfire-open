#include "hitbox_messageConstructor.h"
#include "standard_includes.h"
#include "weapondebugevents/weapondebugevent_hitscanfire.h"
#include "customGeometry/messageWriter.h"
#include "geometryConstructors/hitboxGeometryConstructor.h"

namespace
{
	static constexpr uint32_t COL_MISS = 0xFFFFFFFF;
	static constexpr uint32_t COL_HIT = 0xFF0000FF;

	static bool HitGroupWasHit(const CWeaponDebugEvent_HitscanFire& event, uint32_t hitGroup)
	{
		for ( uint32_t traceIndex = 0; traceIndex < event.TraceCount(); ++traceIndex )
		{
			if ( static_cast<uint32_t>(event.GetTrace(traceIndex)->traceResult.iHitgroup) == hitGroup )
			{
				return true;
			}
		}

		return false;
	}

	static inline bool
	HitboxWasHit(const CWeaponDebugEvent_HitscanFire& event, const edict_t* edict, uint32_t hitboxIndex)
	{
		int hitGroup = g_engfuncs.pfnGetHitboxHitGroup(edict, hitboxIndex);
		return hitGroup >= 0 && HitGroupWasHit(event, hitGroup);
	}
}  // namespace

CHitboxMessageConstructor::CHitboxMessageConstructor(CBasePlayer* attacker, CBaseAnimating* target) :
	m_Attacker(attacker),
	m_Target(target)
{
}

void CHitboxMessageConstructor::FireMessages(const CWeaponDebugEvent_HitscanFire& event)
{
	if ( !m_Attacker )
	{
		return;
	}

	FireClearMessage();

	m_HitboxCount = m_Target ? g_engfuncs.pfnGetHitboxCount(m_Target->edict()) : 0;

	SendHitscanGeometry(event);
	SendHitboxGeometry(event);
}

void CHitboxMessageConstructor::SendHitscanGeometry(const CWeaponDebugEvent_HitscanFire& event)
{
	CustomGeometry::GeometryItemPtr_t missTraceLines(new CustomGeometry::CGeometryItem());
	missTraceLines->SetDrawType(CustomGeometry::DrawType::Lines);
	missTraceLines->SetColour(COL_MISS);

	CustomGeometry::GeometryItemPtr_t hitTraceLines(new CustomGeometry::CGeometryItem());
	hitTraceLines->SetDrawType(CustomGeometry::DrawType::Lines);
	hitTraceLines->SetColour(COL_HIT);

	for ( uint32_t traceIndex = 0; traceIndex < event.TraceCount(); ++traceIndex )
	{
		const CWeaponDebugEvent_HitscanFire::Trace* trace = event.GetTrace(traceIndex);

		if ( trace->traceResult.flFraction < 1.0f && trace->traceResult.pHit &&
			 GetClassPtrFromEdict<CBasePlayer>(trace->traceResult.pHit) &&
			 static_cast<uint32_t>(trace->traceResult.iHitgroup) < m_HitboxCount )
		{
			hitTraceLines->AddLine(trace->begin, trace->traceResult.vecEndPos);
		}
		else
		{
			missTraceLines->AddLine(trace->begin, trace->traceResult.vecEndPos);
		}
	}

	CustomGeometry::CMessageWriter writer(CustomGeometry::Category::HitboxDebugging);
	writer.SetTargetClient(m_Attacker);
	writer.WriteMessage(*missTraceLines);
	writer.WriteMessage(*hitTraceLines);
}

void CHitboxMessageConstructor::SendHitboxGeometry(const CWeaponDebugEvent_HitscanFire& event)
{
	if ( !m_Target )
	{
		return;
	}

	for ( uint32_t hitboxIndex = 0; hitboxIndex < m_HitboxCount; ++hitboxIndex )
	{
		CHitboxGeometryConstructor constructor;

		constructor.SetEntity(m_Target);
		constructor.SetHitboxIndex(hitboxIndex);

		CustomGeometry::GeometryItemPtr_t hitboxGeometry(new CustomGeometry::CGeometryItem());
		hitboxGeometry->SetColour(HitboxWasHit(event, m_Target->edict(), hitboxIndex) ? COL_HIT : COL_MISS);
		constructor.AddGeometry(hitboxGeometry);

		CustomGeometry::CMessageWriter writer(CustomGeometry::Category::HitboxDebugging);
		writer.SetTargetClient(m_Attacker);
		writer.WriteMessage(*hitboxGeometry);
	}
}

void CHitboxMessageConstructor::FireClearMessage()
{
	if ( !m_Attacker )
	{
		return;
	}

	CustomGeometry::CMessageWriter writer(CustomGeometry::Category::HitboxDebugging);
	writer.SetTargetClient(m_Attacker);
	writer.WriteClearMessage();
}
