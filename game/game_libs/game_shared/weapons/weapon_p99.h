#pragma once

#include "standard_includes.h"
#include "generichitscanweapon.h"

class CWeaponP99 : public CGenericHitscanWeapon
{
public:
	CWeaponP99();
	virtual const WeaponAtts::WACollection& WeaponAttributes() const override;

	virtual void SecondaryAttack() override;

	virtual bool ReadPredictionData(const weapon_data_t* from) override;
	virtual bool WritePredictionData(weapon_data_t* to) override;

#ifndef CLIENT_DLL
	virtual float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	virtual void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
	virtual int Save(CSave &save) override;
	virtual int Restore(CRestore &restore) override;
	static TYPEDESCRIPTION m_SaveData[];
#endif

private:
	const WeaponAtts::WAHitscanAttack* m_pAttackUnsilenced = nullptr;
	const WeaponAtts::WAHitscanAttack* m_pAttackSilenced = nullptr;
	bool m_bSilenced = false;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponP99>();
}
