#pragma once

#include "BaseResourceCollection.h"

enum class MaterialBreakSoundId
{
	BreakNone = 0,
	BreakWood,
	BreakFlesh,
	BreakGlass,
	BreakMetal,
	BreakConcrete,
	Count
};

class CMaterialBreakSoundResources :
	public CBaseResourceCollection<MaterialBreakSoundId, static_cast<size_t>(MaterialBreakSoundId::Count)>
{
public:
	CMaterialBreakSoundResources();
};
