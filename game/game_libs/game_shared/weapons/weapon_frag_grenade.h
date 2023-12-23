#pragma once

#include "standard_includes.h"
#include "weapons/basegrenadelauncher.h"
#include "weapons.h"

class CWeaponFragGrenade : public CBaseGrenadeLauncher
{
public:
	CWeaponFragGrenade();

	const WeaponAtts::WACollection& WeaponAttributes() const override;
	void Precache() override;
	void WeaponTick() override;
	void PrimaryAttack() override;
	bool ReadPredictionData(const weapon_data_t* from) override;
	bool WritePredictionData(weapon_data_t* to) override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

protected:
	void CreateProjectile(const WeaponAtts::WAProjectileAttack& projectileAttack) override;
#endif

private:
	enum class ThrowState
	{
		Idle = 0,	// Nothing is happening
		Primed,		// Player has pulled the pin
		Released	// Player has let go of the fire button but grenade is not yet thrown
	};

	ThrowState m_ThrowState = ThrowState::Idle;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFragGrenade>();
}
