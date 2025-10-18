#pragma once

#include "BaseResourceCollection.h"

enum class PlayerSoundId
{
	PainBond = 0,
	DieBond,
	PainMale,
	DieMale,
	PainFemale,
	DieFemale,
	Drown,
	Count
};

class CPlayerSoundResources : public CBaseResourceCollection<PlayerSoundId, static_cast<size_t>(PlayerSoundId::Count)>
{
public:
	CPlayerSoundResources();
};
