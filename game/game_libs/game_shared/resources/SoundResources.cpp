#include "SoundResources.h"
#include "standard_includes.h"
#include "enginecallback.h"

namespace SoundResources
{
	CItemSoundResources ItemSounds;
	CWeaponSoundResources WeaponSounds;
	CSurfaceSoundResources SurfaceSounds;
	CFootstepSoundResources FootstepSounds;
	CPlayerSoundResources PlayerSounds;
	CShellImpactSoundResources ShellSounds;
	CMaterialBreakSoundResources BreakSounds;

	static void Precache(const IIterableResourceCollection& collection)
	{
		for ( IIterableResourceCollection::Iterator it = collection.Begin(); it.IsValid(); ++it )
		{
			PRECACHE_SOUND(it.Path());
		}
	}

	void PrecacheAll()
	{
		// TODO: Do we want a way to precache subsets of these sounds?
		// Eg. so that we don't have to precache multiplayer-only sounds in singleplayer?
		Precache(ItemSounds);
		Precache(WeaponSounds);
		Precache(SurfaceSounds);
		Precache(FootstepSounds);
		Precache(PlayerSounds);
		Precache(ShellSounds);
		Precache(BreakSounds);
	}
}  // namespace SoundResources
