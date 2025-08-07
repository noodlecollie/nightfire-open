//========= Copyright (c) 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

// eventscripts.h
#pragma once
#if !defined(EVENTSCRIPTSH)
#define EVENTSCRIPTSH

// defaults for clientinfo messages
#define DEFAULT_VIEWHEIGHT 28
#define VEC_DUCK_VIEW_HEIGHT 12

#define FTENT_FADEOUT 0x00000080

// Some of these are HL/TFC specific?
void EV_EjectBrass(float* origin, float* velocity, float rotation, int model, int soundtype);
void EV_GetGunPosition(const struct event_args_s* args, float* pos, const float* origin);
qboolean EV_IsLocal(int idx);
qboolean EV_IsPlayer(int idx);
void EV_CreateTracer(float* start, float* end);
void EV_CreateBulletTracer(float* start, float* end);

void EV_GetDefaultShellInfo(
	const struct event_args_s* args,
	const float* origin,
	const float* velocity,
	float* ShellVelocity,
	float* ShellOrigin,
	const float* forward,
	const float* right,
	const float* up,
	float forwardScale,
	float upScale,
	float rightScale);

struct cl_entity_s* GetEntity(int idx);
struct cl_entity_s* GetViewEntity(void);
void EV_MuzzleFlash(void);
#endif  // EVENTSCRIPTSH
