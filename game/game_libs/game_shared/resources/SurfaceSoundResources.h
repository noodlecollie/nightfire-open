#pragma once

#include "BaseResourceCollection.h"

enum class SurfaceSoundId
{
	HitNone = 0,
	HitConcrete,
	HitMetal,
	HitVentDuct,
	HitMetalGrate,
	HitGlassA,
	HitGlassB,
	HitFlesh,
	HitFleshCritical,
	HitSnow,
	HitSand,
	HitWood,
	HitTile,
	HitWater,
	HitPlaster,
	HitPlastic,
	HitPaper,
	Count
};

class CSurfaceSoundResources :
	public CBaseResourceCollection<SurfaceSoundId, static_cast<size_t>(SurfaceSoundId::Count)>
{
public:
	CSurfaceSoundResources();
};
