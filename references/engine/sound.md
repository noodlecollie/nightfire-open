## Server-Side

* `EMIT_SOUND_DYN(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int flags, int pitch)`: Used to emit a server-side sound that can follow an entity's location.
    * Calls `EMIT_SOUND_DYN2()`, which is an alias for `enginefuncs_t::pfnEmitSound()`.
* `UTIL_EmitAmbientSound(edict_t *entity, const Vector &vecOrigin, const char *samp, float vol, float attenuation, int fFlags, int pitch)`: Emits a server-side sound from a fixed location.
    * Calls `EMIT_AMBIENT_SOUND()`, which is an alias for `enginefuncs_t::pfnEmitAmbientSound()`.

## Client-Side

* `cl_enginefunc_t::pfnPlaySoundByNameAtLocation(char *szSound, float volume, float *origin)`: Plays a sound to the given client only, located at the given origin in the world.
    * This is the function used by StudioMDL event 5004.
* `cl_enginefunc_t::pfnPlaySoundByName(char *szSound, float volume)`: Plays a sound specified by name to the given client only. The sound is played directly to the client and is not considered to be located at any world position.
* `cl_enginefunc_t::pfnPlaySoundByIndex(int iSound, float volume)`: Same as above, but a sound index is used instead of a file name.
* `cl_enginefunc_t::pfnPlaySoundByNameAtPitch(char *szSound, float volume, int pitch)`: Same as `pfnPlaySoundByName()`, but the pitch of the sound can be specified.
* `cl_enginefunc_t::pfnPlaySoundVoiceByName(char *szSound, float volume, int pitch)`: Unsure. Doesn't seem to be used in the Half Life libraries.
