#pragma once

#include <functional>

class CGenericWeapon;
class CBaseBotFightStyle;
class CBaseBot;
class CBaseEntity;

// A collection of functions used to provide useful information about a weapon to the bot AI.
// This is a plug-in item rather than an interface on the weapon class, because the bot code
// exists only on the server and inheriting from different classes on the server vs. the
// client sounds like a really awful idea.
class CBotWeaponAttributes
{
public:
	// Given distance to enemy, returns desire to use this weapon between 0 and 1 inclusive.
	typedef float (*Func_DesireToUse)(CGenericWeapon&, CBaseBot&, CBaseEntity&, float);

	// Set attributes on fightStyle in order to tell the bot how to use this weapon.
	typedef void (*Func_UseWeapon)(CGenericWeapon&, CBaseBotFightStyle&);

	// Convenience override - set to false to prevent a weapon from being used
	// at all by bots. This can be useful for things that are difficult for AI
	// to cope with (eg. placed explosives).
	inline bool CanBeUsed() const
	{
		return m_bCanBeUsed;
	}

	inline CBotWeaponAttributes& CanBeUsed(bool val)
	{
		m_bCanBeUsed = val;
		return *this;
	}

	inline CBotWeaponAttributes& DesireToUse(const Func_DesireToUse& func)
	{
		m_DesireToUse = func;
		return *this;
	}

	inline CBotWeaponAttributes& UseWeapon(const Func_UseWeapon& func)
	{
		m_UseWeapon = func;
		return *this;
	}

	inline float ExecDesireToUse(CGenericWeapon& weapon, CBaseBot& owner, CBaseEntity& enemy, float distanceToEnemy)
		const
	{
		return m_DesireToUse ? m_DesireToUse(weapon, owner, enemy, distanceToEnemy) : 0.0f;
	}

	inline bool ExecUseWeapon(CGenericWeapon& weapon, CBaseBotFightStyle& fightStyle) const
	{
		if ( !m_UseWeapon )
		{
			return false;
		}

		m_UseWeapon(weapon, fightStyle);
		return true;
	}

private:
	bool m_bCanBeUsed = true;
	Func_DesireToUse m_DesireToUse = NULL;
	Func_UseWeapon m_UseWeapon = NULL;
};