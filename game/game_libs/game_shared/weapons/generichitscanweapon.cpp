#include <memory>
#include "generichitscanweapon.h"
#include "weaponatts_hitscanattack.h"
#include "skill.h"
#include "eventConstructor/eventConstructor.h"
#include "gameplay/inaccuracymodifiers.h"

#ifndef CLIENT_DLL
#include "weaponregistry.h"
#include "weapondebugevents/weapondebugevent_hitscanfire.h"
#endif

CGenericHitscanWeapon::~CGenericHitscanWeapon()
{
#ifndef CLIENT_DLL
	Debug_DeleteHitscanEvent();
#endif
}

void CGenericHitscanWeapon::WeaponIdle()
{
	const WeaponAtts::WAHitscanAttack* primaryAttack = GetPrimaryAttackMode<const WeaponAtts::WAHitscanAttack>();

	if ( primaryAttack )
	{
		m_pPlayer->GetAutoaimVector(primaryAttack->AutoAim);
	}

	CGenericWeapon::WeaponIdle();
}

void CGenericHitscanWeapon::PrecacheAttackMode(const WeaponAtts::WABaseAttack& attackMode)
{
	CGenericWeapon::PrecacheAttackMode(attackMode);

	if ( attackMode.Classify() != WeaponAtts::WABaseAttack::Classification::Hitscan )
	{
		ASSERT(false);
		return;
	}

	const WeaponAtts::WAHitscanAttack& hitscanAttack = static_cast<const WeaponAtts::WAHitscanAttack&>(attackMode);

	if ( hitscanAttack.ShellModelName )
	{
		PRECACHE_MODEL(hitscanAttack.ShellModelName);
	}
}

bool CGenericHitscanWeapon::InvokeWithAttackMode(const CGenericWeapon::WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode)
{
	if ( !attackMode || attackMode->Classify() != WeaponAtts::WABaseAttack::Classification::Hitscan )
	{
		return false;
	}

	const WeaponAtts::WAHitscanAttack* hitscanAttack = static_cast<const WeaponAtts::WAHitscanAttack*>(attackMode);

	if ( hitscanAttack->AttackRate <= 0.0f || hitscanAttack->BulletsPerShot < 1 )
	{
		return false;
	}

	// Check base class allows the attack:
	if ( !CGenericWeapon::InvokeWithAttackMode(type, hitscanAttack) )
	{
		return false;
	}

	DecrementAmmo(hitscanAttack, 1);

	m_pPlayer->pev->effects = (int)( m_pPlayer->pev->effects ) | EF_MUZZLEFLASH;

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_pPlayer->m_iWeaponVolume = hitscanAttack->Volume;
	m_pPlayer->m_iWeaponFlash = hitscanAttack->MuzzleFlashBrightness;

	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecAiming;

	if( hitscanAttack->AutoAim > 0.0f )
	{
		vecAiming = m_pPlayer->GetAutoaimVector(hitscanAttack->AutoAim);
	}
	else
	{
		vecAiming = gpGlobals->v_forward;
	}

	Vector vecDir = FireBulletsPlayer(*hitscanAttack, vecSrc, vecAiming);
	const int eventID = GetEventIDForAttackMode(hitscanAttack);

	if ( eventID >= 0 )
	{
		using namespace EventConstructor;
		CEventConstructor event;

		event
			<< Flags(DefaultEventFlags())
			<< Invoker(m_pPlayer->edict())
			<< EventIndex(eventID)
			<< IntParam1(m_pPlayer->random_seed)
			<< BoolParam1(m_iClip == 0)
			<< FloatParam1(GetInaccuracy())
			;

		event.Send();
	}

	DelayFiring(1.0f / hitscanAttack->AttackRate);

	if ( !HasAmmo(hitscanAttack, 1, true) && !HasAmmo(hitscanAttack, 1, false) )
	{
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}

	SetNextIdleTime(5, true);
	return true;
}

Vector CGenericHitscanWeapon::FireBulletsPlayer(const WeaponAtts::WAHitscanAttack& hitscanAttack,
												const Vector& vecSrc,
										 		const Vector& vecDirShooting)
{
#ifdef CLIENT_DLL
	// The client doesn't actually do any bullet simulation, we just make sure that
	// the generated random vectors match up.
	return FireBulletsPlayer_Client(hitscanAttack);
#else
	TraceResult tr;
	Vector vecRight = gpGlobals->v_right;
	Vector vecUp = gpGlobals->v_up;
	float x = 0.0f;
	float y = 0.0f;

	WeaponAtts::AccuracyParameters accuracyParams(hitscanAttack.Accuracy);

	if ( InaccuracyModifiers::IsInaccuracyDebuggingEnabled() )
	{
		InaccuracyModifiers::GetInaccuracyValuesFromDebugCvars(accuracyParams);
	}

	const Vector2D spread = InaccuracyModifiers::GetInterpolatedSpread(accuracyParams, GetInaccuracy());

	entvars_t* const pevAttacker = m_pPlayer->pev;
	const int shared_rand = m_pPlayer->random_seed;

	ClearMultiDamage();
	gMultiDamage.type = DMG_BULLET | DMG_NEVERGIB;

	const uint32_t numShots = hitscanAttack.BulletsPerShot;
	for( uint32_t shot = 0; shot < numShots; shot++ )
	{
		float damagePerShot = 0.0f;
		const WeaponAtts::WASkillRecord::SkillDataEntryPtr dmgPtr = hitscanAttack.BaseDamagePerShot;
		if ( dmgPtr )
		{
			damagePerShot = gSkillData.*dmgPtr;
		}

		GetSharedCircularGaussianSpread(shot, shared_rand, x, y);

		Vector vecDir = vecDirShooting + (x * spread.x * vecRight) + (y * spread.y * vecUp);
		Vector vecEnd = vecSrc + (vecDir * DEFAULT_BULLET_TRACE_DISTANCE);
		UTIL_TraceLine(vecSrc, vecEnd, dont_ignore_monsters, ENT(pev), &tr);

		Debug_HitscanBulletFired(vecSrc, tr);

		// do damage, paint decals
		if( tr.flFraction != 1.0 )
		{
			CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);

			pEntity->TraceAttack(pevAttacker, damagePerShot, vecDir, &tr, DMG_BULLET);
			TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, BULLET_GENERIC);
		}

		// make bullet trails
		// TODO: This should be clientside in the event playback.
		UTIL_BubbleTrail(vecSrc, tr.vecEndPos, (int)((DEFAULT_BULLET_TRACE_DISTANCE * tr.flFraction) / 64.0));
	}

	Debug_FinaliseHitscanEvent();
	ApplyMultiDamage(pev, pevAttacker);

	return Vector(x * spread.x, y * spread.y, 0.0);
#endif
}

////////////////////////////////////////////
// SERVER
////////////////////////////////////////////
#ifndef CLIENT_DLL
void CGenericHitscanWeapon::Debug_HitscanBulletFired(const Vector& start, const TraceResult& tr)
{
	CWeaponDebugEventSource& evSource = CWeaponRegistry::StaticInstance().DebugEventSource();

	if ( !evSource.EventHasSubscribers(CWeaponDebugEvent_Base::EventType::Event_HitscanFire) )
	{
		return;
	}

	if ( !m_pHitscanFireEvent )
	{
		m_pHitscanFireEvent = new CWeaponDebugEvent_HitscanFire(*this);
	}

	m_pHitscanFireEvent->AddTrace(start, tr);
}

void CGenericHitscanWeapon::Debug_FinaliseHitscanEvent()
{
	if ( !m_pHitscanFireEvent )
	{
		return;
	}

	// The hitscan fire event will only have been created if we had event subscribers in the first place,
	// so no need to check this.
	CWeaponDebugEventSource& evSource = CWeaponRegistry::StaticInstance().DebugEventSource();
	evSource.FireEvent(m_pHitscanFireEvent);

	Debug_DeleteHitscanEvent();
}

void CGenericHitscanWeapon::Debug_DeleteHitscanEvent()
{
	delete m_pHitscanFireEvent;
	m_pHitscanFireEvent = nullptr;
}
#endif

////////////////////////////////////////////
// CLIENT
////////////////////////////////////////////
#ifdef CLIENT_DLL
Vector CGenericHitscanWeapon::FireBulletsPlayer_Client(const WeaponAtts::WAHitscanAttack& hitscanAttack)
{
	float x = 0.0f;
	float y = 0.0f;

	WeaponAtts::AccuracyParameters accuracyParams(hitscanAttack.Accuracy);

	if ( InaccuracyModifiers::IsInaccuracyDebuggingEnabled() )
	{
		InaccuracyModifiers::GetInaccuracyValuesFromDebugCvars(accuracyParams);
	}

	const Vector2D spread = InaccuracyModifiers::GetInterpolatedSpread(accuracyParams, GetInaccuracy());

	// Just return the last vector we would have generated.
	GetSharedCircularGaussianSpread(hitscanAttack.BulletsPerShot - 1, m_pPlayer->random_seed, x, y);
	return Vector(x * spread.x, y * spread.y, 0.0);
}
#endif
