#pragma once

#include "standard_includes.h"
#include "genericweapon.h"
#include "weaponmechanics/hitscanmechanic.h"

class CWeaponFrinesi : public CGenericWeapon
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
	bool PrepareToInvokeAttack(WeaponAtts::AttackMode mode) override;
	void AttackInvoked(WeaponAtts::AttackMode mode, const WeaponMechanics::InvocationResult& result) override;

private:
	float& NextPumpSoundTime();
	const float& NextPumpSoundTime() const;

	float& NextReloadSoundTime();
	const float& NextReloadSoundTime() const;

	void PlayPumpSound();
	bool FlagReloadInterrupt();

	WeaponMechanics::CHitscanMechanic* m_AutoAttackMode = nullptr;
	WeaponMechanics::CHitscanMechanic* m_PumpAttackMode = nullptr;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFrinesi>();
}
