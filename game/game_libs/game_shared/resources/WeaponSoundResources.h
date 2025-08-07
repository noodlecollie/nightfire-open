#pragma once

#include "BaseResourceCollection.h"

enum class WeaponSoundId
{
	BulletRicochet = 0,
	GrenadeBounce,
	Explosion,
	Count
};

class CWeaponSoundResources : public CBaseResourceCollection<WeaponSoundId, static_cast<size_t>(WeaponSoundId::Count)>
{
public:
	CWeaponSoundResources();
};
