#include "ClientSoundInstance.h"
#include "standard_includes.h"
#include "EnginePublicAPI/cdll_int.h"
#include "cl_dll.h"
#include "EnginePublicAPI/event_api.h"

namespace ClientSoundInstance
{
	void PlayAmbient(const CSoundInstance& instance)
	{
		if ( !instance.IsValid() )
		{
			return;
		}

		gEngfuncs.pfnPlaySoundByNameAtPitch(instance.SoundPathCStr(), instance.Volume(), instance.GetPitch());
	}

	void PlayPositional(const CSoundInstance& instance)
	{
		if ( !instance.IsValid() )
		{
			return;
		}

		gEngfuncs.pfnPlaySoundByNameAtLocation(instance.SoundPathCStr(), instance.Volume(), instance.Position());
	}

	void PlayEventSound(const CSoundInstance& instance, int entIndex)
	{
		gEngfuncs.pEventAPI->EV_PlaySound(
			entIndex,
			instance.Position(),
			instance.Channel(),
			instance.SoundPathCStr(),
			instance.Volume(),
			instance.Attenuation(),
			instance.Flags(),
			instance.GetPitch());
	}
}  // namespace ClientSoundInstance
