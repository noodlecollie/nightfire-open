#pragma once

#include "standard_includes.h"
#include "SurfaceSoundResources.h"
#include "SurfaceDecalResources.h"
#include "FootstepSoundResources.h"
#include "EnginePublicAPI/surfaceprops.h"

class CSurfaceAttributes
{
public:
	// Default values should be zero.
	struct Attributes
	{
		SurfaceSoundId hitSound;
		float hitSoundVol;
		float ricochetSoundVol;
		float ricochetChance;
		SurfaceDecalId decal;
		FootstepSoundId footstepSound;
		float footstepSoundVolRun;
		float footstepSoundVolWalk;
	};

	static CSurfaceAttributes& StaticInstance();

	CSurfaceAttributes();
	const Attributes& GetAttributes(SurfaceProp surfaceProp) const;

private:
	Attributes m_Attributes[SurfaceProp__Count];
};
