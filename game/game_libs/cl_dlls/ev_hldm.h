//========= Copyright (c) 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#pragma once

#include "EnginePublicAPI/surfaceprops.h"

struct pmtrace_s;
struct event_args_s;

extern "C"
{
#ifdef HL_CONTENT
// HLDM
void EV_FireGlock1(event_args_t* args);
void EV_FireGlock2(event_args_t* args);
void EV_FireShotGunSingle(event_args_t* args);
void EV_FireShotGunDouble(event_args_t* args);
void EV_FireMP5(event_args_t* args);
void EV_FireMP52(event_args_t* args);
void EV_FirePython(event_args_t* args);
void EV_FireGauss(event_args_t* args);
void EV_SpinGauss(event_args_t* args);
void EV_Crowbar(event_args_t* args);
void EV_FireCrossbow(event_args_t* args);
void EV_FireCrossbow2(event_args_t* args);
void EV_FireRpg(event_args_t* args);
void EV_EgonFire(event_args_t* args);
void EV_EgonStop(event_args_t* args);
void EV_HornetGunFire(event_args_t* args);
void EV_TripmineFire(event_args_t* args);
void EV_SnarkFire(event_args_t* args);
#endif  // HL_CONTENT

void EV_TrainPitchAdjust(event_args_t* args);

void EV_HandleGenericWeaponFire(struct event_args_s* args);
}

void EV_HLDM_Init();
void EV_HLDM_PlayTextureSound(int idx, struct pmtrace_s* ptr, float* vecSrc, float* vecEnd, int iBulletType);
enum _SurfaceProp EV_HLDM_GetSurfacePropForTexture(int idx, struct pmtrace_s* ptr, float* vecSrc, float* vecEnd);
void EV_HLDM_GunshotDecalTrace(struct pmtrace_s* pTrace, const char* decalName);
void EV_HLDM_DecalGunshot(struct pmtrace_s* pTrace, int iBulletType);
void EV_HLDM_DecalGunshotNew(int idx, struct pmtrace_s* ptr, float* vecSrc, float* vecEnd);
void EV_HLDM_CheckTracer(int idx, float* vecSrc, float* end, float* forward, float* right, int iBulletType);
void EV_HLDM_FireBullets(
	int idx,
	float* forward,
	float* right,
	float* up,
	int cShots,
	float* vecSrc,
	float* vecDirShooting,
	float flDistance,
	int iBulletType,
	int iTracerFreq,
	float flSpreadX,
	float flSpreadY);
