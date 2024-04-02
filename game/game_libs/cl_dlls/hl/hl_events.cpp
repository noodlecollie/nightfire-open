/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/

#include "../hud.h"
#include "../cl_util.h"
#include "EnginePublicAPI/event_api.h"
#include "ev_hldm.h"
#include "weaponregistry.h"
#include "genericweapon.h"

/*
======================
Game_HookEvents

Associate script file name with callback functions.  Callback's must be extern "C" so
 the engine doesn't get confused about name mangling stuff.  Note that the format is
 always the same.  Of course, a clever mod team could actually embed parameters, behavior
 into the actual .sc files and create a .sc file parser and hook their functionality through
 that.. i.e., a scripting system.

That was what we were going to do, but we ran out of time...oh well.
======================
*/
void Game_HookEvents(void)
{
	EV_HLDM_Init();

#ifdef HL_CONTENT
	gEngfuncs.pfnHookEvent("events/glock1.sc", EV_FireGlock1, NULL);
	gEngfuncs.pfnHookEvent("events/glock2.sc", EV_FireGlock2, NULL);
	gEngfuncs.pfnHookEvent("events/shotgun1.sc", EV_FireShotGunSingle, NULL);
	gEngfuncs.pfnHookEvent("events/shotgun2.sc", EV_FireShotGunDouble, NULL);
	gEngfuncs.pfnHookEvent("events/mp5.sc", EV_FireMP5, NULL);
	gEngfuncs.pfnHookEvent("events/mp52.sc", EV_FireMP52, NULL);
	gEngfuncs.pfnHookEvent("events/python.sc", EV_FirePython, NULL);
	gEngfuncs.pfnHookEvent("events/gauss.sc", EV_FireGauss, NULL);
	gEngfuncs.pfnHookEvent("events/gaussspin.sc", EV_SpinGauss, NULL);
	gEngfuncs.pfnHookEvent("events/train.sc", EV_TrainPitchAdjust, NULL);
	gEngfuncs.pfnHookEvent("events/crowbar.sc", EV_Crowbar, NULL);
	gEngfuncs.pfnHookEvent("events/crossbow1.sc", EV_FireCrossbow, NULL);
	gEngfuncs.pfnHookEvent("events/crossbow2.sc", EV_FireCrossbow2, NULL);
	gEngfuncs.pfnHookEvent("events/rpg.sc", EV_FireRpg, NULL);
	gEngfuncs.pfnHookEvent("events/egon_fire.sc", EV_EgonFire, NULL);
	gEngfuncs.pfnHookEvent("events/egon_stop.sc", EV_EgonStop, NULL);
	gEngfuncs.pfnHookEvent("events/firehornet.sc", EV_HornetGunFire, NULL);
	gEngfuncs.pfnHookEvent("events/tripfire.sc", EV_TripmineFire, NULL);
	gEngfuncs.pfnHookEvent("events/snarkfire.sc", EV_SnarkFire, NULL);
#endif // HL_CONTENT

	// Hook up all registered weapons to generic event handlers for firing.
	CWeaponRegistry::StaticInstance().ForEach(
		[](const WeaponAtts::WACollection& atts)
		{
			FOR_EACH_VEC(atts.AttackModes, index)
			{
				const WeaponAtts::WABaseAttack* attackMode = atts.AttackModes[index].get();
				gEngfuncs.pfnHookEvent(
					attackMode->EventScript,
					EV_HandleGenericWeaponFire,
					(void*)attackMode->Signature());
			}
		});
}
