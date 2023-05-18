#pragma once

#include <cstring>
#include "standard_includes.h"
#include "weaponatts_base.h"
#include "weightedvaluelist.h"

namespace WeaponAtts
{
	struct WASoundSet : public WABase
	{
		float MinVolume = 1.0f;
		float MaxVolume = 1.0f;
		int MinPitch = 100;
		int MaxPitch = 100;
		float Attenuation = ATTN_NORM;
		int Flags = 0;

		// Names are expected to be compile-time constant!
		WeightedValueList<const char*> SoundNames;
	};
}  // namespace WeaponAtts
