#pragma once

#include "BaseResourceCollection.h"

enum class ItemSoundId
{
	AmmoPickup = 0,
	WeaponPickup,
	WeaponDrop,
	Spark,
	Count
};

class CItemSoundResources : public CBaseResourceCollection<ItemSoundId, static_cast<size_t>(ItemSoundId::Count)>
{
public:
	CItemSoundResources();
};
