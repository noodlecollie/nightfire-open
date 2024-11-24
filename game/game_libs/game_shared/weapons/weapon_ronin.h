#pragma once

#include "standard_includes.h"
#include "genericweapon.h"
#include "weaponmechanics/projectilemechanic.h"

#ifndef CLIENT_DLL
class CNPCRoninTurret;
#endif

// NFTODO: Make a common base class from the frag grenade and use the
// same logic here. It'd be nice to be able to scale the toss velocity
// based on how long the button is held down for.
class CWeaponRonin : public CGenericWeapon
{
public:
	CWeaponRonin();
	const WeaponAtts::WACollection& WeaponAttributes() const override;

#ifndef CLIENT_DLL
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif

protected:
#ifndef CLIENT_DLL
	void LaunchThrownTurret(const WeaponMechanics::CProjectileMechanic& mechanic);
#endif

private:
	static constexpr float PROJECTILE_SPAWN_DIST_IN_FRONT_OF_PLAYER = 32.0f;

#ifndef CLIENT_DLL
	bool SelectRoninSpawnLocation(CNPCRoninTurret& turret, const Vector& forward, Vector& outLocation) const;
#endif

	WeaponMechanics::CProjectileMechanic* m_ThrowMechanic = nullptr;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponRonin>();
}
