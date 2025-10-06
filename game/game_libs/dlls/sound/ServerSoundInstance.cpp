#include "ServerSoundInstance.h"
#include "standard_includes.h"
#include "EnginePublicAPI/eiface.h"
#include "util.h"

namespace ServerSoundInstance
{
	void PlayAmbient(const CSoundInstance& instance, edict_t* entity)
	{
		if ( !instance.IsValid() )
		{
			return;
		}

		UTIL_EmitAmbientSound(
			entity ? entity : ENT(0),
			instance.Position(),
			instance.SoundPathCStr(),
			instance.Volume(),
			instance.Attenuation(),
			instance.Flags(),
			instance.GetPitch()
		);
	}

	void PlayLocalAmbient(const CSoundInstance& instance, edict_t* entity)
	{
		if ( !instance.IsValid() || !entity )
		{
			return;
		}

		EMIT_PLAYER_AMBIENT_SOUND(entity, instance.SoundPathCStr(), instance.Volume(), instance.GetPitch());
	}

	void PlayDynamic(const CSoundInstance& instance, edict_t* entity)
	{
		if ( !instance.IsValid() )
		{
			return;
		}

		EMIT_SOUND_DYN(
			entity ? entity : ENT(0),
			instance.Channel(),
			instance.SoundPathCStr(),
			instance.Volume(),
			instance.Attenuation(),
			instance.Flags(),
			instance.GetPitch()
		);
	}

	void StopDynamic(const CSoundInstance& instance, edict_t* entity)
	{
		if ( !instance.IsValid() )
		{
			return;
		}

		STOP_SOUND(entity ? entity : ENT(0), instance.Channel(), instance.SoundPathCStr());
	}
};  // namespace ServerSoundInstance
