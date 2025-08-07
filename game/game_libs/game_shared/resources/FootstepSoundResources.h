#pragma once

#include "BaseResourceCollection.h"

enum class FootstepSoundId
{
	None = 0,
	Carpet,
	ConcreteClean,
	ConcreteGrit,
	Dirt,
	Glass,
	Grass,
	Ice,
	Ladder,
	Metal,
	MetalGrate,
	Snow,
	VentDuct,
	WoodClean,
	WoodGrit,
	ShallowWater,
	DeepWater,
	Swim,
	Count
};

class CFootstepSoundResources :
	public CBaseResourceCollection<FootstepSoundId, static_cast<size_t>(FootstepSoundId::Count)>
{
public:
	enum class Foot
	{
		Right,
		Left
	};

	CFootstepSoundResources();

	const char* RandomFootstepSoundPath(FootstepSoundId id, Foot foot) const;
};
