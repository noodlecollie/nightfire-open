#include "genericmeleeweapon.h"
#include "util.h"
#include "weaponatts_meleeattack.h"
#include "gamerules.h"
#include "eventConstructor/eventConstructor.h"

namespace
{
	void FindHullIntersection( const Vector &vecSrc, TraceResult &tr, float *mins, float *maxs, edict_t *pEntity )
	{
		float* minmaxs[2] = {mins, maxs};
		TraceResult tmpTrace;
		Vector vecEnd;

		float distance = 1e6f;

		Vector vecHullEnd = vecSrc + ( (tr.vecEndPos - vecSrc) * 2 );
		UTIL_TraceLine(vecSrc, vecHullEnd, dont_ignore_monsters, pEntity, &tmpTrace);

		if( tmpTrace.flFraction < 1.0 )
		{
			tr = tmpTrace;
			return;
		}

		for( int i = 0; i < 2; i++ )
		{
			for( int j = 0; j < 2; j++ )
			{
				for( int k = 0; k < 2; k++ )
				{
					vecEnd.x = vecHullEnd.x + minmaxs[i][0];
					vecEnd.y = vecHullEnd.y + minmaxs[j][1];
					vecEnd.z = vecHullEnd.z + minmaxs[k][2];

					UTIL_TraceLine(vecSrc, vecEnd, dont_ignore_monsters, pEntity, &tmpTrace);

					if( tmpTrace.flFraction < 1.0 )
					{
						float thisDistance = (tmpTrace.vecEndPos - vecSrc).Length();

						if( thisDistance < distance )
						{
							tr = tmpTrace;
							distance = thisDistance;
						}
					}
				}
			}
		}
	}
}

CGenericMeleeWeapon::CGenericMeleeWeapon() : CGenericWeapon(),
	m_pCachedAttack(nullptr),
	m_iStrikeIndex(0)
{
}

void CGenericMeleeWeapon::Precache()
{
	CGenericWeapon::Precache();
}

void CGenericMeleeWeapon::PrecacheAttackMode(const WeaponAtts::WABaseAttack& attackMode)
{
	CGenericWeapon::PrecacheAttackMode(attackMode);

	if ( attackMode.Classify() != WeaponAtts::WABaseAttack::Classification::Melee )
	{
		ASSERT(false);
		return;
	}

	const WeaponAtts::WAMeleeAttack& hitscanAttack = static_cast<const WeaponAtts::WAMeleeAttack&>(attackMode);

	PrecacheSoundSet(hitscanAttack.BodyHitSounds);
	PrecacheSoundSet(hitscanAttack.WorldHitSounds);
}

bool CGenericMeleeWeapon::InvokeWithAttackMode(WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode)
{
	if ( !attackMode || attackMode->Classify() != WeaponAtts::WABaseAttack::Classification::Melee )
	{
		return false;
	}

	if ( !CGenericWeapon::InvokeWithAttackMode(type, attackMode) )
	{
		return false;
	}

	const WeaponAtts::WAMeleeAttack* meleeAttack = static_cast<const WeaponAtts::WAMeleeAttack*>(attackMode);

	FireEvent(meleeAttack);
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	DelayFiring(1.0f / meleeAttack->AttackRate);
	SetNextIdleTime(5, true);

	m_pCachedAttack = meleeAttack;
	m_iStrikeIndex = 0;

	float strikeTime = meleeAttack->Strikes.Count() > 0
		? meleeAttack->Strikes[m_iStrikeIndex]
		: 0.0f;

	if ( strikeTime <= 0.0f )
	{
		AttackStrike();
	}
	else
	{
		SetThink(&CGenericMeleeWeapon::AttackStrike);
		pev->nextthink = gpGlobals->time + strikeTime;
	}

	return true;
}

void CGenericMeleeWeapon::AttackStrike()
{
	if ( !m_pCachedAttack )
	{
		return;
	}

	InitTraceVecs(m_pCachedAttack);

	TraceResult tr;
	bool madeContact = CheckForContact(m_pCachedAttack, tr);

	const WeaponAtts::WASoundSet* bodyHitSounds = &m_pCachedAttack->BodyHitSounds;
	const WeaponAtts::WASoundSet* worldHitSounds = &m_pCachedAttack->WorldHitSounds;

	if ( bodyHitSounds->SoundNames.Count() < 1 )
	{
		bodyHitSounds = worldHitSounds;
	}

	if ( madeContact )
	{
#ifndef CLIENT_DLL
		CBaseEntity* pEntity = CBaseEntity::Instance(tr.pHit);
		const bool hitBody = pEntity && pEntity->Classify() != CLASS_NONE && pEntity->Classify() != CLASS_MACHINE;

		if ( pEntity )
		{
			ClearMultiDamage();

			float damagePerShot = 1.0f;
			const WeaponAtts::WASkillRecord::SkillDataEntryPtr dmgPtr = m_pCachedAttack->BaseDamagePerHit;

			if ( dmgPtr )
			{
				damagePerShot = gSkillData.*dmgPtr;
			}

			pEntity->TraceAttack(m_pPlayer->pev, damagePerShot, gpGlobals->v_forward, &tr, DMG_CLUB);

			ApplyMultiDamage(m_pPlayer->pev, m_pPlayer->pev);
		}

		if ( hitBody )
		{
			PlaySound(*bodyHitSounds, CHAN_ITEM);
			m_pPlayer->m_iWeaponVolume = m_pCachedAttack->Volume;
		}
		else
		{
			vec3_t traceEnd = m_vecAttackTraceStart + ((m_vecContactPointOnSurface - m_vecAttackTraceStart) * 2);

			TEXTURETYPE_PlaySound(&tr, m_vecAttackTraceStart, traceEnd, BULLET_MELEE);

			PlaySound(*worldHitSounds, CHAN_ITEM, 1.0f);
			m_pPlayer->m_iWeaponVolume = m_pCachedAttack->Volume;
		}

		if ( m_pCachedAttack->DecalOnImpact )
		{
			DecalGunshot(&tr, BULLET_MELEE);
		}
#endif
	}

	if ( m_iStrikeIndex >= m_pCachedAttack->Strikes.Count() - 1 )
	{
		// End of strike sequence.
		m_pCachedAttack = nullptr;
		m_iStrikeIndex = 0;
	}
	else
	{
		++m_iStrikeIndex;
		float nextStrikeTime = m_pCachedAttack->Strikes[m_iStrikeIndex];

		SetThink(&CGenericMeleeWeapon::AttackStrike);
		pev->nextthink = gpGlobals->time + nextStrikeTime;
	}

}

bool CGenericMeleeWeapon::CheckForContact(const WeaponAtts::WAMeleeAttack* meleeAttack, TraceResult& tr)
{
	UTIL_TraceLine(m_vecAttackTraceStart, m_vecAttackTraceEnd, dont_ignore_monsters, ENT(m_pPlayer->pev), &tr);

#ifndef CLIENT_DLL
	if( tr.flFraction >= 1.0f )
	{
		// Line didn't hit - try more expensive hull check instead.
		UTIL_TraceHull( m_vecAttackTraceStart, m_vecAttackTraceEnd, dont_ignore_monsters, head_hull, ENT(m_pPlayer->pev), &tr);

		if( tr.flFraction < 1.0f )
		{
			// Calculate the point of intersection of the line (or hull) and the object we hit
			// This is and approximation of the "best" intersection
			CBaseEntity *pHit = CBaseEntity::Instance(tr.pHit);

			if( !pHit || pHit->IsBSPModel() )
			{
				FindHullIntersection(m_vecAttackTraceStart, tr, VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX, m_pPlayer->edict());
			}

			m_vecContactPointOnSurface = tr.vecEndPos;	// This is the point on the actual surface (the hull could have hit space)
		}
	}
#endif

	return tr.flFraction < 1.0f;
}

void CGenericMeleeWeapon::FireEvent(const WeaponAtts::WAMeleeAttack* meleeAttack)
{
	using namespace EventConstructor;

	const int eventID = GetEventIDForAttackMode(meleeAttack);

	if ( eventID < 0 )
	{
		return;
	}

	CEventConstructor event;

	event
		<< Flags(DefaultEventFlags())
		<< Invoker(m_pPlayer->edict())
		<< EventIndex(eventID)
		;

	event.Send();
}

void CGenericMeleeWeapon::InitTraceVecs(const WeaponAtts::WAMeleeAttack* meleeAttack)
{
	UTIL_MakeVectors(m_pPlayer->pev->v_angle);
	m_vecAttackTraceStart = m_pPlayer->GetGunPosition();
	m_vecAttackTraceEnd = m_vecAttackTraceStart + (gpGlobals->v_forward * meleeAttack->Reach);
	m_vecContactPointOnSurface = vec3_t();
}
