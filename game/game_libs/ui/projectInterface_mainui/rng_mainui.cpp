#include "rng_mainui.h"
#include "enginecallback_menu.h"

float RNG_MainUI::GetRandomFloat(float min, float max)
{
	return EngFuncs::RandomFloat(min, max);
}

int32_t RNG_MainUI::GetRandomInt(int32_t min, int32_t max)
{
	return EngFuncs::RandomLong(min, max);
}
