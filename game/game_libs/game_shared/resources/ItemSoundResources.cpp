#include "ItemSoundResources.h"

CItemSoundResources::CItemSoundResources() :
	CBaseResourceCollection<ItemSoundId, static_cast<size_t>(ItemSoundId::Count)>()
{
	InitialiseResource(ItemSoundId::AmmoPickup, "items/ammopickup1.wav");
	InitialiseResource(ItemSoundId::WeaponDrop, "items/weapondrop1.wav");
	InitialiseResource(ItemSoundId::WeaponPickup, "items/gunpickup1.wav");
}
