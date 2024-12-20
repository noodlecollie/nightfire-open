#include "gameplay/hitscancomponent.h"
#include "gameplay/inaccuracymodifiers.h"
#include "gameplay/inaccuracyCvars.h"
#include "gameplay/spreadPatterns.h"
#include "weapons/weaponregistry.h"

#ifndef CLIENT_DLL
#include "weapondebugevents/weapondebugeventsource.h"
#endif

CHitscanComponent::~CHitscanComponent()
{
	// Required for using std::unique_ptr with a forward-declared type.
}

Vector CHitscanComponent::GetGunPos() const
{
	return m_GunPos;
}

void CHitscanComponent::SetGunPos(const Vector& pos)
{
	m_GunPos = pos;
}

Vector CHitscanComponent::GetShootDir() const
{
	return m_ShootDir;
}

void CHitscanComponent::SetShootDir(const Vector& dir)
{
	m_ShootDir = dir;
}

Vector CHitscanComponent::GetRightDir() const
{
	return m_RightDir;
}

void CHitscanComponent::SetRightDir(const Vector& dir)
{
	m_RightDir = dir;
}

Vector CHitscanComponent::GetUpDir() const
{
	return m_UpDir;
}

void CHitscanComponent::SetUpDir(const Vector& dir)
{
	m_UpDir = dir;
}

int CHitscanComponent::GetRandomSeed() const
{
	return m_RandomSeed;
}

void CHitscanComponent::SetRandomSeed(int seed)
{
	m_RandomSeed = seed;
}

CGenericWeapon* CHitscanComponent::GetWeapon() const
{
	return m_Weapon;
}

void CHitscanComponent::SetWeapon(CGenericWeapon* weapon)
{
	m_Weapon = weapon;
}

uint32_t CHitscanComponent::GetBulletsPerShot() const
{
	return m_BulletsPerShot;
}

void CHitscanComponent::SetBulletsPerShot(uint32_t bulletsPerShot)
{
	m_BulletsPerShot = bulletsPerShot;
}

WeaponAtts::SpreadPattern CHitscanComponent::GetSpreadPattern() const
{
	return m_SpreadPattern;
}

void CHitscanComponent::SetSpreadPattern(WeaponAtts::SpreadPattern pattern)
{
	m_SpreadPattern = pattern;
}

bool CHitscanComponent::GetSendTracerMessage() const
{
	return m_SendTracerMessage;
}

void CHitscanComponent::SetSendTracerMessage(bool send)
{
	m_SendTracerMessage = send;
}

void CHitscanComponent::SetSpread(const WeaponAtts::AccuracyParameters& params, float inaccuracy)
{
	WeaponAtts::AccuracyParameters localAccuracyParams(params);

	if ( InaccuracyModifiers::IsInaccuracyDebuggingEnabled() )
	{
		InaccuracyModifiers::GetInaccuracyValuesFromDebugCvars(localAccuracyParams);
	}

	m_BaseSpread = InaccuracyModifiers::GetInterpolatedSpread(localAccuracyParams, inaccuracy);
}

void CHitscanComponent::SetSpread(const Vector2D& spread)
{
	m_BaseSpread = spread;
}

void CHitscanComponent::SetBaseDamagePerShot(WeaponAtts::WASkillRecord::SkillDataEntryPtr ptr)
{
	m_BaseDamagePerShot = ptr ? gSkillData.*ptr : 0.0f;
}

void CHitscanComponent::SetBaseDamagePerShot(float damage)
{
	m_BaseDamagePerShot = damage;
}

entvars_t* CHitscanComponent::GetInflictor() const
{
	return m_pevInflictor;
}

void CHitscanComponent::SetInflictor(entvars_t* inflictor)
{
	m_pevInflictor = inflictor;
}

entvars_t* CHitscanComponent::GetAttacker() const
{
	return m_pevAttacker;
}

void CHitscanComponent::SetAttacker(entvars_t* attacker)
{
	m_pevAttacker = attacker;
}

Vector CHitscanComponent::FireBullets()
{
	// Sanity checks:
	if ( VectorIsNull(m_ShootDir) || VectorIsNull(m_RightDir) || VectorIsNull(m_UpDir) || !m_pevInflictor )
	{
		return Vector();
	}

#ifdef CLIENT_DLL
	// The client doesn't actually do any bullet simulation, we just make sure that
	// the generated random vectors match up.
	return FireBullets_Client();
#else
	return FireBullets_Server();
#endif
}

#ifdef CLIENT_DLL
Vector CHitscanComponent::FireBullets_Client()
{
	SpreadPatternArgs spreadArgs {};

	spreadArgs.pattern = m_SpreadPattern;
	spreadArgs.baseSpread = m_BaseSpread;
	spreadArgs.randomSeed = m_RandomSeed;
	spreadArgs.totalShots = m_BulletsPerShot;
	spreadArgs.shotNumber = spreadArgs.totalShots - 1;

	// Just return the last vector we would have generated.
	const Vector2D spread = CalculateSpread(spreadArgs);

	return Vector(spread.x, spread.y, 0.0);
}

#else

Vector CHitscanComponent::FireBullets_Server()
{
	TraceResult tr;
	Vector2D modifiedSpread;

	ClearMultiDamage();
	gMultiDamage.type = DMG_BULLET | DMG_NEVERGIB;

	SpreadPatternArgs spreadArgs {};

	spreadArgs.pattern = m_SpreadPattern;
	spreadArgs.baseSpread = m_BaseSpread;
	spreadArgs.randomSeed = m_RandomSeed;
	spreadArgs.totalShots = m_BulletsPerShot;

	for ( spreadArgs.shotNumber = 0; spreadArgs.shotNumber < spreadArgs.totalShots; ++spreadArgs.shotNumber )
	{
		modifiedSpread = CalculateSpread(spreadArgs);

		Vector vecDir = m_ShootDir + (modifiedSpread.x * m_RightDir) + (modifiedSpread.y * m_UpDir);
		Vector vecEnd = m_GunPos + (vecDir * DEFAULT_BULLET_TRACE_DISTANCE);
		UTIL_TraceLine(m_GunPos, vecEnd, dont_ignore_monsters, ENT(m_pevInflictor), &tr);

		Debug_HitscanBulletFired(m_GunPos, tr);

		// do damage, paint decals
		if ( tr.flFraction != 1.0 )
		{
			CBaseEntity* pEntity = CBaseEntity::Instance(tr.pHit);

			pEntity->TraceAttack(m_pevAttacker, m_BaseDamagePerShot, vecDir, &tr, DMG_BULLET);
			TEXTURETYPE_PlaySound(&tr, m_GunPos, vecEnd, BULLET_GENERIC);
		}

		if ( m_SendTracerMessage )
		{
			MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, m_GunPos);
			WRITE_BYTE(TE_BULLET_TRACER);
			WRITE_COORD(m_GunPos.x);
			WRITE_COORD(m_GunPos.y);
			WRITE_COORD(m_GunPos.z);
			WRITE_COORD(tr.vecEndPos[0]);
			WRITE_COORD(tr.vecEndPos[1]);
			WRITE_COORD(tr.vecEndPos[2]);
			MESSAGE_END();
		}

		// make bullet trails
		// TODO: This should be clientside in the event playback.
		UTIL_BubbleTrail(m_GunPos, tr.vecEndPos, (int)((DEFAULT_BULLET_TRACE_DISTANCE * tr.flFraction) / 64.0));
	}

	Debug_FinaliseHitscanEvent();
	ApplyMultiDamage(m_pevInflictor, m_pevAttacker);

	return Vector(modifiedSpread.x, modifiedSpread.y, 0.0);
}

void CHitscanComponent::Debug_HitscanBulletFired(const Vector& start, const TraceResult& tr)
{
	if ( !m_Weapon )
	{
		// Can't currently debug shots that don't come from a weapon.
		return;
	}

	CWeaponDebugEventSource& evSource = CWeaponRegistry::StaticInstance().DebugEventSource();

	if ( !evSource.EventHasSubscribers(CWeaponDebugEvent_Base::EventType::Event_HitscanFire) )
	{
		return;
	}

	m_HitscanFireEvent.reset(new CWeaponDebugEvent_HitscanFire(*m_Weapon));
	m_HitscanFireEvent->AddTrace(start, tr);
}

void CHitscanComponent::Debug_FinaliseHitscanEvent()
{
	if ( !m_HitscanFireEvent )
	{
		return;
	}

	// The hitscan fire event will only have been created if we had event subscribers in the first place,
	// so no need to check this.
	CWeaponDebugEventSource& evSource = CWeaponRegistry::StaticInstance().DebugEventSource();
	evSource.FireEvent(m_HitscanFireEvent.get());

	m_HitscanFireEvent.reset();
}
#endif
