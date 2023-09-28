#pragma once

#include <cstdint>
#include "MathLib/vec3.h"
#include "utlstring.h"

class CSoundInstance
{
public:
	CSoundInstance();

	void ClearSettings();
	bool IsValid() const;

	Vector Position() const;
	void SetPosition(const Vector& pos);

	CUtlString SoundPath() const;
	const char* SoundPathCStr() const;
	void SetSoundPath(const CUtlString& path);
	void SetSoundPath(const char* path);

	float Volume() const;
	void SetVolume(float vol);

	float Attenuation() const;
	void SetAttenuation(float attn);

	uint32_t Flags() const;
	void SetFlags(uint32_t flags);

	int32_t MinPitch() const;
	int32_t MaxPitch() const;
	int32_t GetPitch() const;
	void SetPitch(int32_t pitch);
	void SetPitch(int32_t minPitch, int32_t maxPitch);

	int32_t Channel() const;
	void SetChannel(int32_t channel);

private:
	Vector m_vecPosition;
	CUtlString m_SoundPath;
	float m_flVolume;
	float m_flAttenuation;
	uint32_t m_iFlags;
	int32_t m_iPitchMin;
	int32_t m_iPitchMax;
	int32_t m_iChannel;
};
