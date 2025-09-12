#include "weapon_hulltrace.h"
#include "weaponregistry.h"
#include "EnginePublicAPI/weaponinfo.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_hulltrace_atts.h"
#include "weaponmechanics/delegatedmechanic.h"
#include "MathLib/utils.h"
#include "customGeometry/constructors/aabboxConstructor.h"

#ifndef CLIENT_DLL
#include "bot.h"
#include "customGeometry/messageWriter.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_hulltrace, CWeaponHullTrace)

CWeaponHullTrace::CWeaponHullTrace() :
	CGenericWeapon()
{
	AddMechanicByAttributeIndex<WeaponAtts::WAEventAttack>(ATTACKMODE_NORMAL, m_PrimaryMechanic);

	m_PrimaryMechanic->SetCallback(
		[this](WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step)
		{
			return DoTrace(mechanic, step);
		}
	);

	SetPrimaryAttackMechanic(m_PrimaryMechanic);
}

const WeaponAtts::WACollection& CWeaponHullTrace::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponHullTrace>();
}

#ifndef CLIENT_DLL
void CWeaponHullTrace::DoTrace_Server()
{
	using namespace CustomGeometry;

	Vector gunPos = m_pPlayer->GetGunPosition();
	Vector aimAngle = Vector(m_pPlayer->pev->v_angle) + Vector(m_pPlayer->pev->punchangle);
	Vector dir;
	AngleVectors(aimAngle, dir, nullptr, nullptr);

	TraceResult tr {};
	g_engfuncs.pfnTraceHull(gunPos, gunPos + (128.0f * dir), ignore_monsters, human_hull, m_pPlayer->edict(), &tr);

	Vector hullMins;
	Vector hullMaxs;
	g_engfuncs.pfnGetHullBounds(human_hull, hullMins, hullMaxs);

	CMessageWriter(Category::HitscanEvents).WriteClearMessage();

	{
		CAABBoxConstructor constructor;
		constructor.SetBounds(gunPos + hullMins, gunPos + hullMaxs);

		GeometryItemPtr_t geom = constructor.Construct();
		geom->SetDrawType(CustomGeometry::DrawType::Lines);
		geom->SetColour(0xFFFFFFFF);

		CMessageWriter(Category::HitscanEvents).WriteMessage(*geom);
	}

	{
		Vector endPos(tr.vecEndPos);
		CAABBoxConstructor constructor;
		constructor.SetBounds(endPos + hullMins, endPos + hullMaxs);

		GeometryItemPtr_t geom = constructor.Construct();
		geom->SetDrawType(CustomGeometry::DrawType::Lines);
		geom->SetColour(0xFFFFFFFF);

		CMessageWriter(Category::HitscanEvents).WriteMessage(*geom);
	}

	{
		CGeometryItem geom;
		geom.AddLine(gunPos, tr.vecEndPos);
		geom.SetDrawType(CustomGeometry::DrawType::Lines);
		geom.SetColour(0xFFD800FF);

		CMessageWriter(Category::HitscanEvents).WriteMessage(geom);
	}
}
#endif

WeaponMechanics::InvocationResult CWeaponHullTrace::DoTrace(WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t)
{
#ifndef CLIENT_DLL
	DoTrace_Server();
#endif

	return WeaponMechanics::InvocationResult::Complete(mechanic);
}

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponHullTrace>()
	{
		return ::StaticWeaponAttributes;
	}
}  // namespace WeaponAtts
