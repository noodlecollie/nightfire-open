#include "standard_includes.h"
#include "entity_state.h"
#include "cvardef.h"
#include "gameplay/weaponInaccuracyCalculator.h"
#include "util/extramath.h"
#include "util/cvarFuncs.h"
#include "weaponattributes/weaponatts_ammobasedattack.h"
#include "gameplay/inaccuracymodifiers.h"

bool CWeaponInaccuracyCalculator::m_CvarsLoaded = false;
cvar_t* CWeaponInaccuracyCalculator::m_CvarMaxSpeed = nullptr;
cvar_t* CWeaponInaccuracyCalculator::m_CvarMaxFallSpeed = nullptr;
WeaponAtts::AccuracyParameters CWeaponInaccuracyCalculator::m_StaticDebugParams;

CWeaponInaccuracyCalculator::CWeaponInaccuracyCalculator()
{
	Clear();
	InitCvars();
}

float CWeaponInaccuracyCalculator::InstantaneousInaccuracy() const
{
	return m_InstantaneousInaccuracy;
}

float CWeaponInaccuracyCalculator::SmoothedInaccuracy() const
{
	return m_SmoothedInaccuracy;
}

void CWeaponInaccuracyCalculator::SetLastSmoothedInaccuracy(float inaccuracy)
{
	m_LastSmoothedInaccuracy = inaccuracy;
}

const CWeaponInaccuracyCalculator::Accuracy* CWeaponInaccuracyCalculator::AccuracyParams() const
{
	return m_AccuracyParams;
}

void CWeaponInaccuracyCalculator::SetAccuracyParams(const Accuracy* params)
{
	m_AccuracyParams = params;
}

CBasePlayer* CWeaponInaccuracyCalculator::Player() const
{
	return m_Player;
}

void CWeaponInaccuracyCalculator::SetPlayer(CBasePlayer* player)
{
	m_Player = player;
}

void CWeaponInaccuracyCalculator::SetFiredThisFrame(bool fired)
{
	m_FiredThisFrame = fired;
}

void CWeaponInaccuracyCalculator::SetLastFireTime(float time)
{
	m_LastFireTime = time;
}

void CWeaponInaccuracyCalculator::SetLastFireTimeIsDecremented(bool decremented)
{
	m_LastFireTimeIsDecremented = decremented;
}

bool CWeaponInaccuracyCalculator::IsValid() const
{
	return m_Player && m_CvarMaxSpeed && m_CvarMaxFallSpeed;
}

void CWeaponInaccuracyCalculator::Clear()
{
	m_InstantaneousInaccuracy = 0.0f;
	m_SmoothedInaccuracy = 0.0f;
	m_LastSmoothedInaccuracy = 0.0f;
	m_FiredThisFrame = false;
	m_LastFireTime = 0.0f;
	m_LastFireTimeIsDecremented = false;
	m_AccuracyParams = nullptr;
	m_Player = nullptr;
}

void CWeaponInaccuracyCalculator::CalculateInaccuracy()
{
	const WeaponAtts::AccuracyParameters* params = GetInternalParameters();
	CalculateInstantaneousInaccuracy(params);
	CalculateSmoothedInaccuracy(params);
}

void CWeaponInaccuracyCalculator::AddInaccuracyPenaltyFromFiring()
{
	if ( !m_FiredThisFrame )
	{
		return;
	}

	m_FiredThisFrame = false;

	if ( !IsValid() )
	{
		return;
	}

	const WeaponAtts::AccuracyParameters* params = GetInternalParameters();

	if ( !params )
	{
		return;
	}

	m_SmoothedInaccuracy += params->FireImpulse;

	const float maxInaccuracy = m_InstantaneousInaccuracy + params->FireImpulseCeiling;

	if ( m_SmoothedInaccuracy > maxInaccuracy )
	{
		m_SmoothedInaccuracy = maxInaccuracy;
	}
}

void CWeaponInaccuracyCalculator::CalculateInstantaneousInaccuracy(const WeaponAtts::AccuracyParameters* params)
{
	m_InstantaneousInaccuracy = 0.0f;

	if ( !IsValid() || !params )
	{
		return;
	}

	if ( (params->RunSpread - params->RestSpread).Length() < 0.001f )
	{
		// No spread difference, don't bother.
		return;
	}

	const float maxPlayerSpeed = m_CvarMaxSpeed->value;
	float spreadValue = ExtraMath::RemapLinear(m_Player->pev->velocity.Length2D(),
											   0.0f,
											   maxPlayerSpeed,
											   params->RestValue,
											   params->RunValue);

	if ( m_Player->pev->button & IN_DUCK )
	{
		spreadValue += params->CrouchShift;
	}

	if ( !(m_Player->pev->flags & FL_ONGROUND) )
	{
		spreadValue += params->AirShift;
	}

	const float zSpeed = fabs(m_Player->pev->velocity.z);
	const float maxZSpeed = m_CvarMaxFallSpeed->value;
	const float shiftFromZSpeed = ExtraMath::RemapSqrt(zSpeed, 0.0f, maxZSpeed, 0, params->FallShift);

	spreadValue += shiftFromZSpeed;
	m_InstantaneousInaccuracy = ExtraMath::Clamp(0.0f, spreadValue, 1.0f);
}

void CWeaponInaccuracyCalculator::CalculateSmoothedInaccuracy(const WeaponAtts::AccuracyParameters* params)
{
	m_SmoothedInaccuracy = m_InstantaneousInaccuracy;

	if ( !IsValid() || !params )
	{
		return;
	}

	const float difference = m_InstantaneousInaccuracy - m_LastSmoothedInaccuracy;
	float smoothed = m_LastSmoothedInaccuracy;

	if ( difference > 0.0f )
	{
		smoothed += params->AttackCoefficient * difference;
	}
	else if ( !LastFireTimeIsInHoldRegion(params->FireImpulseHoldTime) )
	{
		smoothed += params->DecayCoefficient * difference;
	}

	m_SmoothedInaccuracy = ExtraMath::Clamp(0.0f, smoothed, 1.0f);
}

bool CWeaponInaccuracyCalculator::LastFireTimeIsInHoldRegion(float holdTime) const
{
	if ( !IsValid() )
	{
		return false;
	}

	const float baseTime = m_LastFireTimeIsDecremented ? 0.0f : gpGlobals->time;
	return (baseTime - m_LastFireTime) <= holdTime;
}

const WeaponAtts::AccuracyParameters* CWeaponInaccuracyCalculator::GetInternalParameters() const
{
	const WeaponAtts::AccuracyParameters* params = m_AccuracyParams;

	if ( InaccuracyModifiers::IsInaccuracyDebuggingEnabled() )
	{
		const bool gotValues = InaccuracyModifiers::GetInaccuracyValuesFromDebugCvars(m_StaticDebugParams);
		params = gotValues ? &m_StaticDebugParams : nullptr;
	}

	return params;
}

void CWeaponInaccuracyCalculator::InitCvars()
{
	if ( m_CvarsLoaded )
	{
		return;
	}

	m_CvarMaxSpeed = GetCvarByName("sv_weapon_inaccuracy_maxspeed");
	m_CvarMaxFallSpeed = GetCvarByName("sv_weapon_inaccuracy_maxfallspeed");

	m_CvarsLoaded =
		m_CvarMaxSpeed &&
		m_CvarMaxFallSpeed;

	ASSERTSZ(m_CvarsLoaded, "Unable to load required cvars.");
}
