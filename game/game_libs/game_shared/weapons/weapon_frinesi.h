#pragma once

#include "standard_includes.h"
#include "generichitscanweapon.h"

class CWeaponFrinesi : public CGenericHitscanWeapon
{
public:
	CWeaponFrinesi();
	virtual void Precache() override;
	virtual void Holster(int skipLocal = 0) override;
	virtual void WeaponTick() override;

	virtual const WeaponAtts::WACollection& WeaponAttributes() const override;

	virtual bool ReadPredictionData(const weapon_data_t* from) override;
	virtual bool WritePredictionData(weapon_data_t* to) override;

#ifndef CLIENT_DLL
	virtual float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	virtual void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
	virtual int Save(CSave& save) override;
	virtual int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];
#endif

protected:
	int HandleSpecialReload(int currentState) override;
	bool InvokeWithAttackMode(
		const CGenericWeapon::WeaponAttackType type,
		const WeaponAtts::WABaseAttack* attackMode) override;

	Vector2D GetShotSpread(uint32_t shotNumber, uint32_t totalShots, const Vector2D baseSpread) const override;

private:
	void PlayPumpSound();
	bool FlagReloadInterrupt();

	const WeaponAtts::WABaseAttack* m_pAutoAttackMode = nullptr;
	const WeaponAtts::WABaseAttack* m_pPumpAttackMode = nullptr;
	float m_flNextPumpSoundTime;
	float m_flNextReloadSoundTime;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFrinesi>();
}
