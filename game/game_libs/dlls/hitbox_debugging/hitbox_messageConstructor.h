#pragma once

#include <cstdint>

class CBasePlayer;
class CBaseAnimating;
class CWeaponDebugEvent_HitscanFire;

class CHitboxMessageConstructor
{
public:
	CHitboxMessageConstructor(CBasePlayer* attacker, CBaseAnimating* target = nullptr);

	void FireMessages(const CWeaponDebugEvent_HitscanFire& event);
	void FireClearMessage();

private:
	void SendHitscanGeometry(const CWeaponDebugEvent_HitscanFire& event);
	void SendHitboxGeometry(const CWeaponDebugEvent_HitscanFire& event);

	CBasePlayer* m_Attacker = nullptr;
	CBaseAnimating* m_Target = nullptr;
	uint32_t m_HitboxCount = 0;
};
