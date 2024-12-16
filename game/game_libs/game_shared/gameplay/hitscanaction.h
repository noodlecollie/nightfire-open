#pragma once

#include <memory>
#include "weaponattributes/weaponatts_hitscanattack.h"
#include "MathLib/vec3.h"

#ifndef CLIENT_DLL
#include "weapondebugevents/weapondebugevent_hitscanfire.h"
#endif

class CGenericWeapon;

static constexpr float DEFAULT_BULLET_TRACE_DISTANCE = 8192;

class CHitscanAction
{
public:
	~CHitscanAction();

	Vector GetGunPos() const;
	void SetGunPos(const Vector& pos);

	Vector GetShootDir() const;
	void SetShootDir(const Vector& dir);

	Vector GetRightDir() const;
	void SetRightDir(const Vector& dir);

	Vector GetUpDir() const;
	void SetUpDir(const Vector& dir);

	int GetRandomSeed() const;
	void SetRandomSeed(int seed);

	CGenericWeapon* GetWeapon() const;
	void SetWeapon(CGenericWeapon* weapon);

	uint32_t GetBulletsPerShot() const;
	void SetBulletsPerShot(uint32_t bulletsPerShot);

	WeaponAtts::SpreadPattern GetSpreadPattern() const;
	void SetSpreadPattern(WeaponAtts::SpreadPattern pattern);

	// For weapons fired by the player, the tracer
	// is drawn as part of the event handling on the
	// client, so doesn't need to be sent individually.
	// For server-side entities like turrets, it will
	// need to be sent.
	bool GetSendTracerMessage() const;
	void SetSendTracerMessage(bool send);

	// For variable vs. fixed spread.
	void SetSpread(const WeaponAtts::AccuracyParameters& params, float inaccuracy);
	void SetSpread(const Vector2D& spread);

	// For skill-based vs. fixed damage.
	void SetBaseDamagePerShot(WeaponAtts::WASkillRecord::SkillDataEntryPtr ptr);
	void SetBaseDamagePerShot(float damage);

	entvars_t* GetInflictor() const;
	void SetInflictor(entvars_t* inflictor);

	entvars_t* GetAttacker() const;
	void SetAttacker(entvars_t* attacker);

	Vector FireBullets();

private:
#ifdef CLIENT_DLL
	Vector FireBullets_Client();
#else
	Vector FireBullets_Server();
	void Debug_HitscanBulletFired(const Vector& start, const TraceResult& tr);
	void Debug_FinaliseHitscanEvent();
#endif

	Vector m_GunPos;
	Vector m_ShootDir;
	Vector m_RightDir;
	Vector m_UpDir;
	int m_RandomSeed = 0;
	CGenericWeapon* m_Weapon = nullptr;
	uint32_t m_BulletsPerShot = 1;
	WeaponAtts::SpreadPattern m_SpreadPattern = WeaponAtts::SpreadPattern::Gaussian;
	bool m_SendTracerMessage = false;
	float m_BaseDamagePerShot = 0.0f;
	Vector2D m_BaseSpread;

	// The inflictor is usually the weapon,
	// and the attacker is usually the player.
	entvars_t* m_pevInflictor = nullptr;
	entvars_t* m_pevAttacker = nullptr;

#ifndef CLIENT_DLL
	std::unique_ptr<CWeaponDebugEvent_HitscanFire> m_HitscanFireEvent;
#endif
};
