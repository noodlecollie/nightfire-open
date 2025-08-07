#pragma once

#include "MathLib/vec3.h"

struct edict_s;
class CBasePlayer;

class CBotrixPlayerInfo
{
public:
	explicit CBotrixPlayerInfo(struct edict_s* player);

	CBasePlayer* GetPlayerEntity() const;
	int GetPlayerEdictIndex() const;

	const char* GetName() const;
	int GetUserID() const;
	const char* GetNetworkIDString() const;
	int GetTeamIndex() const;
	void ChangeTeam(int newTeamIndex);
	int GetFragCount() const;
	int GetDeathCount() const;
	bool IsConnected() const;
	int GetArmorValue() const;

	bool IsHLTV() const;
	bool IsPlayer() const;
	bool IsFakeClient() const;
	bool IsDead() const;
	bool IsInAVehicle() const;
	bool IsObserver() const;

	Vector GetAbsOrigin() const;
	Vector GetEyePosition() const;
	Vector GetAbsAngles() const;
	Vector GetEyeAngles() const;
	Vector GetPlayerMins() const;
	Vector GetPlayerMaxs() const;
	const char* GetWeaponName() const;
	const char* GetModelName() const;
	int GetHealth() const;
	int GetMaxHealth() const;

	bool IsReplay() const;

private:
	struct edict_s* m_pPlayer = nullptr;
};
