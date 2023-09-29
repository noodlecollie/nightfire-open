#pragma once

#include <cstdint>

struct CAmmoDef
{
	const char* ClassName;
	const char* AmmoName;
	uint32_t MaxCarry;
	uint32_t AmmoBoxGive;
};

extern const CAmmoDef AmmoDef_9mm;
extern const CAmmoDef AmmoDef_357;
extern const CAmmoDef AmmoDef_Buckshot;
extern const CAmmoDef AmmoDef_Bolt;
extern const CAmmoDef AmmoDef_Rocket;
extern const CAmmoDef AmmoDef_HandGrenade;
extern const CAmmoDef AmmoDef_Satchel;
extern const CAmmoDef AmmoDef_Tripmine;
extern const CAmmoDef AmmoDef_Snark;
extern const CAmmoDef AmmoDef_Hornet;
extern const CAmmoDef AmmoDef_M203Grenade;
extern const CAmmoDef AmmoDef_Uranium;

// Common ammo pools that may be shared between weapons:
#define AMMOPOOL_9MM_NAME "ammo_pool_9mm"
#define AMMOPOOL_9MM_MAXCARRY 200
#define AMMOPOOL_9MM_AMMOBOXGIVE 32
