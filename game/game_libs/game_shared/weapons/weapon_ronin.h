#pragma once

#include <cstdint>
#include "standard_includes.h"
#include "genericweapon.h"
#include "weaponmechanics/projectilemechanic.h"
#include "weaponmechanics/delegatedmechanic.h"

#ifndef CLIENT_DLL
namespace CustomGeometry
{
	class CMessageWriter;
}

class CNPCRoninTurret;

extern cvar_t debug_ronin_placement;
extern cvar_t sv_ronin_place_fov;
#endif

extern const vec3_t RONIN_TURRET_MINS;
extern const vec3_t RONIN_TURRET_MAXS;

// NFTODO: Make a common base class from the frag grenade and use the
// same logic here. It'd be nice to be able to scale the toss velocity
// based on how long the button is held down for.
class CWeaponRonin : public CGenericWeapon
{
public:
	CWeaponRonin();
	const WeaponAtts::WACollection& WeaponAttributes() const override;
	void Spawn() override;
	BOOL IsUseable() override;
	BOOL CanDeploy() override;
	BOOL Deploy() override;
	void Drop() override;
	int AddToPlayer(CBasePlayer* pPlayer) override;
	int AddDuplicate(CBasePlayerItem* pOriginal) override;
	void DetachFromPlayer(CBasePlayer* pPlayer) override;
	void ItemPostFrame() override;
	bool ReadPredictionData(const weapon_data_t* from) override;
	bool WritePredictionData(weapon_data_t* to) override;

#ifndef CLIENT_DLL
	enum class TurretPickupType
	{
		ON_TOUCH,
		ON_USE
	};

	static bool AddRoninWeaponToPlayer(
		CBasePlayer* player,
		TurretPickupType pickupType = TurretPickupType::ON_TOUCH,
		CNPCRoninTurret* turret = nullptr
	);

	CNPCRoninTurret* GetTurret() const;
	bool PickUpTurret(TurretPickupType pickupType, CNPCRoninTurret* turret = nullptr);

	// Used to update internal state based on the passed turret.
	// Pass null to clear out the stored turret.
	void RefreshStateFromTurret(CNPCRoninTurret* turret);

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];
#endif

private:
#define RONIN_STATE_LIST \
	LIST_ITEM(RONIN_HELD) \
	LIST_ITEM(RONIN_THROWN) \
	LIST_ITEM(RONIN_DEPLOYING) \
	LIST_ITEM(RONIN_DEPLOYED) \
	LIST_ITEM(RONIN_GONE)

	enum class State
	{
#define LIST_ITEM(name) name,
		RONIN_STATE_LIST
#undef LIST_ITEM
	};

	static constexpr float PROJECTILE_SPAWN_DIST_IN_FRONT_OF_PLAYER = 32.0f;
	static constexpr float TURRET_PLACE_HDIST = 32.0f;

	bool ThrowTurret(const WeaponMechanics::CProjectileMechanic& mechanic);
	WeaponMechanics::InvocationResult PlaceTurret(WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step);
	WeaponMechanics::InvocationResult ActivateTurret(WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step);
	WeaponMechanics::InvocationResult DetonateTurret(WeaponMechanics::CDelegatedMechanic& mechanic, uint32_t step);
	void SendEvent(const WeaponMechanics::CDelegatedMechanic& mechanic);
	bool IsHoldingRonin() const;
	bool HasThrownButUndeployedRonin() const;
	bool SelectRoninThrowSpawnLocation(const Vector& forward, Vector& outLocation) const;
	bool SelectRoninPlaceSpawnLocation(Vector& outLocation) const;
	bool FitRoninAtLocation(const Vector& traceBegin, const Vector& deltaToIdealLocation, Vector& outLocation) const;
	void Redeploy();
	void UpdateState();
	void UpdateState(State idealState);
	State DecideNewState(State baseState) const;
	void SetNewState(State state);
	void RespondToNewState(State oldState);
	void UpdateAmmo();
	void UpdateWeaponMechanics();

#ifndef CLIENT_DLL
	State ComputeTurretRelatedState(State currentState) const;
	void LaunchThrownTurret(const Vector& forward, const Vector& spawnLocation);
	void PlaceTurret(const Vector& spawnLocation);
	CNPCRoninTurret* CreateTurret();
	void ActivateThrownTurret();
	void DetonateThrownTurret();
	void DestroyTurret();
	void DrawDebugBounds(CustomGeometry::CMessageWriter* writer, const Vector& location, uint32_t colour) const;
#endif

	WeaponMechanics::CProjectileMechanic* m_ThrowMechanic = nullptr;
	WeaponMechanics::CDelegatedMechanic* m_PlaceMechanic = nullptr;
	WeaponMechanics::CDelegatedMechanic* m_DeployMechanic = nullptr;
	WeaponMechanics::CDelegatedMechanic* m_DetonateMechanic = nullptr;
	State m_State = State::RONIN_HELD;
	bool m_InStateUpdate = false;

#ifndef CLIENT_DLL
	EHANDLE m_Turret;
#endif
};

namespace WeaponAtts
{
	template<>
	const WACollection& StaticWeaponAttributes<CWeaponRonin>();
}
