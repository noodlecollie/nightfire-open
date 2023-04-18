#pragma once

#include <functional>
#include "weaponatts_base.h"
#include "weightedvaluelist.h"
#include "weaponatts_soundset.h"

namespace WeaponAtts
{
	struct WAViewModel : public WABase
	{
		const char* ModelName = NULL;
		int Anim_Draw = -1;

		WeightedValueList<int> AnimList_Idle;
		WeightedValueList<int> AnimList_Reload;

		// These animations are specific to when the clip is empty.
		// If there are no empty-specific animations, these lists can be ignored.
		WeightedValueList<int> AnimList_IdleEmpty;
		WeightedValueList<int> AnimList_ReloadEmpty;

		WASoundSet ReloadSounds;

		virtual void Validate() const override
		{
			ASSERTSZ_Q(ModelName, "View model must be specified.");
			ASSERTSZ_Q(AnimList_Idle.Count() > 0, "At least one idle animation must be specified.");
		}
	};
}
