#include "SoundInstance.h"
#include "standard_includes.h"
#include "EnginePublicAPI/eiface.h"
#include "util.h"
#include "projectInterface/IProjectInterface.h"

CSoundInstance::CSoundInstance()
{
	ClearSettings();
}

bool CSoundInstance::IsValid() const
{
	return m_SoundPath.Length() > 0;
}

void CSoundInstance::ClearSettings()
{
	m_vecPosition = Vector();
	m_SoundPath.Clear();
	m_flVolume = 1.0f;
	m_flAttenuation = ATTN_NORM;
	m_iFlags = 0;
	m_iPitchMin = 100;
	m_iPitchMax = 100;
	m_iChannel = CHAN_STATIC;
}

Vector CSoundInstance::Position() const
{
	return m_vecPosition;
}

void CSoundInstance::SetPosition(const Vector& pos)
{
	m_vecPosition = pos;
}

CUtlString CSoundInstance::SoundPath() const
{
	return m_SoundPath;
}

const char* CSoundInstance::SoundPathCStr() const
{
	return m_SoundPath.Get();
}

void CSoundInstance::SetSoundPath(const CUtlString& path)
{
	m_SoundPath = path;
}

void CSoundInstance::SetSoundPath(const char* path)
{
	m_SoundPath = CUtlString(path);
}

float CSoundInstance::Volume() const
{
	return m_flVolume;
}

void CSoundInstance::SetVolume(float vol)
{
	if ( vol > 1.0f )
	{
		vol = 1.0f;
	}
	else if ( vol < 0.0f )
	{
		vol = 0.0f;
	}

	m_flVolume = vol;
}

float CSoundInstance::Attenuation() const
{
	return m_flAttenuation;
}

void CSoundInstance::SetAttenuation(float attn)
{
	if ( attn < 0.0f )
	{
		attn = 0.0f;
	}

	m_flAttenuation = attn;
}

uint32_t CSoundInstance::Flags() const
{
	return m_iFlags;
}

void CSoundInstance::SetFlags(uint32_t flags)
{
	m_iFlags = flags;
}

int32_t CSoundInstance::MinPitch() const
{
	return m_iPitchMin;
}

int32_t CSoundInstance::MaxPitch() const
{
	return m_iPitchMax;
}

int32_t CSoundInstance::GetPitch() const
{
	if ( m_iPitchMin == m_iPitchMax )
	{
		return m_iPitchMin;
	}

	return IProjectInterface::ProjectInterfaceImpl()->RNG().GetRandomInt(m_iPitchMin, m_iPitchMax);
}

void CSoundInstance::SetPitch(int32_t pitch)
{
	m_iPitchMin = m_iPitchMax = pitch;
}

void CSoundInstance::SetPitch(int32_t minPitch, int32_t maxPitch)
{
	m_iPitchMin = minPitch;
	m_iPitchMax = maxPitch;

	if ( m_iPitchMin > m_iPitchMax )
	{
		int32_t temp = m_iPitchMin;
		m_iPitchMin = m_iPitchMax;
		m_iPitchMax = temp;
	}
}

void CSoundInstance::ChooseRandomPitch(int32_t minPitch, int32_t maxPitch)
{
	SetPitch(IProjectInterface::ProjectInterfaceImpl()->RNG().GetRandomInt(minPitch, maxPitch));
}

int32_t CSoundInstance::Channel() const
{
	return m_iChannel;
}

void CSoundInstance::SetChannel(int32_t channel)
{
	m_iChannel = channel;
}
