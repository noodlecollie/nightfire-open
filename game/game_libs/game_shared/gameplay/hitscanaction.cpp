#include "gameplay/hitscanaction.h"
#include "gameplay/inaccuracymodifiers.h"
#include "gameplay/inaccuracyCvars.h"
#include "gameplay/spreadPatterns.h"
#include "weapons/weaponregistry.h"

#ifndef CLIENT_DLL
#include "weapondebugevents/weapondebugeventsource.h"
#endif

CHitscanAction::~CHitscanAction()
{
	// Required for using std::unique_ptr with a forward-declared type.
}

Vector CHitscanAction::GetGunPos() const
{
	return m_GunPos;
}

void CHitscanAction::SetGunPos(const Vector& pos)
{
	m_GunPos = pos;
}

Vector CHitscanAction::GetShootDir() const
{
	return m_ShootDir;
}

void CHitscanAction::SetShootDir(const Vector& dir)
{
	m_ShootDir = dir;
}

Vector CHitscanAction::GetRightDir() const
{
	return m_RightDir;
}

void CHitscanAction::SetRightDir(const Vector& dir)
{
	m_RightDir = dir;
}

Vector CHitscanAction::GetUpDir() const
{
	return m_UpDir;
}

void CHitscanAction::SetUpDir(const Vector& dir)
{
	m_UpDir = dir;
}

int CHitscanAction::GetRandomSeed() const
{
	return m_RandomSeed;
}

void CHitscanAction::SetRandomSeed(int seed)
{
	m_RandomSeed = seed;
}

CGenericWeapon* CHitscanAction::GetWeapon() const
{
	return m_Weapon;
}

void CHitscanAction::SetWeapon(CGenericWeapon* weapon)
{
	m_Weapon = weapon;
}

uint32_t CHitscanAction::GetBulletsPerShot() const
{
	return m_BulletsPerShot;
}

void CHitscanAction::SetBulletsPerShot(uint32_t bulletsPerShot)
{
	m_BulletsPerShot = bulletsPerShot;
}

WeaponAtts::SpreadPattern CHitscanAction::GetSpreadPattern() const
{
	return m_SpreadPattern;
}

void CHitscanAction::SetSpreadPattern(WeaponAtts::SpreadPattern pattern)
{
	m_SpreadPattern = pattern;
}

bool CHitscanAction::GetSendTracerMessage() const
{
	return m_SendTracerMessage;
}

void CHitscanAction::SetSendTracerMessage(bool send)
{
	m_SendTracerMessage = send;
}

void CHitscanAction::SetSpread(const WeaponAtts::AccuracyParameters& params, float inaccuracy)
{
	WeaponAtts::AccuracyParameters localAccuracyParams(params);

	if ( InaccuracyModifiers::IsInaccuracyDebuggingEnabled() )
	{
		InaccuracyModifiers::GetInaccuracyValuesFromDebugCvars(localAccuracyParams);
	}

	m_BaseSpread = InaccuracyModifiers::GetInterpolatedSpread(localAccuracyParams, inaccuracy);
}

void CHitscanAction::SetSpread(const Vector2D& spread)
{
	m_BaseSpread = spread;
}

void CHitscanAction::SetBaseDamagePerShot(WeaponAtts::WASkillRecord::SkillDataEntryPtr ptr)
{
	m_BaseDamagePerShot = ptr ? gSkillData.*ptr : 0.0f;
}

void CHitscanAction::SetBaseDamagePerShot(float damage)
{
	m_BaseDamagePerShot = damage;
}

entvars_t* CHitscanAction::GetInflictor() const
{
	return m_pevInflictor;
}

void CHitscanAction::SetInflictor(entvars_t* inflictor)
{
	m_pevInflictor = inflictor;
}

entvars_t* CHitscanAction::GetAttacker() const
{
	return m_pevAttacker;
}

void CHitscanAction::SetAttacker(entvars_t* attacker)
{
	m_pevAttacker = attacker;
}

Vector CHitscanAction::FireBullets()
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
Vector CHitscanAction::FireBullets_Client()
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

Vector CHitscanAction::FireBullets_Server()
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

void CHitscanAction::Debug_HitscanBulletFired(const Vector& start, const TraceResult& tr)
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

void CHitscanAction::Debug_FinaliseHitscanEvent()
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
