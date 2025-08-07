#pragma once

#include "standard_includes.h"
#include "genericweapon.h"
#include "weaponmechanics/hitscanmechanic.h"

class CWeaponL96A1 : public CGenericWeapon
{
public:
	CWeaponL96A1();

	const WeaponAtts::WACollection& WeaponAttributes() const override;

	void Precache() override;
	void SecondaryAttack() override;
	void Reload() override;
	void Holster(int skiplocal = 0) override;
	void WeaponTick() override;

	bool ReadPredictionData(const weapon_data_t* from) override;
	bool WritePredictionData(weapon_data_t* to) override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif

private:
	static constexpr float SCOPE_MOVE_SPEED_FACTOR = 0.5f;

	void SetZoomLevel(uint32_t level);
	void PlayZoomSound();

	uint32_t m_iZoomLevel = 0;
	WeaponMechanics::CHitscanMechanic* m_AttackUnscoped = nullptr;
	WeaponMechanics::CHitscanMechanic* m_AttackScoped = nullptr;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponL96A1>();
}
