#pragma once

#include "standard_includes.h"
#include "generichitscanweapon.h"

class CWeaponFrinesi : public CGenericHitscanWeapon
{
public:
	CWeaponFrinesi();
	void Precache() override;
	void Spawn() override;
	void Holster(int skipLocal = 0) override;
	void WeaponTick() override;

	const WeaponAtts::WACollection& WeaponAttributes() const override;

	bool ReadPredictionData(const weapon_data_t* from) override;
	bool WritePredictionData(weapon_data_t* to) override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif

protected:
	int HandleSpecialReload(int currentState) override;
	bool InvokeWithAttackMode(WeaponAtts::AttackMode mode, const WeaponAtts::WABaseAttack* attack) override;

private:
	float& NextPumpSoundTime();
	const float& NextPumpSoundTime() const;

	float& NextReloadSoundTime();
	const float& NextReloadSoundTime() const;

	void PlayPumpSound();
	bool FlagReloadInterrupt();

	const WeaponAtts::WABaseAttack* m_pAutoAttackMode = nullptr;
	const WeaponAtts::WABaseAttack* m_pPumpAttackMode = nullptr;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFrinesi>();
}
