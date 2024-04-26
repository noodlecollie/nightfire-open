#include "weaponregistry.h"
#include "standard_includes.h"
#include "genericweapon.h"

CWeaponRegistry::CWeaponRegistry() :
	m_AttributesList {0}
{
}

CWeaponRegistry& CWeaponRegistry::StaticInstance()
{
	static CWeaponRegistry registry;
	return registry;
}

void CWeaponRegistry::Add(const WeaponAtts::WACollection* atts)
{
	if ( !atts )
	{
		return;
	}

	const int id = static_cast<int>(atts->Core.Id);
	ASSERTSZ_Q(id >= 0 && id < MAX_WEAPONS, "Weapon ID is out of range!");
	ASSERTSZ_Q(m_AttributesList[id] == NULL, "Attributes already present at this index.");

	m_AttributesList[id] = atts;
}

const WeaponAtts::WACollection* CWeaponRegistry::Get(int index) const
{
	// Don't allow index 0 as this indicates no weapon.
	return (index > 0 && index < MAX_WEAPONS) ? m_AttributesList[index] : NULL;
}

const WeaponAtts::WACollection* CWeaponRegistry::Get(WeaponId_e id) const
{
	return Get(static_cast<int>(id));
}

const WeaponAtts::WACollection* CWeaponRegistry::GetByName(const char* name) const
{
	for ( uint32_t index = 0; index < MAX_WEAPONS; ++index )
	{
		const WeaponAtts::WACollection* atts = m_AttributesList[index];

		// Note that index 0 (Weapon_None) will not have a valid pointer.
		if ( atts && strcmp(atts->Core.Classname, name) == 0 )
		{
			return atts;
		}
	}

	return nullptr;
}

void CWeaponRegistry::RegisterCvars()
{
	ForEach(
		[](const WeaponAtts::WACollection& atts)
		{
			atts.RegisterCvars();
		});
}

#ifndef CLIENT_DLL
CWeaponDebugEventSource& CWeaponRegistry::DebugEventSource()
{
	return m_DebugEventSource;
}

const CWeaponDebugEventSource& CWeaponRegistry::DebugEventSource() const
{
	return m_DebugEventSource;
}
#endif
