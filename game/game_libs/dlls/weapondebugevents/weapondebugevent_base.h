#pragma once

class CGenericWeapon;
class CBasePlayer;

class CWeaponDebugEvent_Base
{
public:
	enum class EventType
	{
		Event_HitscanFire = 0
	};

	virtual ~CWeaponDebugEvent_Base()
	{
	}

	virtual EventType Type() const = 0;

	CGenericWeapon* Weapon() const;
	CBasePlayer* WeaponOwner() const;

protected:
	CWeaponDebugEvent_Base(CGenericWeapon& weapon);

	CGenericWeapon* m_pWeapon;
};
