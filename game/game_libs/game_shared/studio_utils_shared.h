#pragma once

#include "utlvector.h"

typedef struct model_s model_t;

float StudioGetAnimationDuration(model_t* model, int anim);
float StudioGetAnimationDuration(int modelIndex, int anim);

void StudioGetAnimationDurations(model_t* model, CUtlVector<float>& outDurations);
void StudioGetAnimationDurations(int modelIndex, CUtlVector<float>& outDurations);
