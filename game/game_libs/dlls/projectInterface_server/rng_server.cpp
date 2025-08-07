#include "rng_server.h"
#include "standard_includes.h"

float RNG_Server::GetRandomFloat(float min, float max)
{
	return g_engfuncs.pfnRandomFloat(min, max);
}

int32_t RNG_Server::GetRandomInt(int32_t min, int32_t max)
{
	return g_engfuncs.pfnRandomLong(min, max);
}
