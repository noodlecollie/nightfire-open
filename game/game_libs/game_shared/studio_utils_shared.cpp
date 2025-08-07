#include "studio_utils_shared.h"
#include "EnginePublicAPI/const.h"
#include "EnginePublicAPI/edict.h"
#include "EnginePublicAPI/eiface.h"
#include "enginecallback.h"

float StudioGetAnimationDuration(int modelIndex, int anim)
{
	return MODEL_SEQ_DURATION(modelIndex, anim);
}

void StudioGetAnimationDurations(int modelIndex, CUtlVector<float>& outDurations)
{
	outDurations.Purge();

	if ( modelIndex < 1 )
	{
		return;
	}

	// TODO: Could probably be done better if we added this function to the engine.
	// Keep pushing durations until we come across one that's zero.
	for ( int animIndex = 0; true; ++animIndex )
	{
		const float duration = MODEL_SEQ_DURATION(modelIndex, animIndex);
		if ( duration <= 0.0f )
		{
			break;
		}

		outDurations.AddToTail(duration);
	}
}
