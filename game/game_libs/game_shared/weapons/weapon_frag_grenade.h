#pragma once

#include "standard_includes.h"
#include "weapons/genericweapon.h"
#include "weapons.h"
#include "weaponmechanics/projectilemechanic.h"

class CWeaponFragGrenade : public CGenericWeapon
{
public:
	CWeaponFragGrenade();

	const WeaponAtts::WACollection& WeaponAttributes() const override;
	void WeaponTick() override;
	bool ReadPredictionData(const weapon_data_t* from) override;
	bool WritePredictionData(weapon_data_t* to) override;

#ifndef CLIENT_DLL
	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];
#endif

protected:
	bool PrepareToInvokeAttack(WeaponAtts::AttackMode mode) override;

#ifndef CLIENT_DLL
	void CreateProjectile(const WeaponMechanics::CProjectileMechanic& mechanic);
#endif

private:
	enum class ThrowState
	{
		Idle = 0,  // Nothing is happening
		Primed,  // Player has pulled the pin
		Released  // Player has let go of the fire button but grenade is not yet thrown
	};

	ThrowState m_ThrowState = ThrowState::Idle;
	WeaponMechanics::CProjectileMechanic* m_ThrowMechanic = nullptr;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponFragGrenade>();
}
