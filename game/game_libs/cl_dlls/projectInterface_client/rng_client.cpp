#include "rng_client.h"
#include "standard_includes.h"
#include "enginecallback.h"
#include "cl_dll.h"

float RNG_Client::GetRandomFloat(float min, float max)
{
	return gEngfuncs.pfnRandomFloat(min, max);
}

int32_t RNG_Client::GetRandomInt(int32_t min, int32_t max)
{
	return gEngfuncs.pfnRandomLong(min, max);
}
