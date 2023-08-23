#pragma once

#include "standard_includes.h"
#include "generichitscanweapon.h"

class CWeaponL96A1 : public CGenericHitscanWeapon
{
public:
	CWeaponL96A1();

	virtual const WeaponAtts::WACollection& WeaponAttributes() const override;

	virtual void Precache() override;
	virtual void SecondaryAttack() override;
	virtual void Reload() override;
	virtual void Holster(int skiplocal = 0) override;
	virtual void WeaponTick() override;

#ifndef CLIENT_DLL
	virtual float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	virtual void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif

private:
	static constexpr float SCOPE_MOVE_SPEED_FACTOR = 0.5f;

	void SetZoomLevel(uint32_t level);
	void PlayZoomSound();

	uint32_t m_iZoomLevel = 0;
	const WeaponAtts::WAHitscanAttack* m_pAttackUnscoped;
	const WeaponAtts::WAHitscanAttack* m_pAttackScoped;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponL96A1>();
}
