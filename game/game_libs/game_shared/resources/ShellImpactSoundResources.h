#pragma once

#include "BaseResourceCollection.h"
#include "EnginePublicAPI/surfaceprops.h"

enum class ShellType
{
	Default = 0,
	Shotgun,
	Heavy
};

enum class ShellImpactSoundId
{
	BrassOnHardSurface = 0,
	HollowOnHardSurface,
	SolidOnHardSurface,
	SolidOnMediumSurface,
	SolidOnMetalSurface,
	SolidOnWoodSurface,
	IntoWater,
	Count
};

class CShellImpactSoundResources :
	public CBaseResourceCollection<ShellImpactSoundId, static_cast<size_t>(ShellImpactSoundId::Count)>
{
public:
	CShellImpactSoundResources();

	const char* RandomResourcePathForImpact(ShellType shell, SurfaceProp surfaceProp) const;

private:
	const char* DefaultShellImpact(SurfaceProp surfaceProp) const;
	const char* ShotgunShellImpact(SurfaceProp surfaceProp) const;
	const char* HeavyShellImpact(SurfaceProp surfaceProp) const;
};
