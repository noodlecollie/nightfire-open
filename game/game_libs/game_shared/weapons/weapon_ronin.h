#pragma once

#include <cstdint>
#include "standard_includes.h"
#include "genericweapon.h"
#include "weaponmechanics/projectilemechanic.h"
#include "weaponmechanics/delegatedmechanic.h"

#ifndef CLIENT_DLL
class CNPCRoninTurret;
#endif

static const vec3_t RONIN_TURRET_MINS = {-14, -12, 0};
static const vec3_t RONIN_TURRET_MAXS = {14, 12, 16};

// NFTODO: Make a common base class from the frag grenade and use the
// same logic here. It'd be nice to be able to scale the toss velocity
// based on how long the button is held down for.
class CWeaponRonin : public CGenericWeapon
{
public:
	CWeaponRonin();
	const WeaponAtts::WACollection& WeaponAttributes() const override;

#ifndef CLIENT_DLL
	enum class TurretPickupType
	{
		ON_TOUCH,
		ON_USE
	};

	CNPCRoninTurret* GetTurret() const;
	bool PickUpUndeployedTurret(CNPCRoninTurret* turret, TurretPickupType pickupType);
	float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const override;
	void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const override;
#endif

private:
	static constexpr float PROJECTILE_SPAWN_DIST_IN_FRONT_OF_PLAYER = 32.0f;
	static constexpr float TURRET_PLACE_DIST_IN_FRONT_OF_PLAYER = 96.0f;

	void ThrowTurret(const WeaponMechanics::CProjectileMechanic& mechanic);
	WeaponMechanics::InvocationResult PlaceTurret(WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step);
	WeaponMechanics::InvocationResult ActivateTurret(WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step);
	void SendEvent(const WeaponMechanics::CDelegatedMechanic& mechanic);
	bool IsHoldingRonin() const;
	bool HasThrownButUndeployedRonin() const;
	bool SelectRoninThrowSpawnLocation(const Vector& forward, Vector& outLocation) const;
	bool SelectRoninPlaceSpawnLocation(Vector& outLocation) const;
	bool FitRoninAtLocation(const Vector& traceBegin, const Vector& deltaToIdealLocation, Vector& outLocation) const;
	void PostCreateTurret();

#ifndef CLIENT_DLL
	void LaunchThrownTurret(const Vector& forward, const Vector& spawnLocation);
	void PlaceTurret(const Vector& spawnLocation);
	CNPCRoninTurret* CreateTurret();
	void ActivateThrownTurret();
#endif

	WeaponMechanics::CProjectileMechanic* m_ThrowMechanic = nullptr;
	WeaponMechanics::CDelegatedMechanic* m_PlaceMechanic = nullptr;
	WeaponMechanics::CDelegatedMechanic* m_DeployMechanic = nullptr;
	EHANDLE m_Turret;
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponRonin>();
}
