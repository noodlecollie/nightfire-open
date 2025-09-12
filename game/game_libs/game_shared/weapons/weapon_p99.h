#pragma once

#include "standard_includes.h"
#include "genericweapon.h"
#include "weaponmechanics/hitscanmechanic.h"

class CWeaponP99 : public CGenericWeapon
{
public:
	CWeaponP99();
	const WeaponAtts::WACollection& WeaponAttributes() const override;

	void SecondaryAttack() override;

	bool ReadPredictionData(const weapon_data_t* from) override;
	bool WritePredictionData(weapon_data_t* to) override;

#ifndef CLIENT_DLL
	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];
#endif

private:
	WeaponMechanics::CHitscanMechanic* m_UnsilencedAttack = nullptr;
	WeaponMechanics::CHitscanMechanic* m_SilencedAttack = nullptr;
	bool m_bSilenced = false;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponP99>();
}
