#pragma once

#include "standard_includes.h"
#include "generichitscanweapon.h"

class CWeaponFrinesi : public CGenericHitscanWeapon
{
public:
	CWeaponFrinesi();
	void Precache() override;
	void Holster(int skipLocal = 0) override;
	void WeaponTick() override;

	const WeaponAtts::WACollection& WeaponAttributes() const override;

	bool ReadPredictionData(const weapon_data_t* from) override;
	bool WritePredictionData(weapon_data_t* to) override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];
#endif

protected:
	int HandleSpecialReload(int currentState) override;
	bool InvokeWithAttackMode(
		const CGenericWeapon::WeaponAttackType type,
		const WeaponAtts::WABaseAttack* attackMode) override;

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
