#include "radialdamage.h"
#include "standard_includes.h"
#include "util.h"
#include "weapons.h"

CRadialDamageInflictor::CRadialDamageInflictor()
{
}

Vector CRadialDamageInflictor::Origin() const
{
	return m_vecOrigin;
}

void CRadialDamageInflictor::SetOrigin(const Vector& origin)
{
	m_vecOrigin = origin;
}

entvars_t* CRadialDamageInflictor::Inflictor() const
{
	return m_pInflictor;
}

void CRadialDamageInflictor::SetInflictor(entvars_t* inflictor)
{
	m_pInflictor = inflictor;
}

entvars_t* CRadialDamageInflictor::Attacker() const
{
	return m_pAttacker;
}

void CRadialDamageInflictor::SetAttacker(entvars_t* attacker)
{
	m_pAttacker = attacker;
}

float CRadialDamageInflictor::Damage() const
{
	return m_flDamage;
}

void CRadialDamageInflictor::SetDamage(float damage)
{
	m_flDamage = damage;
}

float CRadialDamageInflictor::Radius() const
{
	return m_flRadius;
}

void CRadialDamageInflictor::SetRadius(float radius)
{
	if ( radius < 0.0f )
	{
		radius = 0.0f;
	}

	m_flRadius = radius;
}

int CRadialDamageInflictor::IgnoreClass() const
{
	return m_iIgnoreClass;
}

void CRadialDamageInflictor::SetIgnoreClass(int ignoreClass)
{
	m_iIgnoreClass = ignoreClass;
}

int CRadialDamageInflictor::DamageTypeBits() const
{
	return m_bitsDamageType;
}

void CRadialDamageInflictor::SetDamageTypeBits(int bits)
{
	m_bitsDamageType = bits;
}

float CRadialDamageInflictor::AttackerDamageMultiplier() const
{
	return m_flAttackerDamageMultiplier;
}

void CRadialDamageInflictor::SetAttackerDamageMultiplier(float multiplier)
{
	if ( multiplier < 0.0f )
	{
		multiplier = 0.0f;
	}

	m_flAttackerDamageMultiplier = multiplier;
}

void CRadialDamageInflictor::ApplyDamage()
{
#ifndef CLIENT_DLL
	m_flWorkingFalloff = m_flRadius >= 0.0f ? (m_flDamage / m_flRadius) : 1.0f;
	m_bWorkingInWater = UTIL_PointContents(m_vecOrigin) == CONTENTS_WATER;
	m_pWorkingAttacker = m_pAttacker ? m_pAttacker : m_pInflictor;
	CBaseEntity* target = NULL;

	while ( (target = UTIL_FindEntityInSphere(target, m_vecOrigin, m_flRadius)) != NULL )
	{
		DamageEntityFromSphere(*target);
	}
#endif
}

#ifndef CLIENT_DLL
void CRadialDamageInflictor::DamageEntityFromSphere(CBaseEntity& target)
{
	if ( target.pev->takedamage == DAMAGE_NO )
	{
		return;
	}

	if ( m_iIgnoreClass != CLASS_NONE && target.Classify() == m_iIgnoreClass )
	{
		return;
	}

	// Blasts don't travel into or out of water
	if ( (m_bWorkingInWater && target.pev->waterlevel == 0) || (!m_bWorkingInWater && target.pev->waterlevel == 3) )
	{
		return;
	}

	m_TraceResult = TraceResult();
	UTIL_TraceLine(
		m_vecOrigin,
		target.BodyTarget(m_vecOrigin),
		dont_ignore_monsters,
		ENT(m_pInflictor),
		&m_TraceResult);

	// Apply damage if the entity was hit, or if they were right on the end of the line.
	if ( m_TraceResult.flFraction == 1.0f || m_TraceResult.pHit == target.edict() )
	{
		ApplyDamageToEntity(target);
	}
}

void CRadialDamageInflictor::ApplyDamageToEntity(CBaseEntity& target)
{
	if ( m_TraceResult.fStartSolid )
	{
		// if we're stuck inside them, fixup the position and distance
		VectorCopy(m_vecOrigin, m_TraceResult.vecEndPos);
		m_TraceResult.flFraction = 0.0f;
	}

	// Decrease damage for an ent that's farther from the bomb.
	const float adjustedDamage = GetAdjustedDamage(target, m_TraceResult.vecEndPos);
	if ( adjustedDamage <= 0.0f )
	{
		return;
	}

	if ( m_TraceResult.flFraction != 1.0 )
	{
		ClearMultiDamage();
		target.TraceAttack(
			m_pInflictor,
			adjustedDamage,
			(Vector(m_TraceResult.vecEndPos) - m_vecOrigin).Normalize(),
			&m_TraceResult,
			m_bitsDamageType);
		ApplyMultiDamage(m_pInflictor, m_pWorkingAttacker);
	}
	else
	{
		target.TakeDamage(m_pInflictor, m_pWorkingAttacker, adjustedDamage, m_bitsDamageType);
	}
}

float CRadialDamageInflictor::GetAdjustedDamage(CBaseEntity& target, const Vector& endPos) const
{
	float adjustedDamage = (m_vecOrigin - endPos).Length() * m_flWorkingFalloff;
	adjustedDamage = m_flDamage - adjustedDamage;

	// If a player hurt themselves with a grenade, for example,
	// apply the self-damage modifier.
	if ( target.pev == m_pWorkingAttacker )
	{
		adjustedDamage *= m_flAttackerDamageMultiplier;
	}

	if ( adjustedDamage < 0.0f )
	{
		adjustedDamage = 0.0f;
	}

	return adjustedDamage;
}
#endif
