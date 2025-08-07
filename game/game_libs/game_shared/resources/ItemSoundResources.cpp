#include "ItemSoundResources.h"

static constexpr const char* const Sparks[] = {
	"items/spark1.wav",
	"items/spark2.wav",
	"items/spark3.wav",
	"items/spark4.wav",
	"items/spark5.wav",
	"items/spark6.wav",
};

CItemSoundResources::CItemSoundResources() :
	CBaseResourceCollection<ItemSoundId, static_cast<size_t>(ItemSoundId::Count)>()
{
	InitialiseResource(ItemSoundId::AmmoPickup, "items/ammopickup1.wav");
	InitialiseResource(ItemSoundId::WeaponDrop, "items/weapondrop1.wav");
	InitialiseResource(ItemSoundId::WeaponPickup, "items/gunpickup1.wav");
	InitialiseResources(ItemSoundId::Spark, Sparks);
}
