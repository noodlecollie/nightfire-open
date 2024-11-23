#include "standard_includes.h"
#include "weaponmechanics/meleemechanic.h"
#include "weaponattributes/weaponatts_meleeattack.h"
#include "weapons/genericweapon.h"
#include "eventConstructor/eventConstructor.h"
#include "MathLib/utils.h"

namespace WeaponMechanics
{
	CMeleeMechanic::CMeleeMechanic(CGenericWeapon* weapon, const WeaponAtts::WAMeleeAttack* attackMode) :
		CBaseMechanic(weapon, attackMode)
	{
	}

	CMeleeMechanic::CMeleeMechanic(CGenericWeapon* weapon, uint32_t attackIndex) :
		CBaseMechanic(weapon, weapon->GetAttackModeFromAttributes<WeaponAtts::WAMeleeAttack>(attackIndex))
	{
	}

	void CMeleeMechanic::Precache()
	{
		CBaseMechanic::Precache();

		const WeaponAtts::WAMeleeAttack* attackMode = MeleeAttackMode();

		PrecacheSoundSet(attackMode->BodyHitSounds);
		PrecacheSoundSet(attackMode->WorldHitSounds);
	}

	InvocationResult CMeleeMechanic::Invoke(uint32_t step)
	{
		const InvocationResult result = CBaseMechanic::Invoke(step);

		if ( result.WasRejected() )
		{
			return result;
		}

		const WeaponAtts::WAMeleeAttack* meleeAttack = MeleeAttackMode();

		if ( step == 0 )
		{
			FireEvent();
			GetPlayer()->SetAnimation(PLAYER_ATTACK1);
			DelayFiring(1.0f / meleeAttack->AttackRate);
		}

		const CUtlVector<float>& strikes = meleeAttack->Strikes;
		const bool firstStrikeIsDelayed = !strikes.IsEmpty() && strikes[0] > 0.0f;
		const bool shouldSkipStrike = step == 0 && firstStrikeIsDelayed;

		if ( !shouldSkipStrike )
		{
			InitTraceVecs();

#ifndef CLIENT_DLL
			AttackStrike_Server();
#endif
		}

		// TODO: Is there a more intelligent value than 5 here?
		SetNextIdleTime(5, true);

		const uint32_t nextStrikeIndex = firstStrikeIsDelayed ? step : (step + 1);

		if ( nextStrikeIndex < static_cast<uint32_t>(strikes.Count()) )
		{
			const float delay = GetDelayFromPreviousStrike(nextStrikeIndex);
			return InvocationResult::Incomplete(*this, delay);
		}

		// End of strike sequence.
		return InvocationResult::Complete(*this);
	}

	const WeaponAtts::WAMeleeAttack* CMeleeMechanic::MeleeAttackMode() const
	{
		return GetAttackMode<WeaponAtts::WAMeleeAttack>();
	}

	void CMeleeMechanic::FireEvent()
	{
		using namespace EventConstructor;

		if ( GetEventIndex() < 0 )
		{
			return;
		}

		CEventConstructor event;

		event << Flags(DefaultEventFlags()) << Invoker(GetPlayer()->edict())
			  << EventIndex(static_cast<unsigned short>(GetEventIndex()));

		event.Send();
	}

	void CMeleeMechanic::InitTraceVecs()
	{
		CBasePlayer* player = GetPlayer();
		const WeaponAtts::WAMeleeAttack* meleeAttack = MeleeAttackMode();

		Vector forward;
		AngleVectors(player->pev->v_angle, forward, nullptr, nullptr);

		m_vecAttackTraceStart = player->GetGunPosition();
		m_vecAttackTraceEnd = m_vecAttackTraceStart + (forward * meleeAttack->Reach);
		m_vecContactPointOnSurface = Vector();
	}

	float CMeleeMechanic::GetDelayFromPreviousStrike(uint32_t index) const
	{
		const CUtlVector<float>& strikes = MeleeAttackMode()->Strikes;

		if ( index >= static_cast<uint32_t>(strikes.Count()) )
		{
			return 0.0f;
		}

		float delay = strikes[static_cast<int>(index)];

		if ( index == 0 )
		{
			return delay;
		}

		return delay - strikes[static_cast<int>(index - 1)];
	}

#ifndef CLIENT_DLL
	void CMeleeMechanic::FindHullIntersection(
		const Vector& vecSrc,
		TraceResult& tr,
		float* mins,
		float* maxs,
		edict_t* pEntity)
	{
		float* minmaxs[2] = {mins, maxs};
		TraceResult tmpTrace;
		Vector vecEnd;

		float distance = 1e6f;

		Vector vecHullEnd = vecSrc + ((Vector(tr.vecEndPos) - vecSrc) * 2);
		UTIL_TraceLine(vecSrc, vecHullEnd, dont_ignore_monsters, pEntity, &tmpTrace);

		if ( tmpTrace.flFraction < 1.0 )
		{
			tr = tmpTrace;
			return;
		}

		for ( int i = 0; i < 2; i++ )
		{
			for ( int j = 0; j < 2; j++ )
			{
				for ( int k = 0; k < 2; k++ )
				{
					vecEnd.x = vecHullEnd.x + minmaxs[i][0];
					vecEnd.y = vecHullEnd.y + minmaxs[j][1];
					vecEnd.z = vecHullEnd.z + minmaxs[k][2];

					UTIL_TraceLine(vecSrc, vecEnd, dont_ignore_monsters, pEntity, &tmpTrace);

					if ( tmpTrace.flFraction < 1.0 )
					{
						float thisDistance = (Vector(tmpTrace.vecEndPos) - vecSrc).Length();

						if ( thisDistance < distance )
						{
							tr = tmpTrace;
							distance = thisDistance;
						}
					}
				}
			}
		}
	}

	void CMeleeMechanic::AttackStrike_Server()
	{
		TraceResult tr;

		if ( !CheckForContact(tr) )
		{
			return;
		}

		CBasePlayer* player = GetPlayer();
		CGenericWeapon* weapon = GetWeapon();
		const WeaponAtts::WAMeleeAttack* meleeAttack = MeleeAttackMode();
		const WeaponAtts::WASoundSet* bodyHitSounds = &meleeAttack->BodyHitSounds;
		const WeaponAtts::WASoundSet* worldHitSounds = &meleeAttack->WorldHitSounds;

		if ( bodyHitSounds->SoundNames.Count() < 1 )
		{
			bodyHitSounds = worldHitSounds;
		}

		CBaseEntity* pEntity = CBaseEntity::Instance(tr.pHit);
		const bool hitBody = pEntity && pEntity->Classify() != CLASS_NONE && pEntity->Classify() != CLASS_MACHINE;

		if ( pEntity )
		{
			ClearMultiDamage();

			float damagePerShot = 1.0f;
			const WeaponAtts::WASkillRecord::SkillDataEntryPtr dmgPtr = meleeAttack->BaseDamagePerHit;

			if ( dmgPtr )
			{
				damagePerShot = gSkillData.*dmgPtr;
			}

			pEntity->TraceAttack(player->pev, damagePerShot, gpGlobals->v_forward, &tr, DMG_CLUB);

			ApplyMultiDamage(weapon->pev, player->pev);
		}

		if ( hitBody )
		{
			PlaySound(*bodyHitSounds, CHAN_ITEM);
			player->m_iWeaponVolume = meleeAttack->Volume;
		}
		else
		{
			Vector traceEnd = m_vecAttackTraceStart + ((m_vecContactPointOnSurface - m_vecAttackTraceStart) * 2);

			TEXTURETYPE_PlaySound(&tr, m_vecAttackTraceStart, traceEnd, BULLET_MELEE);

			PlaySound(*worldHitSounds, CHAN_ITEM, 1.0f);
			player->m_iWeaponVolume = meleeAttack->Volume;
		}

		if ( meleeAttack->DecalOnImpact )
		{
			DecalGunshot(&tr, BULLET_MELEE);
		}
	}

	bool CMeleeMechanic::CheckForContact(TraceResult& tr)
	{
		CBasePlayer* player = GetPlayer();

		UTIL_TraceLine(
			Vector(m_vecAttackTraceStart),
			Vector(m_vecAttackTraceEnd),
			dont_ignore_monsters,
			ENT(player->pev),
			&tr);

		if ( tr.flFraction >= 1.0f )
		{
			// Line didn't hit - try more expensive hull check instead.
			UTIL_TraceHull(
				m_vecAttackTraceStart,
				m_vecAttackTraceEnd,
				dont_ignore_monsters,
				head_hull,
				ENT(player->pev),
				&tr);

			if ( tr.flFraction < 1.0f )
			{
				// Calculate the point of intersection of the line (or hull) and the object we hit
				// This is an approximation of the "best" intersection
				CBaseEntity* pHit = CBaseEntity::Instance(tr.pHit);

				if ( !pHit || pHit->IsBSPModel() )
				{
					FindHullIntersection(
						m_vecAttackTraceStart,
						tr,
						VEC_DUCK_HULL_MIN,
						VEC_DUCK_HULL_MAX,
						player->edict());
				}

				// This is the point on the actual surface (the hull could have hit space)
				m_vecContactPointOnSurface = tr.vecEndPos;
			}
		}

		return tr.flFraction < 1.0f;
	}
#endif
}  // namespace WeaponMechanics
