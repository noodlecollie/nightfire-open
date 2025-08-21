#include "weapon_projectiletrace.h"
#include "weaponregistry.h"
#include "EnginePublicAPI/weaponinfo.h"
#include "skill.h"
#include "gamerules.h"
#include "weapon_pref_weights.h"
#include "weapon_projectiletrace_atts.h"
#include "weaponmechanics/delegatedmechanic.h"
#include "MathLib/utils.h"
#include "customGeometry/constructors/aabboxConstructor.h"
#include "util/projectileMotion.h"

#ifndef CLIENT_DLL
#include "bot.h"
#include "customGeometry/messageWriter.h"
#include "customGeometry/rollingLineMessageWriter.h"
#include "customGeometry/constructors/crosshair3DConstructor.h"
#endif

LINK_ENTITY_TO_CLASS(weapon_projectiletrace, CWeaponProjectileTrace)

CWeaponProjectileTrace::CWeaponProjectileTrace() :
	CGenericWeapon()
{
	AddMechanicByAttributeIndex<WeaponAtts::WAEventAttack>(ATTACKMODE_NORMAL, m_PrimaryMechanic);

	m_PrimaryMechanic->SetCallback(
		[this](WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step)
		{
			return DoTrace(mechanic, step);
		});

	SetPrimaryAttackMechanic(m_PrimaryMechanic);
}

const WeaponAtts::WACollection& CWeaponProjectileTrace::WeaponAttributes() const
{
	return WeaponAtts::StaticWeaponAttributes<CWeaponProjectileTrace>();
}

#ifndef CLIENT_DLL
float CWeaponProjectileTrace::Bot_CalcDesireToUse(CBaseBot&, CBaseEntity&, float) const
{
	return static_cast<float>(WeaponAttributes().Core.SwitchWeight) / static_cast<float>(WeaponPref_Max);
}

void CWeaponProjectileTrace::Bot_SetFightStyle(CBaseBotFightStyle&) const
{
}

void CWeaponProjectileTrace::DoTrace_Server()
{
	using namespace CustomGeometry;

	static constexpr uint32_t COLOUR = 0x4FFFEDFF;

	Vector gunPos = m_pPlayer->GetGunPosition();
	Vector aimAngle = Vector(m_pPlayer->pev->v_angle) + Vector(m_pPlayer->pev->punchangle);
	Vector launchVel;
	AngleVectors(aimAngle, launchVel, nullptr, nullptr);

	ALERT(at_console, "%f %f %f\n", aimAngle.x, aimAngle.y, aimAngle.z);

	launchVel *= debug_projectiletrace_launch_speed.value;

	CMessageWriter(Category::HitscanEvents).WriteClearMessage();

	CRollingLineMessageWriter writer(Category::HitscanEvents);
	writer.BeginGeometry(COLOUR);

	const int maxIterations = std::max<int>(static_cast<int>(debug_projectiletrace_max_iterations.value), 2);
	const float timeStep = std::max<float>(debug_projectiletrace_time_step.value, 0.001f);

	bool hitSomething = false;
	float currentTime = 0.0f;
	Vector lastLocation = gunPos;

	for ( size_t iteration = 0; iteration < static_cast<size_t>(maxIterations) && !hitSomething; ++iteration )
	{
		Vector currentLocation =
			CalculateProjectilePosition(gunPos, launchVel, debug_projectiletrace_gravity.value, currentTime);

		TraceResult tr {};
		TRACE_LINE(lastLocation, currentLocation, TRUE, m_pPlayer->edict(), &tr);
		hitSomething = tr.flFraction < 1.0f;

		if ( hitSomething )
		{
			currentLocation = tr.vecEndPos;
		}

		writer.AddLine(lastLocation, currentLocation);

		lastLocation = currentLocation;
		currentTime += timeStep;
	}

	writer.Finalise();

	if ( hitSomething )
	{
		CCrosshair3DConstructor constructor;
		constructor.SetOrigin(lastLocation);
		constructor.SetScale(8.0f);

		GeometryItemPtr_t item = constructor.Construct();
		item->SetColour(COLOUR);

		CMessageWriter(Category::HitscanEvents).WriteMessage(*item);
	}

	// TraceResult tr {};
	// g_engfuncs.pfnTraceHull(gunPos, gunPos + (128.0f * dir), ignore_monsters, human_hull, m_pPlayer->edict(), &tr);

	// Vector hullMins;
	// Vector hullMaxs;
	// g_engfuncs.pfnGetHullBounds(human_hull, hullMins, hullMaxs);

	// CMessageWriter(Category::HitscanEvents).WriteClearMessage();

	// {
	// 	CAABBoxConstructor constructor;
	// 	constructor.SetBounds(gunPos + hullMins, gunPos + hullMaxs);

	// 	GeometryItemPtr_t geom = constructor.Construct();
	// 	geom->SetDrawType(CustomGeometry::DrawType::Lines);
	// 	geom->SetColour(0xFFFFFFFF);

	// 	CMessageWriter(Category::HitscanEvents).WriteMessage(*geom);
	// }

	// {
	// 	Vector endPos(tr.vecEndPos);
	// 	CAABBoxConstructor constructor;
	// 	constructor.SetBounds(endPos + hullMins, endPos + hullMaxs);

	// 	GeometryItemPtr_t geom = constructor.Construct();
	// 	geom->SetDrawType(CustomGeometry::DrawType::Lines);
	// 	geom->SetColour(0xFFFFFFFF);

	// 	CMessageWriter(Category::HitscanEvents).WriteMessage(*geom);
	// }

	// {
	// 	CGeometryItem geom;
	// 	geom.AddLine(gunPos, tr.vecEndPos);
	// 	geom.SetDrawType(CustomGeometry::DrawType::Lines);
	// 	geom.SetColour(0xFFD800FF);

	// 	CMessageWriter(Category::HitscanEvents).WriteMessage(geom);
	// }
}
#endif

WeaponMechanics::InvocationResult CWeaponProjectileTrace::DoTrace(
	WeaponMechanics::CDelegatedMechanic& mechanic,
	uint32_t)
{
#ifndef CLIENT_DLL
	DoTrace_Server();
#endif

	return WeaponMechanics::InvocationResult::Complete(mechanic);
}

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponProjectileTrace>()
	{
		return ::StaticWeaponAttributes;
	}
}  // namespace WeaponAtts
